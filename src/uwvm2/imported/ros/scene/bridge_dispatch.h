/** @file
 * @brief Directed typed topic forwarding with retained receive reservations.
 * @details A topic-forwarder binds one subscription to one publisher. It copies
 * only a compiled canonical String/Bool schema between independently authorized
 * endpoints; their native ROS adapters perform serialization. No guest address,
 * socket, library, topic name or native descriptor selects an endpoint here.
 * Each source remains reserved until the target returns its native acceptance
 * receipt and source acknowledgement completes. Acceptance is not proof of
 * remote delivery. Unknown outcomes latch the route with its original record;
 * they are never retried under a new key. Inspect only reads cached counters.
 *
 * One deployment-owned finite supervisor pumps nonblocking communication
 * adapters. Native peers retain their own isolated process/CPU/NUMA settings;
 * this coordinator inherits the VM's placement, has an explicitly reserved
 * guarded fast_io stack and finite poll interval. It never migrates cgroups.
 * Stop has a separate recovery sequence and storage, independent of traffic
 * rates. Shutdown joins the supervisor before any referenced provider dies.
 */
#pragma once
#include "communication_host.h"
#include "communication_codecs.h"
#include "robot_provider_response.h"
#include "proposal_policy.h"
#include "supervisor_stack.h"
namespace uwvm2::imported::ros::scene
{
    class bridge_dispatch
    {
    public:
        using operation=robot_contract::operation;using bytes=wire::bytes;
        using sink=communication_host::sink;using cancellation=communication_host::cancellation;
        /** @brief Trusted typed access to the same admitted communication carrier.
         * @details Production binds communication_host. Core qualification can
         * bind a communication_dispatch with independently exercised native
         * providers. These callbacks are never configured by a guest or file.
         */
        struct transport
        {
            void* context{};
            error(*invoke)(void*,caller,communication_contract::operation,handle,bytes,std::size_t,std::size_t&,sink){};
            error(*receive)(void*,caller,communication_receive_contract::operation,handle,bytes,std::size_t,std::size_t&,sink){};
            error(*quiescent)(void*,caller,handle,handle,bool&){};
            template<class Host>static transport from(Host& host)noexcept
            {return {&host,[](void* p,caller who,auto action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver)
                    {return static_cast<Host*>(p)->invoke(who,action,cap,input,capacity,required,deliver);},
                [](void* p,caller who,auto action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver)
                    {return static_cast<Host*>(p)->invoke_receive(who,action,cap,input,capacity,required,deliver);},
                [](void* p,caller who,handle cap,handle id,bool& output)
                    {return static_cast<Host*>(p)->topic_receive_quiescent(who,cap,id,output);}};}
        };
        struct plan
        {
            buffers::text<255> alias,source_alias,destination_alias;
            buffers::profile selected,stop,source_profile,destination_profile;
            buffers::text<32> codec;
            std::uint64_t message_bytes{},storage_bytes{},deadline_ns{};
        };
        struct limits {std::uint64_t bindings{},storage_bytes{},supervisor_bytes{},poll_ns{};};
        struct inspection {std::uint64_t handles{},pending{},unknown{},forwarded{},losses{},reserved_bytes{};bool active{},retired{},busy{};};
    private:
        enum class phase:std::uint8_t {empty,receiving,reserved,publishing,acknowledging,releasing,uncertain};
        struct binding
        {
            plan policy;mutable std::mutex mutex;
            ::uwvm2::utils::container::std_vector<std::uint8_t> received,transmit;
            caller owner{};handle cap{},id{},generation{},normal_key{},stop_key{},publish_key{},settle_key{};
            robot_contract::reference source,destination;
            buffers::digest normal_digest{},stop_digest{};robot_contract::receipt started,stopped;
            communication_receive_contract::reference retained;
            std::size_t received_bytes{},transmit_bytes{};
            std::uint64_t until{},stop_until{},forwarded{},losses{};
            phase current{};bool published{},active{},stopping{},retired{},busy{},attempted{},frame_charged{};
        };
        std::shared_ptr<authority> permissions_;transport communication_;
        handle generation_{};limits capacity_;mutable std::mutex configuration_,join_;
        ::uwvm2::utils::container::std_vector<std::shared_ptr<binding>> bindings_;
        std::uint64_t storage_{};std::atomic<bool> prepared_{},stopped_{};
#if defined(__linux__)
        pid_t process_{::getpid()};model_worker::supervisor_stack stack_;pthread_t thread_{};bool launched_{},joined_{};
#endif
        bool same_process()const noexcept
        {
#if defined(__linux__)
            return process_==::getpid();
#else
            return true;
#endif
        }
        static std::uint64_t now()noexcept{return static_cast<std::uint64_t>(resources::steady_now());}
        static bool accepts(operation action)noexcept{return action>=operation::bridge_create&&action<=operation::bridge_describe;}
        static std::string_view name(operation action)noexcept{return robot_contract::operations[static_cast<std::size_t>(action)].name;}
        static bool match(robot_contract::profile const& from,buffers::profile const& to)noexcept
        {return from.identity==to.identity.view()&&from.digest.size()==32&&std::equal(from.digest.begin(),from.digest.end(),to.sha256.begin());}
        static bool profile(wire::writer& out,buffers::profile const& value)noexcept{return out.text(value.identity.view())&&out.blob(value.sha256);}
        static bool receipt(wire::writer& out,robot_contract::receipt const& value)noexcept
        {return out.u64(value.request)&&out.text(value.state)&&out.text(value.effect)&&out.u64(value.provider_generation)&&out.text(value.reason);}
        static bool same(caller left,caller right)noexcept{return left.instance==right.instance&&left.generation==right.generation;}
        static void increment(std::uint64_t& value)noexcept{if(value!=UINT64_MAX)++value;}
        std::shared_ptr<binding> find(std::string_view alias)const noexcept
        {for(auto const& row:bindings_)if(row->policy.alias.view()==alias) { return row; }return {};}
        error select(caller who,operation action,handle cap,std::shared_ptr<binding>& output)
        {
            if(!same_process()||!permissions_||!accepts(action)||!prepared_.load()||stopped_.load()) { return error::denied; }
            lease allowed;auto status=permissions_->admit(who,cap,name(action),0,allowed,false);if(status!=error::success) { return status; }
            auto const& grant=allowed.specification();if(grant.kind!=purpose::bridge||grant.binding!="topic-forwarder") { return error::denied; }
            std::lock_guard lock{configuration_};output=find(grant.alias);return output?error::success:error::denied;
        }
        /** @brief Validate both exact directional grants together with the owning bridge.
         * @details Host topic adapters additionally fence the original node,
         * context and type origins. No source lease implies destination authority.
         * Only the primary bridge work is charged here; native adapters account
         * their own complete operations. Partial budget exhaustion retains the
         * reservation and exposes uncertainty instead of silently dropping data.
         */
        error group(caller who,handle cap,operation action,binding const& row,robot_contract::reference source,
                    robot_contract::reference destination,group_lease& held,std::uint64_t bytes=0,bool charge=false)
        {
            std::array claims{
                admission_claim{cap,name(action),bytes,false,false,action==operation::bridge_stop},
                admission_claim{source.capability,"topic.reserve",0,false,true},
                admission_claim{source.capability,"topic.acknowledge",0,false,true},
                admission_claim{source.capability,"topic.release",0,false,true},
                admission_claim{source.capability,"topic.describe",0,false,true},
                admission_claim{source.capability,"data.export",0,true,true},
                admission_claim{destination.capability,"topic.publish",0,false,true},
                admission_claim{destination.capability,"topic.describe",0,false,true}};
            auto status=permissions_->admit_group(who,claims,held,charge);if(status!=error::success) { return status; }
            auto const* from=held.specification(1);auto const* to=held.specification(6);auto const* primary=held.specification(0);
            if(!primary||primary->kind!=purpose::bridge||primary->binding!="topic-forwarder"||primary->alias!=row.policy.alias.view()||
                !from||from->kind!=purpose::topic||from->binding!="native-communication"||from->alias!=row.policy.source_alias.view()||
                !to||to->kind!=purpose::topic||to->binding!="native-communication"||to->alias!=row.policy.destination_alias.view())return error::denied;
            return !charge||held.consume(who,claims)?error::success:error::denied;
        }
        error endpoint(caller who,robot_contract::reference selected,buffers::profile const& profile,bool subscription,
                       communication::codecs::descriptor const& codec)
        {
            std::array<std::uint8_t,8> input;wire::writer request{input};request.u64(selected.resource);std::size_t required{};bool correct{};
            auto status=communication_.invoke(communication_.context,who,communication_contract::operation::topic_describe,selected.capability,request.written(),2048,required,[&](bytes bytes)
            {
                wire::reader in{bytes};std::uint64_t node{},generation{};communication_contract::profile_view actual;
                std::string_view name,type,role;bool present{};wire::bytes gid;
                correct=in.u64(node)&&node&&communication_contract::read_profile(in,actual)&&actual.identity==profile.identity.view()&&
                    actual.digest.size()==32&&std::equal(actual.digest.begin(),actual.digest.end(),profile.sha256.begin())&&
                    in.text(name)&&communication::absolute_name(name)&&in.text(type)&&type==codec.identity&&in.text(role)&&
                    role==(subscription?"subscription":"publisher")&&in.boolean(present)&&(!present||(in.u64(generation)&&generation))&&
                    in.boolean(present)&&(!present||(in.blob(gid)&&!gid.empty()))&&in.done();return correct;
            });return status==error::success&&!correct?error::denied:status;
        }
        static std::string_view state(binding const& row)noexcept
        {return row.retired?"retired":row.current==phase::uncertain?"outcome_unknown":row.stopping?"draining":row.active?"running":row.stopped.request?"stopped":"prepared";}
        static bool describe(wire::writer& out,binding const& row)noexcept
        {
            bool pending=row.current!=phase::empty;
            auto const& last=row.stopped.request>row.started.request?row.stopped:row.started;
            return profile(out,row.policy.selected)&&out.text(state(row))&&out.u64(row.generation)&&out.u64(1)&&
                out.u64(pending?1:0)&&out.u64(row.losses)&&out.u64(row.current==phase::uncertain?1:0)&&out.boolean(bool(last.request))&&(!last.request||receipt(out,last));
        }
        static void unknown(binding& row,std::string_view why)noexcept
        {
            row.current=phase::uncertain;row.active=false;
            if(row.stopped.request){row.stopped.state="outcome_unknown";row.stopped.effect="unknown";row.stopped.reason=why;}
        }
        static void settled(binding& row)noexcept
        {
            row.current=phase::empty;row.retained={};row.received_bytes=row.transmit_bytes=0;row.attempted=row.frame_charged=false;
            if(row.stopping){row.active=false;row.stopping=false;row.stopped.state="succeeded";row.stopped.effect="committed";row.stopped.reason="";}
        }
        /** @brief Perform at most one native step; caller holds the exact authority cohort before the row lock. */
        error step(binding& row)
        {
            auto time=now();auto codec=communication::codecs::find(row.policy.codec.view());if(!codec) { return error::invalid; }
            if(row.stopping&&time>=row.stop_until&&row.current!=phase::empty){unknown(row,"stop.deadline");return error::timeout;}
            if(row.current==phase::uncertain) { return error::conflict; }
            if(row.current==phase::empty||row.current==phase::receiving)
            {
                if(row.stopping&&row.current==phase::empty){settled(row);return error::success;}
                if(!row.active&&row.current==phase::empty) { return error::again; }
                if(!row.until){if(time>INT64_MAX-row.policy.deadline_ns) { return error::too_large; }row.until=time+row.policy.deadline_ns;}
                std::array<std::uint8_t,16> bytes;wire::writer request{bytes};request.u64(row.source.resource);request.u64(row.until);std::size_t required{};
                row.current=phase::receiving;
                auto status=communication_.receive(communication_.context,row.owner,communication_receive_contract::operation::topic_reserve,row.source.capability,
                    request.written(),row.received.size(),required,[&](wire::bytes bytes)
                    {if(bytes.size()>row.received.size()) { return false; }std::copy(bytes.begin(),bytes.end(),row.received.begin());row.received_bytes=bytes.size();return true;});
                if(status==error::again||status==error::timeout)
                {
                    // AGAIN can mean an in-flight RPC or a completed empty
                    // receive. Only verified zero debt permits an idle/stop
                    // transition; a timeout alone never discards a reservation.
                    bool empty{};auto inspected=communication_.quiescent(communication_.context,row.owner,
                        row.source.capability,row.source.resource,empty);
                    if(inspected==error::success&&empty){row.until=0;settled(row);return status;}
                    if(status==error::timeout)unknown(row,"reservation.deadline");
                    return inspected==error::success||inspected==error::again?status:inspected;
                }
                if(status!=error::success){unknown(row,"reservation.unresolved");return status;}
                // bind() checked that both complete buffers fit in the host address space.
                auto const blob_limit=std::max<std::size_t>(static_cast<std::size_t>(row.policy.message_bytes),256);
                robot_contract::response_decoder in{{row.received.data(),row.received_bytes},{row.received.size(),blob_limit,255,0}};
                robot_contract::metadata metadata;std::uint64_t deadline{},correlation{};std::string_view kind;robot_contract::message message;
                bool valid=in.identity(row.retained.endpoint)&&in.identity(row.retained.provider_generation)&&in.identity(row.retained.sequence)&&
                    in.identity(row.retained.nonce)&&in.deadline(deadline)&&deadline==row.until&&in.origin(metadata)&&in.number(correlation)&&!correlation&&
                    in.text(kind)&&kind=="topic.message"&&in.body(message)&&in.done()&&metadata.source==row.source.resource&&
                    row.retained.endpoint==row.source.resource&&metadata.sequence==row.retained.sequence&&message.type==codec->identity&&message.encoding==codec->encoding&&
                    message.body.size()<=row.policy.message_bytes&&codec->validate(message.body);
                if(!valid){unknown(row,"source.invalid");return error::invalid;}
                row.losses=metadata.loss_count>UINT64_MAX-row.losses?UINT64_MAX:row.losses+metadata.loss_count;
                if(row.publish_key==UINT64_MAX||row.settle_key==UINT64_MAX){unknown(row,"sequence.exhausted");return error::too_large;}
                wire::writer output{row.transmit};
                if(!output.u64(row.destination.resource)||!output.u64(row.publish_key+1)||!output.u64(row.destination.resource)||
                    !communication_contract::encode_message(output,{message.type,message.encoding,message.body})||!output.u64(row.until))
                {unknown(row,"record.capacity");return error::too_large;}
                row.transmit_bytes=output.size();row.current=phase::reserved;return error::success;
            }
            if(row.current==phase::reserved&&row.stopping)row.current=phase::releasing;
            if(row.current==phase::reserved||row.current==phase::publishing)
            {
                if(time>=row.until)
                {if(row.attempted){unknown(row,"publish.deadline");return error::timeout;}row.current=phase::releasing;}
                else
                {
                    if(!row.frame_charged)
                    {
                        group_lease charged;auto status=group(row.owner,row.cap,operation::bridge_start,row,row.source,row.destination,charged,row.transmit_bytes,true);
                        if(status!=error::success) { return status; }row.frame_charged=true;
                    }
                    row.attempted=true;row.current=phase::publishing;std::size_t required{};bool valid{},uncertain{},accepted{},none{};
                    auto status=communication_.invoke(communication_.context,row.owner,communication_contract::operation::topic_publish,row.destination.capability,
                        {row.transmit.data(),row.transmit_bytes},256,required,[&](wire::bytes bytes)
                        {robot_contract::receipt result;robot_contract::response_decoder in{bytes,{256,0,255,0}};
                            valid=in.outcome(result)&&in.done();uncertain=result.state=="outcome_unknown";
                            accepted=result.effect=="locally_accepted"||result.effect=="observed_remote";none=result.effect=="none";return valid;});
                    if(status==error::again) { return status; }
                    if(status!=error::success||!valid||uncertain){unknown(row,"publish.unresolved");return status==error::success?error::io:status;}
                    ++row.publish_key;
                    if(accepted)row.current=phase::acknowledging;
                    else if(none){row.active=false;row.current=phase::releasing;}
                    else{unknown(row,"publish.invalid");return error::io;}
                    return error::success;
                }
            }
            if(row.current==phase::acknowledging||row.current==phase::releasing)
            {
                bool acknowledge=row.current==phase::acknowledging;std::array<std::uint8_t,48> body;wire::writer request{body};
                request.u64(row.source.resource);request.u64(row.settle_key+1);request.u64(row.retained.endpoint);request.u64(row.retained.provider_generation);
                request.u64(row.retained.sequence);request.u64(row.retained.nonce);std::size_t required{};bool valid{},accepted{};
                auto status=communication_.receive(communication_.context,row.owner,acknowledge?communication_receive_contract::operation::topic_acknowledge:
                    communication_receive_contract::operation::topic_release,row.source.capability,request.written(),256,required,[&](wire::bytes bytes)
                    {robot_contract::receipt result;robot_contract::response_decoder in{bytes,{256,0,255,0}};
                        valid=in.outcome(result)&&in.done();accepted=result.effect=="locally_accepted";return valid;});
                if(status==error::again) { return status; }
                if(status!=error::success||!valid||!accepted){unknown(row,"settlement.unresolved");return status==error::success?error::io:status;}
                ++row.settle_key;if(acknowledge)increment(row.forwarded);row.until=0;settled(row);return error::success;
            }
            return error::again;
        }
#if defined(__linux__)
        static void* supervise(void* pointer)noexcept
        {
            auto& self=*static_cast<bridge_dispatch*>(pointer);
            while(!self.stopped_.load())
            {
                for(auto const& row:self.bindings_)if(!self.stopped_.load())(void)self.pump(row->policy.alias.view());
                timespec delay{static_cast<time_t>(self.capacity_.poll_ns/1000000000),static_cast<long>(self.capacity_.poll_ns%1000000000)};
                while(::nanosleep(&delay,&delay)&&errno==EINTR&&!self.stopped_.load()){}
            }
            return nullptr;
        }
#endif
    public:
        bridge_dispatch(std::shared_ptr<authority> permissions,communication_host& host,handle generation,limits capacity)
            :bridge_dispatch(std::move(permissions),transport::from(host),generation,capacity){}
        bridge_dispatch(std::shared_ptr<authority> permissions,transport host,handle generation,limits capacity)
            :permissions_(std::move(permissions)),communication_(host),generation_(generation),capacity_(capacity){}
        ~bridge_dispatch(){retire();}
        static bool required_storage(plan const& value,std::uint64_t& output)noexcept
        {
            auto codec=communication::codecs::find(value.codec.view());
            if(!identifier(value.alias.view())||!identifier(value.source_alias.view())||!identifier(value.destination_alias.view())||
                value.source_alias==value.destination_alias||!proposals::envelope_profile(value.selected)||!proposals::envelope_profile(value.stop)||
                !proposals::envelope_profile(value.source_profile)||!proposals::envelope_profile(value.destination_profile)||
                !codec||(codec->codec!="std-string"&&codec->codec!="std-bool")||!value.message_bytes||value.message_bytes>buffers::maximum_transfer||
                !value.deadline_ns||value.deadline_ns>INT64_MAX)return false;
            output=sizeof(binding)+2*value.message_bytes+2048;return output<=SIZE_MAX;
        }
        error bind(plan const& value)
        {
            if(!same_process()) { return error::denied; }std::uint64_t required{};
            if(!generation_||!required_storage(value,required)||required>value.storage_bytes) { return error::invalid; }
            std::lock_guard lock{configuration_};if(prepared_.load()||stopped_.load()) { return error::denied; }
            if(find(value.alias.view())) { return error::conflict; }
            if(bindings_.size()>=capacity_.bindings||storage_>capacity_.storage_bytes||required>capacity_.storage_bytes-storage_) { return error::too_large; }
            // No configured destination may feed another route's source. This
            // prevents cycles inside this deployment; external topology remains
            // a separately reviewed native-network property.
            for(auto const& other:bindings_)
                if(other->policy.source_alias==value.destination_alias||other->policy.destination_alias==value.source_alias||
                    other->policy.source_alias==value.source_alias||other->policy.destination_alias==value.destination_alias) { return error::conflict; }
            try
            {
                auto row=::uwvm2::utils::container::make_shared<binding>();row->policy=value;row->generation=generation_;
                row->received.resize(static_cast<std::size_t>(value.message_bytes+1024));row->transmit.resize(static_cast<std::size_t>(value.message_bytes+1024));
                bindings_.push_back(std::move(row));storage_+=required;return error::success;
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}
        }
        error prepare()noexcept
        {if(!same_process()||!communication_.context||!communication_.invoke||!communication_.receive||!communication_.quiescent) { return error::denied; }
            std::lock_guard lock{configuration_};if(prepared_.load()||stopped_.load()) { return error::conflict; }prepared_.store(true);return error::success;}
        /** @brief Start only the trusted coordinator after all communication workers are prepared. */
        error start_supervisor()noexcept
        {
            if(!same_process()||!prepared_.load()||stopped_.load()||!capacity_.poll_ns||capacity_.poll_ns>100000000) { return error::invalid; }
#if defined(__linux__)
            std::lock_guard lock{join_};if(launched_) { return error::conflict; }
            auto status=stack_.prepare(capacity_.supervisor_bytes);if(status!=error::success) { return status; }
            pthread_attr_t attr;if(::pthread_attr_init(&attr)) { return error::io; }
            auto result=::pthread_attr_setstack(&attr,stack_.data(),stack_.size());if(!result)result=::pthread_create(&thread_,&attr,supervise,this);
            ::pthread_attr_destroy(&attr);if(result) { return error::io; }launched_=true;return error::success;
#else
            return error::unsupported;
#endif
        }
        error preflight(caller who,operation action,handle cap,std::size_t extent)
        {if(extent>4096) { return error::too_large; }std::shared_ptr<binding> row;return select(who,action,cap,row);}
        /** @brief Four canonical bridge operations; immutable endpoints and independent stop sequence. */
        error invoke(caller who,operation action,handle cap,bytes input,std::size_t capacity,std::size_t& required,sink deliver,cancellation cancelled={})noexcept
        {
            required=0;
            try
            {
                auto status=preflight(who,action,cap,input.size());if(status!=error::success) { return status; }
                if(!deliver) { return error::invalid; }robot_contract::request request;if(!robot_contract::decode(action,input,{4096,4096,255,4},request)) { return error::invalid; }
                std::shared_ptr<binding> row;status=select(who,action,cap,row);if(status!=error::success) { return status; }
                auto check=[&](){if(cancelled&&cancelled()) { return error::cancelled; }std::shared_ptr<binding> selected;return select(who,action,cap,selected);};
                status=check();if(status!=error::success) { return status; }
                std::array<std::uint8_t,1024> output;wire::writer encoded{output};
                if(action==operation::bridge_create)
                {
                    required=8;if(capacity<required) { return error::too_large; }
                    if(!match(request.selected,row->policy.selected)||request.bindings.count!=1) { return error::denied; }
                    robot_contract::decoder in{request.bindings.encoded,{request.bindings.encoded.size(),0,0,0}};
                    std::uint64_t slot{};robot_contract::reference source,destination;
                    if(!in.identity(slot)||slot!=1||!in.resource(source)||!in.resource(destination)||!in.done()||source==destination) { return error::invalid; }
                    group_lease allowed;status=group(who,cap,action,*row,source,destination,allowed);if(status!=error::success) { return status; }
                    lease budget;status=permissions_->preflight(who,cap,name(action),input.size()+8,budget);if(status!=error::success) { return status; }
                    auto codec=communication::codecs::find(row->policy.codec.view());
                    status=endpoint(who,source,row->policy.source_profile,true,*codec);if(status!=error::success) { return status; }
                    status=endpoint(who,destination,row->policy.destination_profile,false,*codec);if(status!=error::success) { return status; }
                    if(!allowed.specification(0)->max_resources) { return error::again; }
                    group_lease charged;status=group(who,cap,action,*row,source,destination,charged,input.size()+8,true);if(status!=error::success) { return status; }
                    {std::lock_guard lock{row->mutex};if(row->retired) { return error::denied; }if(row->id) { return error::again; }
                        row->id=resources::allocate_identity();if(!row->id) { return error::too_large; }row->owner=who;row->cap=cap;row->source=source;row->destination=destination;encoded.u64(row->id);}
                    charged.release();budget.release();allowed.release();bool copied{};if(check()==error::success)try{copied=deliver(encoded.written());}catch(...){copied=false;}
                    status=check();if(status==error::success)status=group(who,cap,action,*row,source,destination,allowed);
                    {std::lock_guard lock{row->mutex};if(row->retired&&status==error::success)status=error::cancelled;
                        if(!copied||status!=error::success){row->id=0;row->owner={};row->cap=0;row->source={};row->destination={};}
                        else row->published=true;}
                    return status!=error::success?status:copied?error::success:error::io;
                }
                robot_contract::receipt reply;bool replay{};
                group_lease fence;lease primary;status=permissions_->admit(who,cap,name(action),0,primary,false);if(status!=error::success) { return status; }
                {
                    std::lock_guard lock{row->mutex};if(row->retired) { return error::denied; }
                    if(!row->published||row->id!=request.resource||!same(who,row->owner)) { return error::bad_handle; }
                    if(action==operation::bridge_describe)
                    {if(!describe(encoded,*row)) { return error::io; }
                        required=encoded.size();if(capacity<required) { return error::too_large; }
                        lease charged;status=permissions_->admit(who,cap,name(action),input.size()+required,charged,true);
                        if(status!=error::success) { return status; }}
                    else
                    {
                        bool stopping=action==operation::bridge_stop;auto digest=buffers::hash(input);
                        auto& key=stopping?row->stop_key:row->normal_key;auto& fingerprint=stopping?row->stop_digest:row->normal_digest;
                        auto& stored=stopping?row->stopped:row->started;
                        if(key==request.key.sequence){if(digest!=fingerprint) { return error::conflict; }reply=stored;replay=true;}
                        if(!replay)
                        {
                            required=256;if(capacity<required) { return error::too_large; }auto time=now();
                            if(time>=request.deadline||request.deadline-time>row->policy.deadline_ns) { return error::timeout; }
                            if(key==UINT64_MAX||request.key.sequence!=key+1) { return error::conflict; }
                            if(row->current==phase::uncertain&&!stopping) { return error::conflict; }
                            if(stopping)
                            {
                                if(!match(request.selected,row->policy.stop)) { return error::denied; }
                                admission_claim claim{cap,name(action),0,false,false,true};status=permissions_->admit_group(who,{&claim,1},fence,true);
                                if(status!=error::success) { return status; }
                                if(!fence.consume(who,{&claim,1})) { return error::denied; }
                            }
                            else
                            {
                                if(row->stopping||row->active||row->current!=phase::empty) { return error::again; }
                                status=group(who,cap,action,*row,row->source,row->destination,fence,input.size()+256,true);if(status!=error::success) { return status; }
                            }
                            reply={resources::allocate_identity(),row->generation,stopping&&row->current!=phase::empty?"running":"succeeded",
                                stopping&&row->current!=phase::empty?"locally_accepted":"committed",""};if(!reply.request) { return error::too_large; }
                            if(stopping&&row->current==phase::uncertain){reply.state="outcome_unknown";reply.effect="unknown";reply.reason="retained.unresolved";}
                            key=request.key.sequence;fingerprint=digest;stored=reply;
                            if(stopping){row->stopping=row->current!=phase::empty;row->stop_until=request.deadline;row->active=false;}
                            else{row->active=true;row->until=0;}
                        }
                        if(!receipt(encoded,reply)) { return error::io; }
                    }
                }
                fence.release();primary.release();required=encoded.size();if(capacity<required) { return error::too_large; }
                status=check();if(status!=error::success) { return status; }if(!deliver(encoded.written())) { return error::io; }return check();
            }
            catch(std::bad_alloc const&){return error::no_memory;}catch(std::length_error const&){return error::too_large;}catch(...){return error::io;}
        }
        /** @brief One finite direction pump; not reachable from debugger inspection. */
        error pump(std::string_view alias)noexcept
        {
            if(!same_process()||!prepared_.load()||stopped_.load()) { return error::denied; }
            try
            {
                std::shared_ptr<binding> row;{std::lock_guard lock{configuration_};row=find(alias);}if(!row) { return error::denied; }
                caller who;handle cap{},id{};robot_contract::reference source,destination;
                {std::lock_guard lock{row->mutex};if(!row->published||row->retired||(!row->active&&!row->stopping)) { return error::again; }
                    who=row->owner;cap=row->cap;id=row->id;source=row->source;destination=row->destination;}
                group_lease allowed;auto status=group(who,cap,operation::bridge_start,*row,source,destination,allowed);
                std::lock_guard lock{row->mutex};if(row->id!=id||row->retired) { return error::cancelled; }
                if(status!=error::success){row->active=false;if(row->current!=phase::empty)unknown(*row,"authority.revoked");return status;}
                return step(*row);
            }
            catch(...){return error::io;}
        }
        error close(caller who,handle cap,handle id)
        {
            if(!same_process()) { return error::denied; }lease allowed;auto status=permissions_->admit(who,cap,"resource.close",0,allowed,false);if(status!=error::success) { return status; }
            std::shared_ptr<binding> row;{std::lock_guard lock{configuration_};row=find(allowed.specification().alias);}if(!row) { return error::denied; }
            std::lock_guard lock{row->mutex};if(!row->published||row->id!=id||!same(row->owner,who)) { return error::bad_handle; }
            if(row->active||row->stopping||row->current!=phase::empty) { return error::again; }
            row->published=false;row->id=0;row->retired=true;return error::success;
        }
        error inspect(std::string_view alias,inspection& output)const noexcept
        {
            if(!same_process()) { return error::denied; }std::shared_ptr<binding> row;
            {std::unique_lock lock{configuration_,std::try_to_lock};if(!lock.owns_lock()) { return error::again; }row=find(alias);}if(!row) { return error::denied; }
            std::unique_lock lock{row->mutex,std::try_to_lock};if(!lock.owns_lock()) { return error::again; }
            output={row->published?1u:0u,row->current!=phase::empty?1u:0u,row->current==phase::uncertain?1u:0u,
                row->forwarded,row->losses,row->policy.storage_bytes,row->active,row->retired,row->busy};return error::success;
        }
        /** @brief Nonblocking revocation fences dependent routes; retained unknown records stay charged. */
        void revoke(std::string_view alias)noexcept
        {
            if(!same_process()) { return; }std::lock_guard lock{configuration_};
            for(auto& row:bindings_)if(row->policy.alias.view()==alias||row->policy.source_alias.view()==alias||row->policy.destination_alias.view()==alias)
            {std::lock_guard owned{row->mutex};row->retired=true;row->active=false;if(row->current!=phase::empty)unknown(*row,"authority.revoked");}
        }
        void request_stop()noexcept{if(same_process())stopped_.store(true);}
        /** @brief Join without authority locks; no native acceptance is reclassified as cancellation. */
        void retire()noexcept
        {
            if(!same_process()) { return; }request_stop();std::lock_guard join{join_};
#if defined(__linux__)
            if(launched_&&!joined_){if(::pthread_join(thread_,nullptr)) { return; }joined_=true;}
#endif
            std::lock_guard lock{configuration_};for(auto& row:bindings_)
            {std::lock_guard owned{row->mutex};row->retired=true;row->active=false;if(row->current!=phase::empty)unknown(*row,"provider.retired");}
        }
    };
}
