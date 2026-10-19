/*************************************************************
 * UlteSoft WebAssembly Virtual Machine (Version 2)          *
 * Copyright (c) 2025-present UlteSoft. All rights reserved. *
 * Licensed under the APL-2.0 License (see LICENSE file).    *
 *************************************************************/
#pragma once
#include "config.h"
#include "owned_rmw_service.h"
#include <uwvm2/imported/ros/ipc/rmw_publish.h>
#include <uwvm2/imported/ros/ipc/rmw_subscription.h>
#include <exception>
#include "event_stream.h"
#include "async_service.h"
#include <uwvm2/imported/ros/ipc/custom_interface.h>
#include <condition_variable>
#include <optional>
#include <uwvm2/imported/ros/ipc/image.h>
#include <uwvm2/imported/ros/ipc/transform.h>
#include <uwvm2/imported/ros/ipc/parameter_event.h>
#include <uwvm2/imported/ros/ipc/action.h>
#include <uwvm2/imported/ros/common/manifest.h>
#include <deque>
#include <thread>
#include <functional>
#include <atomic>
#include <csignal>
#include <sys/syscall.h>

namespace uwvm2::ros_gateway
{
    struct bucket
    {
        double tokens{}, bytes{};
        std::chrono::steady_clock::time_point last{std::chrono::steady_clock::now()};
        bool initialized{};
        double rate{},burst{},bandwidth{};
        void refresh(ros::capability_spec const& c)
        {
            if(!initialized)
            {
                rate=positive(field(c,"max-rate-hz"));burst=static_cast<double>(limit(c,"burst"));bandwidth=static_cast<double>(limit(c,"max-bytes-per-second"));
                if(c.limits.count("max-log-bytes-per-second")) { bandwidth=std::min(bandwidth,static_cast<double>(limit(c,"max-log-bytes-per-second"))); }
                tokens=burst;bytes=bandwidth;initialized=true;
            }
            auto now{std::chrono::steady_clock::now()};
            double elapsed{std::chrono::duration<double>(now-last).count()}; last=now;
            tokens=std::min(burst,tokens+elapsed*rate); bytes=std::min(bandwidth,bytes+elapsed*bandwidth);
        }
        bool available(ros::capability_spec const& c,std::size_t count) {refresh(c);return tokens>=1&&bytes>=static_cast<double>(count);}
        bool available_bytes(ros::capability_spec const& c,std::size_t count) {refresh(c);return bytes>=static_cast<double>(count);}
        bool take(ros::capability_spec const& c,std::size_t count)
        { refresh(c);if(tokens<1||bytes<static_cast<double>(count)){return false;}tokens-=1;bytes-=static_cast<double>(count);return true; }
        bool take_bytes(ros::capability_spec const& c,std::size_t count)
        { refresh(c);if(bytes<static_cast<double>(count)){return false;}bytes-=static_cast<double>(count);return true; }
    };
    inline bool validate_payload(ros::capability_spec const& c,std::vector<std::uint8_t> const& data)
    {
        for(unsigned version:{1u,2u})
        {
            if(auto item=ipc::find_custom_service(version,c.type))
            {
                return c.type_identity==item->native_identity&&field(c,"adapter-codec-identity")==item->codec_identity&&
                    ipc::validate_custom_service_request(*item,c.operation,data.data(),data.size());
            }
            if(auto item=ipc::find_custom_action(version,c.type))
            {
                return c.type_identity==item->native_identity&&field(c,"adapter-codec-identity")==item->codec_identity&&
                    ipc::validate_custom_action_request(*item,c.operation,data.data(),data.size());
            }
        }
        auto custom{ipc::find_custom_topic_for_operation(1,c.type,c.operation)};
        if(!custom) { custom=ipc::find_custom_topic_for_operation(2,c.type,c.operation); }
        if(custom)
        {
            auto codec{c.limits.find("adapter-codec-identity")};
            return (c.operation=="topic.publish"||c.operation=="topic.subscribe")&&
                   c.type_identity==custom->native_identity&&codec!=c.limits.end()&&codec->second==custom->codec_identity&&
                   ipc::validate_custom_topic(*custom,data.data(),data.size());
        }
        if(c.type=="sensor_msgs/Image"||c.type=="sensor_msgs/msg/Image") {ipc::image_view image;return ipc::decode_image_view(data.data(),data.size(),c.type=="sensor_msgs/Image"?1:2,image);}
        if(c.type=="std_msgs/String"||c.type=="std_msgs/msg/String") { return data.size()>=4&&ipc::get(data.data(),4)==data.size()-4&&valid_utf8(data.data()+4,data.size()-4); }
        if(c.type=="std_msgs/Bool"||c.type=="std_msgs/msg/Bool"||c.type=="std_srvs/SetBool"||c.type=="std_srvs/srv/SetBool") { return data.size()==1&&data[0]<=1; }
        if(c.type=="std_srvs/Trigger"||c.type=="std_srvs/srv/Trigger") { return data.empty(); }
        if(c.type=="geometry_msgs/Twist"||c.type=="geometry_msgs/msg/Twist")
        {
            if(data.size()!=48) { return false; }
            for(unsigned i{};i!=6;++i)
            {
                auto bits{ipc::get(data.data()+8*i,8)}; double value{}; std::memcpy(&value,&bits,8);
                if(!std::isfinite(value)) { return false; }
                if(field(c,"validator")=="twist-bounds"&&std::abs(value)>positive(field(c,i<3?"twist.linear-max-abs":"twist.angular-max-abs"))) { return false; }
            }
            return true;
        }
        return false;
    }
    // Legacy consumers retain their raw body ABI. A separately granted observer
    // selects the versioned destination before native activation; the two paths
    // never race to consume a shared message. Callback admission is nonblocking.
    struct event_queue
    {
        ros::capability_spec capability;
        mutable std::mutex mutex;
        std::deque<std::vector<std::uint8_t>> events;
        std::optional<std::vector<std::uint8_t>> pending;
        std::size_t bytes{};
        std::uint64_t dropped{};
        bool active{true};
        bucket quota;
        event_queue_statistics totals{};
        std::shared_ptr<event_stream> observer;
        explicit event_queue(ros::capability_spec c):capability{std::move(c)} {}
        static void bump(std::uint64_t& value) noexcept { if(value!=UINT64_MAX) { ++value; } }
        std::uint64_t depth_limit() const
        { return limit(capability,(capability.operation=="topic.subscribe")?(capability.type.find("/msg/")==std::string::npos?"queue-depth":"qos.depth"):"max-event-queue-depth"); }
        std::uint64_t byte_limit() const
        { return limit(capability,capability.operation=="topic.subscribe"?"max-queue-bytes":"max-event-queue-bytes"); }
        bool fits(std::size_t size) const { auto maximum{byte_limit()};return events.size()<depth_limit()&&bytes<=maximum&&size<=maximum-bytes; }
        void high_water() noexcept
        { totals.high_water_depth=std::max(totals.high_water_depth,std::uint64_t(events.size()+(pending?1:0)));totals.high_water_bytes=std::max(totals.high_water_bytes,std::uint64_t(bytes+(pending?pending->capacity():0))); }
        void promote()
        { if(pending&&fits(pending->capacity())) { auto size{pending->capacity()};events.push_back(std::move(*pending));pending.reset();bytes+=size; } }
        bool enqueue(std::vector<std::uint8_t> data)
        {
            std::lock_guard<std::mutex> guard{mutex};
            bool service{capability.operation=="service.advertise"},parameter{capability.operation=="param.subscribe-events"},action{capability.operation=="action.advertise"};
            bool valid{},selected_custom{};
            if(service||action)
            {
                if(data.size()>=8&&ipc::get(data.data(),8))
                {
                    if(service)
                    {
                        for(unsigned version:{1u,2u}) { if(auto item=ipc::find_custom_service(version,capability.type)) {selected_custom=true;auto codec{capability.limits.find("adapter-codec-identity")};valid=capability.type_identity==item->native_identity&&codec!=capability.limits.end()&&codec->second==item->codec_identity&&data.size()-8<=limit(capability,"max-request-bytes")&&ipc::validate_custom_service_event(*item,data.data(),data.size());} }
                        if(!selected_custom&&data.size()-8<=limit(capability,"max-request-bytes")) {bool trigger{capability.type=="std_srvs/Trigger"||capability.type=="std_srvs/srv/Trigger"},set_bool{capability.type=="std_srvs/SetBool"||capability.type=="std_srvs/srv/SetBool"};valid=(trigger&&data.size()==8)||(set_bool&&data.size()==9&&data[8]<=1);}
                    }
                    else
                    {
                        for(unsigned version:{1u,2u}) { if(auto item=ipc::find_custom_action(version,capability.type)) {selected_custom=true;auto codec{capability.limits.find("adapter-codec-identity")};valid=capability.type_identity==item->native_identity&&codec!=capability.limits.end()&&codec->second==item->codec_identity&&data.size()-8<=limit(capability,"max-goal-bytes")&&ipc::validate_custom_action_event(*item,data.data(),data.size());} }
                        if(!selected_custom&&data.size()==12&&(capability.type=="actionlib_tutorials/Fibonacci"||capability.type=="action_tutorials_interfaces/action/Fibonacci")) {std::int32_t order{};valid=data.size()-8<=limit(capability,"max-goal-bytes")&&ipc::fibonacci_order(data.data()+8,data.size()-8,order)&&static_cast<std::uint64_t>(order)<=limit(capability,"fibonacci.max-order");}
                    }
                }
            }
            else if(parameter)
            {
                if(ipc::parameter_event_type(ros::version::ros2,capability.type)!=ipc::parameter_kind::unsupported) { valid=!ipc::validate_typed_parameter_event(ros::version::ros2,capability,data); }
                else { ipc::parameter_event event;valid=data.size()<=limit(capability,"max-parameter-bytes")&&ipc::decode_parameter_event(data,event)&&event.node==field(capability,"parameter-node")&&event.name==field(capability,"parameter-name"); }
            }
            else { valid=data.size()<=limit(capability,"max-message-bytes")&&validate_payload(capability,data); }
            if(!active||!valid||!quota.take(capability,data.size())) { bump(dropped);bump(totals.rejected);return false; }
            if(observer)
            {
                ipc::event_kind kind{};std::uint8_t flags{};if(!ipc::event_source_kind(capability.operation,kind,flags)) { bump(dropped);return false; }
                auto id{service||action?ipc::get(data.data(),8):std::uint64_t{0}};
                auto error{observer->enqueue({kind,flags,0,id,std::move(data)})};if(error) { bump(dropped); }return !error;
            }
            promote();auto size{data.capacity()};
            if(!pending&&fits(size)) { bytes+=size;events.push_back(std::move(data));bump(totals.enqueued);high_water();return true; }
            auto overflow{field(capability,"overflow-policy")};
            if(overflow=="backpressure")
            {
                bump(totals.backpressured);
                if(!pending&&size<=limit(capability,"max-pending-event-bytes")) { pending=std::move(data);bump(totals.enqueued);high_water();return true; }
                bump(totals.admission_refused);bump(dropped);return false;
            }
            while(!fits(size)&&overflow=="drop-oldest"&&!events.empty()) { bytes-=events.front().capacity();events.pop_front();bump(dropped);bump(totals.dropped_oldest); }
            if(!fits(size)) { bump(dropped);bump(totals.admission_refused);if(overflow=="drop-newest") { bump(totals.dropped_newest); }else { bump(totals.rejected); }return false; }
            bytes+=size;events.push_back(std::move(data));bump(totals.enqueued);high_water();return true;
        }
        ipc::call_result poll()
        {
            std::lock_guard<std::mutex> guard{mutex};if(!active) { return {125,{}}; }if(observer) { return {17,{}}; }
            promote();if(events.empty()) { return {11,{}}; }auto size{events.front().capacity()};auto value{std::move(events.front())};events.pop_front();bytes-=size;bump(totals.dequeued);promote();return {0,std::move(value)};
        }
        event_queue_statistics statistics() const noexcept
        { std::lock_guard<std::mutex> guard{mutex};auto value{totals};value.depth=events.size();value.bytes=bytes;value.pending_depth=pending?1:0;value.pending_bytes=pending?pending->capacity():0;return value; }
        void deactivate(std::uint32_t reason=125)
        { std::lock_guard<std::mutex> guard{mutex};active=false;events.clear();pending.reset();bytes=0;bump(totals.closed);if(observer) { observer->seal(reason); } }
    };
    struct adapter:owned_service_backend
    {
        virtual ~adapter()=default;
        virtual ipc::call_result publish(ros::capability_spec const&,std::vector<std::uint8_t> const&)=0;
        virtual ipc::call_result subscribe(ros::capability_spec const&,std::shared_ptr<event_queue>)=0;
        virtual ipc::call_result service(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t)=0;
        virtual ipc::call_result operation(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) { return {95,{}}; }
        virtual ipc::call_result advertise(ros::capability_spec const&,std::shared_ptr<event_queue>) { return {95,{}}; }
        virtual ipc::call_result advertise_action(ros::capability_spec const&,std::shared_ptr<event_queue>) { return {95,{}}; }
        // Non-consuming ownership/QoS qualification. Reading a terminal result
        // here would retire the goal before the observer had delivery credit.
        virtual ipc::call_result observe_action(ros::capability_spec const&,std::uint64_t) { return {95,{}}; }
        virtual ipc::call_result service_availability(ros::capability_spec const&,ros::capability_spec const&,
            ipc::client_availability::packet const&,std::uint32_t) { return {95,{}}; }
        virtual void retire_service_availability(ipc::client_availability::context const&,ipc::client_availability::retirement,
            std::string_view,std::string_view) noexcept {}
        virtual void clear_service_availability() noexcept {}
        virtual ipc::call_result owned_rmw_publisher(ros::capability_spec const&,
            ipc::rmw_endpoint::packet const&,ipc::deadline) { return {95,{}}; }
        virtual ipc::call_result owned_rmw_publish(ros::capability_spec const&,ipc::rmw_publish::header const&,
            std::uint8_t const*,std::size_t,ipc::deadline) { return {95,{}}; }
        virtual void clear_owned_rmw_publishers() noexcept {}
        // Optional native subscription route. No default implementation opens
        // an endpoint or pretends that an old UWRE queue carries native facts.
        virtual ipc::call_result owned_rmw_subscription(ros::capability_spec const&,
            ipc::rmw_endpoint::packet const&,ipc::deadline) {return {95,{}};}
        virtual ipc::call_result owned_rmw_subscription_transfer(ros::capability_spec const&,
            ipc::rmw_subscription::control const&,ipc::deadline) {return {95,{}};}
        virtual std::uint32_t owned_rmw_subscription_delivery_failed(ros::capability_spec const&,
            ipc::rmw_endpoint::binding const&,std::uint64_t,std::uint32_t) noexcept {return 95;}
        virtual void clear_owned_rmw_subscriptions() noexcept {}
        virtual void close(std::string const&)=0;
    };
    /** Validate the actual whole canonical topic body without a second owned
     * body copy. Metadata association/extent has already been checked by UMRS.
     * This is exclusively the new subscription route; old validators stay exact.
     */
    inline bool valid_owned_subscription_body(ros::capability_spec const& c,std::uint8_t const* p,std::size_t n)
    {
        if(c.operation!="topic.subscribe"||(!p&&n)){return false;}
        if(auto custom=ipc::find_custom_topic_for_operation(2,c.type,c.operation))
        {return c.type_identity==custom->native_identity&&field(c,"adapter-codec-identity")==custom->codec_identity&&
            ipc::validate_custom_topic(*custom,p,n);}
        if(c.type=="std_msgs/msg/String"){return n>=4&&ipc::get(p,4)==n-4&&valid_utf8(p+4,n-4);}
        if(c.type=="std_msgs/msg/Bool"){return n==1&&p[0]<=1;}
        if(c.type=="sensor_msgs/msg/Image"){ipc::image_view image;return ipc::decode_image_view(p,n,2,image);}
        if(c.type=="geometry_msgs/msg/Twist")
        {
            if(n!=48){return false;}
            for(unsigned i{};i!=6;++i)
            {
                auto bits=ipc::get(p+8*i,8);double value{};std::memcpy(&value,&bits,8);
                if(!std::isfinite(value)){return false;}
                if(field(c,"validator")=="twist-bounds"&&std::abs(value)>positive(field(c,i<3?"twist.linear-max-abs":"twist.angular-max-abs"))){return false;}
            }
            return true;
        }
        return false;
    }
    /** A successful peek owns one reservation until its whole reply is sent.
     * Arm immediately after the registry's successful no-throw vector move,
     * before validation/quota/audit/output preparation can throw. A failed peek
     * never arms: it must not cancel an earlier attempt's still-live borrow.
     * The registry separately retains primary/cancel/retire observations.
     */
    struct owned_subscription_delivery
    {
        std::shared_ptr<adapter>& backend;
        ros::capability_spec const* capability{};
        ipc::rmw_endpoint::binding owner{};
        std::uint64_t incarnation{};
        bool armed{};
        explicit owned_subscription_delivery(std::shared_ptr<adapter>& value) noexcept:backend{value}{}
        owned_subscription_delivery(owned_subscription_delivery const&)=delete;
        owned_subscription_delivery& operator=(owned_subscription_delivery const&)=delete;
        void arm(ros::capability_spec const& cap,ipc::rmw_subscription::control const& request) noexcept
        {capability=&cap;owner=request.owner;incarnation=request.incarnation;armed=true;}
        void fail(std::uint32_t primary) noexcept
        {
            if(!armed){return;}armed=false;
            if(backend&&capability){(void)backend->owned_rmw_subscription_delivery_failed(*capability,owner,incarnation,primary?primary:107);}
        }
        void delivered() noexcept {armed=false;}
        ~owned_subscription_delivery(){fail(std::uncaught_exceptions()?5u:107u);}
    };
    // The same signal-safe flag is read by independent connection workers.
    // Volatile sig_atomic_t alone is NOT C++ inter-thread synchronization.
    static_assert(std::atomic<std::sig_atomic_t>::is_always_lock_free,
        "Gateway signal and worker cancellation require actual lock-free atomics");
    inline std::atomic<std::sig_atomic_t> stopping{0};
    inline void stop(int) noexcept { stopping.store(1,std::memory_order_relaxed); }
    struct audit_limiter
    {
        std::chrono::steady_clock::time_point window{std::chrono::steady_clock::now()},updated{window};
        std::uint64_t suppressed{},total_records{},total_bytes{};
        std::uint32_t written{};
        double tokens{};
        bool initialized{};
        // Each grant limiter belongs to one serve_connection request thread.
        // Its settings come only from the validated immutable configuration
        // entry; close/revoke does not replace that entry or reset this state.
        // Keep these borrowed identities within that configuration/session.
        configuration const* configured_owner{};
        ros::capability_spec const* configured_grant{};
        double audit_burst{},audit_rate{};
        std::uint64_t audit_record_limit{},audit_byte_limit{};
    };
    inline void audit(std::uint64_t session,ipc::request const& r,ros::capability_spec const* c,std::uint32_t error,audit_limiter& global,audit_limiter& grant,configuration const& cfg)
    {
        if(!error&&c&&field(*c,"audit")!="allow-and-deny") { return; }
        auto now{std::chrono::steady_clock::now()};
        if(now-global.window>=std::chrono::seconds(1)) { global.window=now;global.written=0; }
        native_io::fixed_text<1024> record;
        bool formatted{record.append("ros_audit timestamp_ns=")&&record.number(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count())&&
            record.append(" backend=")&&record.number(static_cast<unsigned>(cfg.policy.ros_version))&&record.append(" worker_uid=")&&record.number(cfg.worker_uid)&&
            record.append(" worker_pid=")&&record.number(cfg.worker_pid)&&record.append(" session=")&&record.number(session)&&record.append(" operation=")&&record.append(r.operation)&&
            record.append(" alias=")&&record.append(r.alias)};
        if(formatted&&c&&c->operation=="service.available"&&
           (r.operation=="service.available-v1"||r.operation=="service.available-retire-v1"||r.operation=="rmw.service-v1"))
        { formatted=record.append(" source_alias=")&&record.append(field(*c,"service-source-alias")); }
        if(formatted&&owned_service_operation(r.operation))
        {
            ipc::rmw_service::control observed{};
            if(ipc::rmw_service::request(r.data.data(),r.data.size(),observed))
                formatted=record.append(" rmw_command=")&&record.number(static_cast<unsigned>(observed.operation))&&
                    record.append(" source_handle=")&&record.number(observed.owner.source)&&record.append(" source_instance=")&&record.number(observed.owner.instance)&&
                    record.append(" source_generation=")&&record.number(observed.owner.generation)&&record.append(" incarnation=")&&record.number(observed.incarnation)&&
                    record.append(" peer_handle=")&&record.number(observed.peer);
        }
        formatted=formatted&&record.append(" error=")&&record.number(error)&&record.append("\n");
        auto text{record.view()};
        auto suppress=[&] { if(global.suppressed!=std::numeric_limits<std::uint64_t>::max()) { ++global.suppressed; } };
        if(!formatted||global.written>=cfg.audit_records_per_second||global.total_records>=cfg.audit_max_records||text.size()>cfg.audit_max_bytes-std::min(global.total_bytes,cfg.audit_max_bytes)) { suppress();return; }
        if(c)
        {
            if(!grant.initialized)
            {
                auto configured{cfg.policy.capabilities.find(r.alias)};
                if(configured==cfg.policy.capabilities.end()||&configured->second!=c||c->alias!=r.alias)
                { throw std::runtime_error("audit limiter requires its configured grant"); }
                grant.audit_burst=static_cast<double>(limit(*c,"audit-burst"));
                grant.audit_rate=positive(field(*c,"audit-max-rate-hz"));
                grant.audit_record_limit=limit(*c,"audit-max-records");
                grant.audit_byte_limit=limit(*c,"audit-max-bytes");
                grant.configured_owner=&cfg;grant.configured_grant=c;
                grant.tokens=grant.audit_burst;grant.initialized=true;
            }
            else if(grant.configured_owner!=&cfg||grant.configured_grant!=c||c->alias!=r.alias)
            { throw std::runtime_error("audit limiter configured grant changed"); }
            grant.tokens=std::min(grant.audit_burst,grant.tokens+std::chrono::duration<double>(now-grant.updated).count()*grant.audit_rate);grant.updated=now;
            if(grant.tokens<1||grant.total_records>=grant.audit_record_limit||text.size()>grant.audit_byte_limit-std::min(grant.total_bytes,grant.audit_byte_limit)) { suppress();return; }
            grant.tokens-=1;++grant.total_records;grant.total_bytes+=text.size();
        }
        ++global.written;++global.total_records;global.total_bytes+=text.size();
        // No message content is logged. A stalled supervisor pipe cannot block IPC.
        if(!native_io::write_audit_text(text)) { suppress(); }
    }
    inline void serve_connection(int fd,configuration const& cfg,std::function<std::unique_ptr<adapter>()> const& factory,int worker_pidfd)
    {
        struct fd_guard { int value; ~fd_guard() { ::close(value); } } guard{fd};
        std::uint32_t uid{},pid{};
        pollfd worker_lifetime{worker_pidfd,POLLIN,0};
        if(::poll(&worker_lifetime,1,0)!=0||!ipc::peer_credentials(fd,uid,pid)||uid!=cfg.worker_uid||pid!=cfg.worker_pid) { return; }
        ipc::frame f;
        if(!ipc::read_frame(fd,f,ipc::after(cfg.timeout_ms),worker_pidfd)||f.type!=ipc::kind::hello||f.backend!=static_cast<std::uint8_t>(cfg.policy.ros_version)||f.request_id!=1||f.response_id||f.session_id||f.error||!f.payload.empty()) { return; }
        auto session{ipc::random_session()}; if(!session) { return; }
        ipc::frame answer; answer.type=ipc::kind::response; answer.backend=f.backend; answer.response_id=f.request_id; answer.session_id=session; answer.timeout_ms=cfg.timeout_ms;
        if(!ipc::write_frame(fd,answer,ipc::after(cfg.timeout_ms),worker_pidfd)) { return; }
        std::shared_ptr<adapter> backend;
        // Declared after backend: the table retires while that real owner is alive.
        std::unique_ptr<owned_service_route> owned_services;
        struct observation
        {
            ros::capability_spec source;
            std::shared_ptr<ros::capability_spec const> source_contract;
            std::uint64_t correlation{};
            std::shared_ptr<event_stream> stream;
            std::shared_ptr<async_service_source> async{std::make_shared<async_service_source>()};
            std::shared_ptr<event_queue> queue;
        };
        std::map<std::string,observation,std::less<>> observations;
        auto action_consumer_claimed=[&](ros::capability_spec const& source,std::uint64_t goal)
        {
            for(auto const& item:observations)
            {
                auto const& watch{item.second};
                if(watch.stream->active()&&ipc::event_action_consumer_conflict(source.operation,source.resource,
                    source.type,goal,watch.source.operation,watch.source.resource,watch.source.type,
                    watch.correlation,watch.stream->allowed_kinds())) {return true;}
            }
            return false;
        };
        async_service_pool service_tasks;
        // Declared AFTER the async pool so exceptional unwinding also clears
        // query clients before the pool joins unrelated outstanding operations.
        struct availability_cleanup
        {
            std::shared_ptr<adapter>& value;bool cleared{};
            void clear() noexcept { if(!cleared) { cleared=true;if(value) { value->clear_service_availability(); } } }
            ~availability_cleanup() { clear(); }
        } availability_guard{backend};
        struct owned_rmw_cleanup
        {
            std::shared_ptr<adapter>& value;
            std::unique_ptr<owned_service_route>& services;bool cleared{};
            // This guard is declared AFTER service_tasks. Every normal and
            // exceptional exit clears real service rows BEFORE that pool joins.
            void clear() noexcept {if(!cleared){cleared=true;if(services)services->clear();if(value){value->clear_owned_rmw_subscriptions();value->clear_owned_rmw_publishers();}}}
            ~owned_rmw_cleanup(){clear();}
        } owned_rmw_guard{backend,owned_services};
        bool bound{};
        std::map<std::string,std::shared_ptr<event_queue>,std::less<>> queues;
        std::set<std::string> revoked;
        ros::service_availability_history availability_history;
        std::map<std::string,bucket,std::less<>> quotas;
        // Native entity creation is independently bounded by the SAME explicit
        // source rate/burst. It must not spend a callback message token: a burst1
        // latched subscription still has one data-admission token after create.
        std::map<std::string,bucket,std::less<>> activation_quotas;
        std::uint64_t last{1};
        audit_limiter log_limit;
        std::map<std::string,audit_limiter,std::less<>> grant_logs;
        while(!stopping.load(std::memory_order_relaxed))
        {
            if(!ipc::read_frame(fd,f,ipc::after(cfg.timeout_ms),worker_pidfd)) { break; }
            if(f.backend!=static_cast<std::uint8_t>(cfg.policy.ros_version)||f.session_id!=session||f.request_id<=last||f.response_id||f.error) { break; }
            last=f.request_id;
            if(f.type==ipc::kind::close||f.type==ipc::kind::cancel) { break; }
            if(f.type!=ipc::kind::request) { break; }
            ipc::request r; if(!ipc::decode_request(f.payload,r)) { break; }
            auto started{std::chrono::steady_clock::now()};
            auto wire_until{started+std::chrono::milliseconds(std::min(f.timeout_ms,cfg.timeout_ms))};
            auto until{wire_until};
            auto remaining_budget=[&](std::uint32_t maximum)
            {
                auto now{std::chrono::steady_clock::now()};if(now>=until) { return std::uint32_t{}; }
                auto milliseconds{std::chrono::duration_cast<std::chrono::milliseconds>(until-now).count()};
                return static_cast<std::uint32_t>(std::min<std::uint64_t>(maximum,static_cast<std::uint64_t>(milliseconds)));
            };
            ipc::call_result result;bool native_invoked{};
            owned_subscription_delivery subscription_delivery{backend};
            bool subscription_ack_attempted{},subscription_endpoint_reply{};
            auto cap{cfg.policy.capabilities.find(r.alias)}; ros::capability_spec const* c{cap==cfg.policy.capabilities.end()?nullptr:&cap->second};
            if(c&&c->limits.count("timeout-ms")) { until=std::min(until,started+std::chrono::milliseconds(limit(*c,"timeout-ms"))); }
            if(r.operation=="session.bind")
            {
                if(bound||r.alias!="target"||r.data!=ros::serialize_manifest(cfg.policy)) { result.error=13; }
                else { backend=factory(); if(!backend)throw std::runtime_error("Adapter factory returned no owner");
                    if(backend->owned_rmw_service_capacity())owned_services=std::make_unique<owned_service_route>(cfg,*backend,fd,worker_pidfd,&stopping);bound=true; }
            }
            else if(!bound) { result.error=13; }
            else if(!c) { result.error=13; }
            // New direct-RCL route uses saved full pair history and the actual
            // immutable grants; neither aliases nor authority come from UMRC.
            else if(owned_service_operation(r.operation))
            {
                auto quota=[&](ros::capability_spec const& selected,std::size_t bytes,owned_service_route::charge_kind kind)
                {
                    using type=owned_service_route::charge_kind;
                    if(kind==type::activate)return activation_quotas[selected.alias].take(selected,0);
                    auto& credit=quotas[selected.alias];
                    if(kind==type::preview_operation)return credit.available(selected,bytes);
                    if(kind==type::preview_bytes)return credit.available_bytes(selected,bytes);
                    return kind==type::bytes?credit.take_bytes(selected,bytes):credit.take(selected,bytes);
                };
                // A previously installed legacy observation/queue has already
                // selected a consuming route, even before its first service call.
                ipc::rmw_service::control preview{};
                bool activating=ipc::rmw_service::request(r.data.data(),r.data.size(),preview)&&preview.operation==ipc::rmw_service::command::activate;
                bool legacy=false;if(activating){legacy=queues.count(r.alias);for(auto const& observing:observations)legacy=legacy||observing.second.source.alias==r.alias;}
                if(!owned_services)result.error=95;
                else if(legacy)result.error=17;
                else result=owned_services->execute(r,*c,revoked,session,started,until,quota);
            }
            // Private retirement is accepted only for a known whole binding
            // in this authenticated session. It bypasses query quota/revocation
            // because local tombstones MUST precede best-effort native cleanup.
            else if(r.operation=="service.available-retire-v1")
            {
                namespace wire=ipc::client_availability;wire::packet request{};
                if(c->operation!="service.available"||!wire::decode_request(r.data.data(),r.data.size(),request)||
                   request.operation!=wire::command::retire||request.binding.backend!=f.backend) { result.error=22; }
                else
                {
                    auto row{availability_history.find(request.binding)};
                    auto source{cfg.policy.capabilities.find(field(*c,"service-source-alias"))};
                    if(!row||row->availability_alias!=r.alias||source==cfg.policy.capabilities.end()||
                       row->source_alias!=source->first||!ros::service_availability_pair(cfg.policy.ros_version,*c,source->second)) { result.error=13; }
                    else
                    {
                        result.error=availability_history.retire(request.binding,request.scope);
                        if(!result.error)
                        {
                            backend->retire_service_availability(request.binding,request.scope,row->availability_alias,row->source_alias);
                            auto raw{wire::encode(request)};result.payload.assign(raw.begin(),raw.end());
                        }
                    }
                }
            }
            // Private same-row teardown may run after alias revocation, but
            // still requires this configured grant, real session and full UMRE.
            else if(r.operation=="rmw.subscription-endpoint-retire-v1")
            {
                namespace wire=ipc::rmw_endpoint;wire::packet request{};
                if(f.backend!=2||c->operation!="topic.subscribe"||!wire::decode(r.data.data(),r.data.size(),false,request)||
                    request.kind!=wire::role::subscription||request.operation!=wire::command::retire||request.owner.session!=session){result.error=22;}
                else
                {
                    result=backend->owned_rmw_subscription(*c,request,until);subscription_endpoint_reply=!result.error;
                    wire::packet actual{};
                    if(!result.error&&!wire::associated_response(result.payload.data(),result.payload.size(),request,actual)){result={13,{}};}
                }
            }
            else if(r.operation=="rmw.endpoint-retire-v1")
            {
                ipc::rmw_endpoint::packet request{};
                if(f.backend!=2||c->operation!="topic.publish"||!ipc::rmw_endpoint::decode(r.data.data(),r.data.size(),false,request)||
                   request.kind!=ipc::rmw_endpoint::role::publisher||request.operation!=ipc::rmw_endpoint::command::retire||request.owner.session!=session)
                {result.error=22;}
                else{result=backend->owned_rmw_publisher(*c,request,until);}
            }
            else if(revoked.count(r.alias)) { result.error=125; }
            else if(r.operation=="rmw.subscription-endpoint-v1")
            {
                namespace wire=ipc::rmw_endpoint;wire::packet request{};
                if(f.backend!=2||c->operation!="topic.subscribe"||!wire::decode(r.data.data(),r.data.size(),false,request)||
                    request.kind!=wire::role::subscription||request.owner.session!=session){result.error=22;}
                else if(request.operation==wire::command::activate&&!activation_quotas[r.alias].take(*c,0)){result.error=11;}
                else if(std::chrono::steady_clock::now()>=until){result.error=110;}
                else
                {
                    native_invoked=request.operation==wire::command::activate;
                    result=backend->owned_rmw_subscription(*c,request,until);subscription_endpoint_reply=!result.error;
                    wire::packet actual{};
                    if(!result.error&&!wire::associated_response(result.payload.data(),result.payload.size(),request,actual)){result={13,{}};}
                }
            }
            else if(r.operation=="rmw.subscription-transfer-v1")
            {
                namespace stream=ipc::rmw_subscription;stream::control request{};
                if(f.backend!=2||c->operation!="topic.subscribe"||!stream::decode(r.data.data(),r.data.size(),false,request)||request.owner.session!=session){result.error=22;}
                else if(std::chrono::steady_clock::now()>=until){result.error=110;}
                else
                {
                    // The request is always96 bytes. A reply can legitimately
                    // occupy all4MiB; do not apply request's -518 wrapper limit.
                    subscription_ack_attempted=request.operation!=stream::command::peek;
                    result=backend->owned_rmw_subscription_transfer(*c,request,until);
                    if(!result.error&&request.operation==stream::command::peek)
                    {
                        subscription_delivery.arm(*c,request);
                        stream::control reply{};ipc::rmw_metadata::header information{};
                        if(!stream::associated_delivery(result.payload.data(),result.payload.size(),request,reply,information)){result={13,{}};}
                        else if(information.payload_bytes>limit(*c,"max-message-bytes")){result={90,{}};}
                        else if(!valid_owned_subscription_body(*c,result.payload.data()+stream::minimum_delivery_bytes,
                            static_cast<std::size_t>(information.payload_bytes))){result={22,{}};}
                        else if(!quotas[r.alias].take(*c,static_cast<std::size_t>(information.payload_bytes))){result={11,{}};}
                    }
                    else if(!result.error)
                    {
                        stream::control ack{};
                        if(!stream::associated_ack(result.payload.data(),result.payload.size(),request,ack)){result={13,{}};}
                    }
                }
            }
            else if(r.operation=="rmw.endpoint-v1")
            {
                ipc::rmw_endpoint::packet request{};
                if(f.backend!=2||c->operation!="topic.publish"||!ipc::rmw_endpoint::decode(r.data.data(),r.data.size(),false,request)||
                   request.kind!=ipc::rmw_endpoint::role::publisher||request.owner.session!=session){result.error=22;}
                else if(request.operation==ipc::rmw_endpoint::command::activate&&!activation_quotas[r.alias].take(*c,0)){result.error=11;}
                else if(std::chrono::steady_clock::now()>=until){result.error=110;}
                else{native_invoked=request.operation==ipc::rmw_endpoint::command::activate;result=backend->owned_rmw_publisher(*c,request,until);}
            }
            else if(r.operation=="rmw.publish-v1")
            {
                ipc::rmw_publish::header request{};
                if(f.backend!=2||c->operation!="topic.publish"||!ipc::rmw_publish::decode(r.data.data(),r.data.size(),request)||request.owner.session!=session){result.error=22;}
                else if(request.payload_bytes>limit(*c,"max-message-bytes")){result.error=90;}
                else if(!quotas[r.alias].take(*c,static_cast<std::size_t>(request.payload_bytes))){result.error=11;}
                else if(std::chrono::steady_clock::now()>=until){result.error=110;}
                else{native_invoked=true;result=backend->owned_rmw_publish(*c,request,r.data.data()+ipc::rmw_publish::header_bytes,static_cast<std::size_t>(request.payload_bytes),until);}
            }
            else if(r.operation=="service.available-v1")
            {
                namespace wire=ipc::client_availability;wire::packet request{};
                if(c->operation!="service.available"||!wire::decode_request(r.data.data(),r.data.size(),request)||
                   request.operation==wire::command::retire||request.binding.backend!=f.backend||request.value>limit(*c,"max-wait-ns")) { result.error=22; }
                else
                {
                    auto source{cfg.policy.capabilities.find(field(*c,"service-source-alias"))};
                    if(source==cfg.policy.capabilities.end()||!ros::service_availability_pair(cfg.policy.ros_version,*c,source->second)) { result.error=13; }
                    else if(revoked.count(source->first)) { result.error=125; }
                    else if(!quotas[r.alias].take(*c,r.data.size())) { result.error=11; }
                    else
                    {
                        ros::service_availability_history::entry const* row{};
                        result.error=availability_history.bind(request.binding,r.alias,source->first,session,static_cast<std::size_t>(limit(*c,"max-binding-history")),row);
                        if(!result.error)
                        {
                            auto budget{remaining_budget(static_cast<std::uint32_t>(limit(*c,"timeout-ms")))};
                            if(!budget) { result.error=110; }
                            else
                            {
                                native_invoked=request.operation==wire::command::query;
                                result=backend->service_availability(*c,source->second,request,budget);
                                wire::packet response{};
                                if(!result.error&&!wire::decode_response(result.payload.data(),result.payload.size(),request,response)) { result={13,{}}; }
                            }
                        }
                    }
                }
            }
            else if(r.operation=="capability.close"||r.operation=="capability.revoke")
            {
                if(!r.data.empty()) { result.error=22; }
                else
                {
                    auto own{observations.find(r.alias)};
                    if(own!=observations.end())
                    {
                        own->second.async->retire();own->second.stream->discard();
                        if(own->second.queue) { own->second.queue->deactivate();backend->close(own->second.source.alias);queues.erase(own->second.source.alias); }
                        observations.erase(own);
                    }
                    for(auto& item:observations)
                    {
                        if(item.second.source.alias!=r.alias) { continue; }
                        item.second.async->retire();item.second.stream->seal(r.operation=="capability.revoke"?125u:0u,true);
                    }
                    auto q{queues.find(r.alias)}; if(q!=queues.end()) { q->second->deactivate(r.operation=="capability.revoke"?125u:0u); queues.erase(q); }
                    for(auto const& item:availability_history.rows())
                    {
                        auto const& row{item.second};using retirement=ipc::client_availability::retirement;
                        auto scope{row.availability_alias==r.alias?(r.operation=="capability.revoke"?retirement::availability_alias:retirement::availability_handle):
                            (r.operation=="capability.revoke"?retirement::source_alias:retirement::source_handle)};
                        if(row.availability_alias!=r.alias&&row.source_alias!=r.alias) { continue; }
                        (void)availability_history.retire(row.binding,scope);
                        backend->retire_service_availability(row.binding,scope,row.availability_alias,row.source_alias);
                    }
                    if(owned_services)owned_services->close_alias(r.alias,r.operation=="capability.revoke");
                    backend->close(r.alias);
                    if(r.operation=="capability.revoke") { revoked.insert(r.alias); }
                }
            }
            else if(r.operation=="event.watch")
            {
                if(c->operation!="event.observe") { result.error=13; }
                else if(r.data.size()!=24||!ipc::get(r.data.data(),8)||!ipc::get(r.data.data()+8,8)) { result.error=22; }
                else if(observations.count(r.alias)) { result.error=17; }
                else
                {
                    auto source{cfg.policy.capabilities.find(field(*c,"event-source-alias"))};
                    if(source==cfg.policy.capabilities.end()||revoked.count(source->first)) { result.error=125; }
                    else if(owned_services&&owned_services->alias_live(source->first)) {result.error=17;}
                    else
                    {
                        auto const& cap{source->second};auto correlation{ipc::get(r.data.data()+16,8)};
                        ipc::event_kind kind{};std::uint8_t flags{};std::uint32_t mask{};ipc::event_overflow overflow{};
                        bool outgoing{cap.operation=="action.receive-feedback"||cap.operation=="action.receive-result"};
                        bool duplicate{};for(auto const& item:observations) { duplicate=duplicate||item.second.source.alias==cap.alias; }
                        if(duplicate) { result.error=17; }
                        else if(cap.operation=="event.observe"||!ipc::parse_event_kinds(field(*c,"event-kinds"),mask)||!ipc::parse_event_overflow(field(*c,"overflow-policy"),overflow)) { result.error=22; }
                        else
                        {
                            bool observable{ipc::event_source_kind(cap.operation,kind,flags)};
                            bool payload_selected{observable&&bool(mask&ipc::event_bit(kind))};
                            auto permitted{ipc::event_bit(ipc::event_kind::resource_error)|ipc::event_bit(ipc::event_kind::resource_closed)|(observable?ipc::event_bit(kind):0)};
                            if(mask&~permitted) { result.error=13; }
                            else if((outgoing&&payload_selected)!=bool(correlation)) {result.error=22;}
                            else if(outgoing&&payload_selected&&action_consumer_claimed(cap,correlation)) {result.error=17;}
                            else if(payload_selected&&queues.count(cap.alias)) {result.error=17;}
                            event_stream_settings settings{limit(*c,"max-event-queue-depth"),limit(*c,"max-event-queue-bytes"),limit(*c,"max-event-bytes"),overflow==ipc::event_overflow::backpressure?limit(*c,"max-pending-events"):0,overflow==ipc::event_overflow::backpressure?limit(*c,"max-pending-event-bytes"):0,overflow};
                            ipc::event_header binding{};binding.backend=f.backend;binding.source_handle=ipc::get(r.data.data(),8);binding.instance=ipc::get(r.data.data()+8,8);binding.session=session;
                            if(!result.error&&!quotas[r.alias].take(*c,0)) {result.error=11;}
                            if(!result.error)
                            {
                                auto stream{std::make_shared<event_stream>(settings,binding,mask)};
                                if(!stream->configured()) {result.error=22;stream->discard();}
                                else
                                {
                                observation selected;selected.source=cap;selected.source_contract=std::make_shared<ros::capability_spec const>(cap);selected.correlation=correlation;selected.stream=stream;
                                if(outgoing&&payload_selected) { result=backend->observe_action(cap,correlation); }
                                else if(payload_selected&&cap.operation!="service.call")
                                {
                                    if(!activation_quotas[cap.alias].take(cap,0)) {result.error=11;}
                                    else
                                    {
                                        auto queue{std::make_shared<event_queue>(cap)};queue->observer=stream;
                                        result=cap.operation=="action.advertise"?backend->advertise_action(cap,queue):cap.operation=="service.advertise"?backend->advertise(cap,queue):backend->subscribe(cap,queue);
                                        if(!result.error) { selected.queue=queue;queues.emplace(cap.alias,std::move(queue)); }
                                    }
                                }
                                if(!result.error) { observations.emplace(r.alias,std::move(selected)); }
                                else { stream->discard(); }
                                }
                            }
                        }
                    }
                }
            }
            else if(r.operation=="event.detach")
            {
                auto selected{observations.find(r.alias)};
                if(c->operation!="event.observe") { result.error=13; }
                else if(!r.data.empty()) { result.error=22; }
                else if(selected==observations.end()) { result.error=9; }
                else
                {
                    auto& observer{selected->second};observer.async->retire();observer.stream->discard();
                    if(observer.queue) { observer.queue->deactivate();backend->close(observer.source.alias);queues.erase(observer.source.alias); }
                    observations.erase(selected);
                }
            }
            else if(r.operation=="event.retire")
            {
                auto selected{observations.find(r.alias)};
                if(c->operation!="event.observe") { result.error=13; }
                else if(selected==observations.end()) { result.error=9; }
                else if(r.data.size()!=4) { result.error=22; }
                else
                {
                    auto& observer{selected->second};observer.async->retire();observer.stream->seal(static_cast<std::uint32_t>(ipc::get(r.data.data(),4)),true);
                    if(observer.queue) { observer.queue->deactivate(static_cast<std::uint32_t>(ipc::get(r.data.data(),4)));backend->close(observer.source.alias);queues.erase(observer.source.alias);observer.queue.reset(); }
                }
            }
            else if(r.operation=="event.poll-v1"||r.operation=="event.peek-v1"||r.operation=="event.commit"||r.operation=="event.ready")
            {
                auto selected{observations.find(r.alias)};
                if(c->operation!="event.observe") { result.error=13; }
                else if(selected==observations.end()) { result.error=9; }
                else if((r.operation=="event.ready"&&!r.data.empty())||(r.operation!="event.ready"&&r.data.size()!=8)) { result.error=22; }
                else
                {
                    auto& observer{selected->second};
                    // Incoming data was debited by its native callback queue.
                    // Only asynchronous service responses and outgoing Action
                    // reads need a separate Source delivery debit here. Every
                    // preview happens before Guest copy; the authenticated IPC
                    // owner cannot interleave another request before commit.
                    auto delivery_admission=[&](ipc::event_header const& header,bool debit)->std::uint32_t
                    {
                        auto wire{static_cast<std::size_t>(ipc::event_header_bytes+header.payload_bytes)};
                        bool service{header.kind==ipc::event_kind::service_response};
                        bool action{header.kind==ipc::event_kind::action_feedback||header.kind==ipc::event_kind::action_result};
                        auto body{static_cast<std::size_t>(header.payload_bytes)};
                        if(!quotas[r.alias].available(*c,wire)) {return 11;}
                        if(service&&!quotas[observer.source.alias].available_bytes(observer.source,body)) {return 11;}
                        if(action&&!quotas[observer.source.alias].available(observer.source,body)) {return 11;}
                        if(debit)
                        {
                            // No debit is attempted until BOTH previews pass.
                            // These buckets have one native request owner and
                            // refresh can only replenish credit between calls.
                            if(service) {(void)quotas[observer.source.alias].take_bytes(observer.source,body);}
                            if(action) {(void)quotas[observer.source.alias].take(observer.source,body);}
                            (void)quotas[r.alias].take(*c,wire);
                        }
                        return 0;
                    };
                    // Action adapters retain their own bounded native queue. A
                    // read is attempted only with room for the maximum record;
                    // native terminal consumption then transfers into a durable
                    // reserved slot, never an overflow-droppable vector.
                    ipc::event_kind payload_kind{};std::uint8_t payload_flags{};
                    bool selected_action{observer.correlation&&ipc::event_source_kind(observer.source.operation,payload_kind,payload_flags)&&bool(observer.stream->allowed_kinds()&ipc::event_bit(payload_kind))};
                    if(r.operation!="event.commit"&&selected_action&&observer.stream->active())
                    {
                        auto response_bytes{ipc::event_header_bytes+limit(observer.source,observer.source.operation=="action.receive-feedback"?"max-feedback-bytes":"max-result-bytes")};
                        auto readiness{observer.stream->readiness()};
                        if(readiness.admission_slots&&readiness.admission_bytes>=response_bytes&&response_bytes<=limit(*c,"max-event-bytes"))
                        {
                            auto id{observer.correlation};auto reserved{observer.stream->reserve(id,response_bytes,0)};
                            if(!reserved)
                            {
                                std::vector<std::uint8_t> argument;ipc::put(argument,id,8);
                                auto answer{backend->operation(observer.source,argument,remaining_budget(cfg.timeout_ms))};
                                if(answer.error==11) { observer.stream->cancel_reservation(id); }
                                else
                                {
                                    ipc::event_kind kind{};std::uint8_t flags{};(void)ipc::event_source_kind(observer.source.operation,kind,flags);
                                    auto event{answer.error?ipc::event_record{ipc::event_kind::resource_error,ipc::event_external_effects_may_remain,answer.error,id,{}}:ipc::event_record{kind,flags,0,id,std::move(answer.payload)}};
                                    auto completion_error{observer.stream->complete(id,std::move(event))};
                                    // An unselected error bit is not permission
                                    // to synthesize an error record. Preserve
                                    // the actual receive errno instead of
                                    // silently presenting an idle queue forever.
                                    if(completion_error) {result.error=answer.error?answer.error:completion_error;}
                                }
                            }
                        }
                    }
                    if(result.error) { /* Non-consuming native refusal above. */ }
                    else if(r.operation=="event.commit")
                    {
                        result.error=observer.stream->commit_if(ipc::get(r.data.data(),8),[&](ipc::event_header const& header){return delivery_admission(header,true);});
                    }
                    else if(r.operation=="event.peek-v1") {result=observer.stream->peek_if(ipc::get(r.data.data(),8),[&](ipc::event_header const& header){return delivery_admission(header,false);});}
                    else if(r.operation=="event.poll-v1")
                    {
                        result=observer.stream->peek_if(ipc::get(r.data.data(),8),[&](ipc::event_header const& header){return delivery_admission(header,false);});
                        if(!result.error) {ipc::event_header header{};if(!ipc::decode_event(result.payload.data(),result.payload.size(),header)) {result={22,{}};}else {auto error{observer.stream->commit_if(header.sequence,[&](ipc::event_header const& h){return delivery_admission(h,true);})};if(error) {result={error,{}};}}}
                    }
                    else if(!quotas[r.alias].take(*c,0)) {result.error=11;}else { auto ready{ipc::encode_event_readiness(observer.stream->readiness())};result.payload.assign(ready.begin(),ready.end()); }
                }
            }
            else if(r.operation=="service.submit")
            {
                auto selected{observations.find(r.alias)};
                if(c->operation!="event.observe") { result.error=13; }
                else if(selected==observations.end()) { result.error=9; }
                else
                {
                    auto& observer{selected->second};auto const& source{observer.source};
                    auto mask{observer.stream->allowed_kinds()};
                    if(source.operation!="service.call"||!(mask&ipc::event_bit(ipc::event_kind::service_response))||!(mask&ipc::event_bit(ipc::event_kind::resource_error))) { result.error=13; }
                    else if(revoked.count(source.alias)||!observer.stream->active()) { result.error=125; }
                    else if(r.data.size()<8||!ipc::get(r.data.data(),8)) { result.error=22; }
                    else
                    {
                        auto id{ipc::get(r.data.data(),8)};
                        auto maximum{ipc::event_header_bytes+limit(source,"max-response-bytes")};
                        if(r.data.size()-8>limit(source,"max-request-bytes")||maximum>limit(*c,"max-event-bytes")) { result.error=90; }
                        else
                        {
                            std::vector<std::uint8_t> input{r.data.begin()+8,r.data.end()};
                            if(!validate_payload(source,input)) {result.error=22;}
                            else if(!quotas[source.alias].take(source,input.size())) {result.error=11;}
                            else
                            {
                            result.error=observer.stream->reserve(id,maximum,input.capacity()+async_service_pool::accounting_bytes);
                            if(!result.error)
                            {
                                auto deadline{started+std::chrono::milliseconds(limit(source,"timeout-ms"))};
                                auto native{backend};auto contract{observer.source_contract};
                                async_service_pool::task work{observer.async,observer.stream,id,deadline,std::move(input),[native,contract](auto const& bytes,std::uint32_t timeout)
                                {
                                    auto const& cap{*contract};
                                    auto response{native->service(cap,bytes,timeout)};
                                    if(!response.error)
                                    {
                                        bool valid{};for(unsigned version:{1u,2u}) { if(auto item=ipc::find_custom_service(version,cap.type)) { valid=ipc::validate_custom_service_output(*item,cap.operation,response.payload.data(),response.payload.size()); } }
                                        if(cap.type=="std_srvs/Trigger"||cap.type=="std_srvs/SetBool"||cap.type=="std_srvs/srv/Trigger"||cap.type=="std_srvs/srv/SetBool") { valid=response.payload.size()>=5&&response.payload[0]<=1&&ipc::get(response.payload.data()+1,4)==response.payload.size()-5&&valid_utf8(response.payload.data()+5,response.payload.size()-5); }
                                        if(!valid||response.payload.size()>limit(cap,"max-response-bytes")) { response={22,{}}; }
                                    }
                                    return response;
                                }};
                                result.error=service_tasks.submit(std::move(work),limit(source,"max-inflight"));
                                if(result.error) { observer.stream->cancel_reservation(id); }
                            }
                            }
                        }
                    }
                }
            }
            else if(r.operation=="event.poll")
            {
                if((c->operation!="topic.subscribe"&&c->operation!="service.advertise"&&c->operation!="param.subscribe-events"&&c->operation!="action.advertise")||!r.data.empty()) { result.error=13; }
                else { auto q{queues.find(r.alias)}; result=q==queues.end()?ipc::call_result{9,{}}:q->second->poll(); }
            }
            else if(r.operation!=c->operation) { result.error=13; }
            // A data watch owns the native goal's receive queue across aliases.
            // Deny BEFORE Source quota debit and before the adapter can consume
            // feedback or retire a terminal result. Metadata-only watches own
            // no native consumer and therefore do not enter this branch.
            else if((r.operation=="action.receive-feedback"||r.operation=="action.receive-result")&&
                    r.data.size()==8&&action_consumer_claimed(*c,ipc::get(r.data.data(),8))) {result.error=17;}
            else if(r.operation=="topic.subscribe"||r.operation=="service.advertise"||r.operation=="param.subscribe-events"||r.operation=="action.advertise")
            {
                if(!r.data.empty()) { result.error=22; }
                else if(!queues.count(r.alias))
                {
                    if(!activation_quotas[r.alias].take(*c,0)) {result.error=11;}
                    else {auto q{std::make_shared<event_queue>(*c)};native_invoked=true; result=r.operation=="action.advertise"?backend->advertise_action(*c,q):r.operation!="service.advertise"?backend->subscribe(*c,q):backend->advertise(*c,q);
                    if(!result.error) { queues.emplace(r.alias,std::move(q)); }}
                }
            }
            else if(r.operation=="topic.publish"||r.operation=="service.call")
            {
                auto max{limit(*c,r.operation=="topic.publish"?"max-message-bytes":"max-request-bytes")};
                if(r.data.size()>max) { result.error=90; }
                else if(!validate_payload(*c,r.data)) { result.error=22; }
                else if(!quotas[r.alias].take(*c,r.data.size())) { result.error=11; }
                else if(std::chrono::steady_clock::now()>=until) { result.error=110; }
                else if(r.operation=="topic.publish") {native_invoked=true; result=backend->publish(*c,r.data); }
                else { auto budget{remaining_budget(static_cast<std::uint32_t>(limit(*c,"timeout-ms")))};if(budget) {native_invoked=true;result=backend->service(*c,r.data,budget);}else {result={110,{}};} }
            }
            else
            {
                bool valid{}; std::uint64_t maximum{};std::uint32_t typed_error{};
                if(r.operation.rfind("time.",0)==0) { valid=r.data.empty(); maximum=0; }
                else if(r.operation=="param.set"||r.operation=="param.declare")
                {
                    maximum=limit(*c,"max-parameter-bytes");
                    typed_error=ipc::validate_parameter_value(cfg.policy.ros_version,*c,r.data.data(),r.data.size());valid=!typed_error;
                    if(valid&&c->type=="builtin/string-v1") { for(auto b=r.data.begin()+4;b!=r.data.end();++b) { if(!*b) { valid=false;break; } } }
                }
                else if(r.operation=="log.write")
                {
                    maximum=limit(*c,r.operation=="log.write"?"max-message-bytes":"max-parameter-bytes");
                    valid=r.data.size()>=4&&ipc::get(r.data.data(),4)==r.data.size()-4&&valid_utf8(r.data.data()+4,r.data.size()-4);
                    if(valid) { for(auto b=r.data.begin()+4;b!=r.data.end();++b) { if(!*b) { valid=false; break; } } }
                }
                else if(r.operation=="service.respond")
                {
                    maximum=limit(*c,"max-response-bytes")+8;
                    if(auto item=ipc::find_custom_service(static_cast<unsigned>(cfg.policy.ros_version),c->type)) { valid=ipc::validate_custom_service_request(*item,c->operation,r.data.data(),r.data.size()); }
                    else { valid=r.data.size()>=13&&ipc::get(r.data.data(),8)!=0&&r.data[8]<=1&&ipc::get(r.data.data()+9,4)==r.data.size()-13&&valid_utf8(r.data.data()+13,r.data.size()-13); }
                }
                else if(r.operation.rfind("action.",0)==0)
                {
                    if(r.operation=="action.send-goal") { maximum=limit(*c,"max-goal-bytes");if(auto item=ipc::find_custom_action(static_cast<unsigned>(cfg.policy.ros_version),c->type)) {valid=ipc::validate_custom_action_request(*item,c->operation,r.data.data(),r.data.size());}else {std::int32_t order{};valid=ipc::fibonacci_order(r.data,order)&&static_cast<std::uint64_t>(order)<=limit(*c,"fibonacci.max-order");} }
                    else if(r.operation=="action.respond") {if(auto item=ipc::find_custom_action(static_cast<unsigned>(cfg.policy.ros_version),c->type)) {valid=ipc::validate_custom_action_request(*item,c->operation,r.data.data(),r.data.size());maximum=limit(*c,valid&&r.data[8]==1?"max-feedback-bytes":"max-result-bytes");}else {valid=ipc::validate_action_sequence(r.data.data(),r.data.size(),true);maximum=limit(*c,valid&&r.data[8]==1?"max-feedback-bytes":"max-result-bytes");} }
                    else { maximum=8;valid=r.data.size()==8&&(r.operation=="action.cancel-any-goal"||ipc::get(r.data.data(),8)!=0); }
                }
                else if(r.operation=="tf.lookup") { maximum=8;valid=r.data.size()==8; }
                else if(r.operation=="tf.broadcast") { maximum=limit(*c,"max-message-bytes");ipc::transform_data t;valid=ipc::decode_transform(r.data,t); }
                else if(r.operation.rfind("param.",0)==0||r.operation.rfind("graph.",0)==0||r.operation.rfind("lifecycle.",0)==0) { valid=r.data.empty(); maximum=0; }
                if(r.data.size()>maximum) { result.error=90; }
                else if(typed_error) { result.error=typed_error; }
                else if(!valid) { result.error=22; }
                else if(!quotas[r.alias].take(*c,r.data.size())) { result.error=11; }
                else if(std::chrono::steady_clock::now()>=until) { result.error=110; }
                else { auto maximum{std::min(f.timeout_ms,cfg.timeout_ms)};if(c->limits.count("timeout-ms")) { maximum=std::min(maximum,static_cast<std::uint32_t>(limit(*c,"timeout-ms"))); }auto budget{remaining_budget(maximum)};if(budget) {native_invoked=true;result=backend->operation(*c,r.data,budget);}else {result={110,{}};} }
                if(!result.error)
                {
                    auto response_max{c->operation.rfind("action.",0)==0?(c->operation=="action.send-goal"?8:c->operation=="action.receive-feedback"?limit(*c,"max-feedback-bytes"):c->operation=="action.receive-result"?limit(*c,"max-result-bytes"):c->operation.rfind("action.cancel-",0)==0?1:0):c->limits.count("max-response-bytes")?limit(*c,"max-response-bytes"):c->limits.count("max-parameter-bytes")?limit(*c,"max-parameter-bytes"):c->limits.count("max-graph-result-bytes")?limit(*c,"max-graph-result-bytes"):c->operation=="tf.lookup"?limit(*c,"max-message-bytes"):std::uint64_t{0}};
                    if(result.payload.size()>response_max) { result={90,{}}; }
                    else if(r.operation=="param.get")
                    { auto error{ipc::validate_parameter_value(cfg.policy.ros_version,*c,result.payload.data(),result.payload.size())};if(error) { result={error,{}}; } }
                }
            }
            if(std::chrono::steady_clock::now()>=until) { result={110,{}}; }
            if(c&&!result.error&&!result.payload.empty()&&r.operation!="event.poll"&&r.operation!="event.poll-v1"&&r.operation!="event.peek-v1"&&r.operation!="service.available-retire-v1"&&r.operation!="rmw.endpoint-v1"&&r.operation!="rmw.endpoint-retire-v1"&&r.operation!="rmw.subscription-endpoint-v1"&&r.operation!="rmw.subscription-endpoint-retire-v1"&&r.operation!="rmw.subscription-transfer-v1"&&!owned_service_operation(r.operation)&&!quotas[r.alias].take_bytes(*c,result.payload.size())) { result={11,{}}; }
            // Quota/validation/deadline failure after peek explicitly cancels
            // THIS borrow before returning. Preserve the primary error even if
            // cancel fails; registry then tombstones/retires and records both.
            if(result.error){subscription_delivery.fail(result.error);}
            if(result.error&&owned_services)owned_services->delivery_failed(result.error);
            // Explicitly selected metadata is independent of payload selection.
            // One preallocated control slot per stream coalesces failures; no
            // native callback blocks, and no unknown body is copied into it.
            // EAGAIN denotes ordinary credit/readiness rather than a fault.
            if(c&&(r.operation==c->operation||(r.operation=="service.available-v1"&&c->operation=="service.available"))&&result.error&&result.error!=11)
            {
                for(auto& item:observations)
                {
                    if(item.second.source.alias!=r.alias||!item.second.stream->active()) {continue;}
                    (void)item.second.stream->enqueue({ipc::event_kind::resource_error,static_cast<std::uint8_t>(native_invoked?ipc::event_external_effects_may_remain:0),result.error,0,{}});
                }
            }
            audit(session,r,c,result.error,log_limit,c?grant_logs[r.alias]:log_limit,cfg);
            answer.response_id=f.request_id; answer.error=result.error; answer.payload=std::move(result.payload);
            // A capability deadline limits the operation. Its timeout result
            // can still be delivered within the caller's original IPC budget.
            if(!ipc::write_frame(fd,answer,wire_until,worker_pidfd))
            {subscription_delivery.fail(errno==ETIMEDOUT?110u:107u);if(owned_services)owned_services->delivery_failed(errno==ETIMEDOUT?110u:107u);break;}
            auto late_subscription_reply{std::chrono::steady_clock::now()>=until};
            if(late_subscription_reply){subscription_delivery.fail(110);}
            else{subscription_delivery.delivered();}
            // An ACK may already have committed/cancelled. A failed/late ACK
            // is ambiguous, never permission to replay or continue the session.
            // Likewise a successful endpoint response lost to a final error
            // must not strand a newly created endpoint without its receipt.
            if(owned_service_operation(r.operation)&&owned_services)
            {
                if(late_subscription_reply)owned_services->delivery_failed(110);else owned_services->delivered();
                if(late_subscription_reply||owned_services->close_session())break;
            }
            if((subscription_ack_attempted&&(answer.error||late_subscription_reply))||
                (subscription_endpoint_reply&&(answer.error||late_subscription_reply))||
                (r.operation=="rmw.subscription-transfer-v1"&&late_subscription_reply)){break;}
        }
        for(auto& item:observations) { item.second.async->retire();item.second.stream->discard(); }
        for(auto const& q:queues) { q.second->deactivate(); }
        availability_history.seal();
        if(owned_services)owned_services->clear();
        owned_rmw_guard.clear();
        availability_guard.clear();
        service_tasks.stop();
    }
    inline int run(configuration const& cfg,std::function<std::unique_ptr<adapter>()> factory)
    {
int worker_pidfd{-1};
#if defined(SYS_pidfd_open)
        worker_pidfd=static_cast<int>(::syscall(SYS_pidfd_open,cfg.worker_pid,0));
        if(worker_pidfd<0) { throw std::runtime_error("cannot pin trusted worker process lifetime with pidfd"); }
        struct worker_guard { int fd;~worker_guard(){::close(fd);} } lifetime{worker_pidfd};
#else
        throw std::runtime_error("UNSUPPORTED worker process lifetime binding without pidfd_open");
#endif
        auto slash{cfg.socket.find_last_of('/')}; auto parent{cfg.socket.substr(0,slash)};
        struct stat st{};
        if(parent.empty()||::lstat(parent.c_str(),&st)<0||!S_ISDIR(st.st_mode)||st.st_uid!=::geteuid()||(st.st_mode&0077)) { throw std::runtime_error("socket parent must be owned mode 0700 directory"); }
        if(::lstat(cfg.socket.c_str(),&st)==0||errno!=ENOENT) { throw std::runtime_error("socket path already exists"); }
        sockaddr_un a{}; a.sun_family=AF_UNIX;
        if(cfg.socket.size()>=sizeof a.sun_path) { throw std::runtime_error("socket path too long"); }
        std::memcpy(a.sun_path,cfg.socket.c_str(),cfg.socket.size()+1);
        int fd{::socket(AF_UNIX,SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK,0)};
        if(fd<0) { throw std::runtime_error("socket"); }
        struct listener_guard { int fd; std::string path; ~listener_guard() { ::close(fd); ::unlink(path.c_str()); } } guard{fd,cfg.socket};
        mode_t old{::umask(0077)}; int bound{::bind(fd,reinterpret_cast<sockaddr*>(&a),sizeof a)}; ::umask(old);
        if(bound<0||::chmod(cfg.socket.c_str(),0600)<0||::listen(fd,static_cast<int>(cfg.max_sessions))<0) { throw std::runtime_error("bind/listen"); }
        std::signal(SIGINT,stop); std::signal(SIGTERM,stop);
        struct worker { std::thread thread; std::shared_ptr<std::atomic<bool>> done; };
        std::vector<worker> workers;
        // Allocate the finite configured table BEFORE any session thread can
        // start. push_back cannot allocate after constructing a joinable thread.
        workers.reserve(cfg.max_sessions);
        while(!stopping.load(std::memory_order_relaxed))
        {
            for(auto i=workers.begin();i!=workers.end();) { if(i->done->load()) { i->thread.join(); i=workers.erase(i); } else { ++i; } }
            pollfd p[2]{{fd,POLLIN,0},{worker_pidfd,POLLIN,0}}; int ready{::poll(p,2,100)}; if(ready<=0) { continue; }
            if(p[1].revents) { stopping.store(1,std::memory_order_relaxed);break; }
            if(!(p[0].revents&POLLIN)) { continue; }
            int peer{::accept4(fd,nullptr,nullptr,SOCK_CLOEXEC|SOCK_NONBLOCK)}; if(peer<0) { continue; }
            if(workers.size()>=cfg.max_sessions) { ::close(peer); continue; }
            auto done{std::make_shared<std::atomic<bool>>(false)};
            workers.push_back({std::thread{[peer,&cfg,&factory,done,worker_pidfd] { try { serve_connection(peer,cfg,factory,worker_pidfd); } catch(std::exception const& e) { native_io::diagnostic("session closed",e.what()); } done->store(true); }},done});
        }
        for(auto& w:workers) { w.thread.join(); } return 0;
    }
}
