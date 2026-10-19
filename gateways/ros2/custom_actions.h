/*************************************************************
 * UWVM2 concrete bounded ROS2 Actions. APL-2.0.             *
 *************************************************************/
#pragma once
#include "compat/node.h"
#include "custom_topics.h"
#include "custom_action_contract.h"
#include "qos.h"
#include "../common/bounded_feedback.h"
#include "../common/nonreusing_ids.h"
// Correlation allocation belongs to the wrapper, independently of whether a
// concrete custom Action type was selected. It creates no native endpoint.
namespace uwvm2::ros_gateway::ros2_custom_actions
{
    // This domain is shared by every selected custom Action type/session.
    // Cleanup retires ownership maps, never the process-wide no-reuse ordering.
    inline nonreusing_id_allocator allocated_ids;
    inline std::uint64_t allocate_id()
    { return allocated_ids.allocate(ipc::random_session); }
}
#if UWVM_ROS2_HAVE_CUSTOM_ACTION
#include <rclcpp_action/rclcpp_action.hpp>
#include "compat/action_client.h"
#include <action_msgs/srv/cancel_goal.hpp>
#include <rosidl_typesupport_cpp/action_type_support.hpp>
#ifndef UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES
#define UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(M)
#endif
namespace uwvm2::ros_gateway::ros2_custom_actions
{
    /** Gateway-global retention bound for all selected custom Action types.
     * Zero is a real zero budget. A timed-out/closed Guest goal remains here
     * until an actual native terminal result, never merely a cancel ACK. */
    class cleanup_pool
    {
        std::uint64_t maximum_;mutable std::mutex mutex_;std::map<std::uint64_t,std::shared_ptr<void>> pending_;
    public:
        explicit cleanup_pool(std::uint64_t maximum):maximum_{maximum} {}
        cleanup_pool(cleanup_pool const&)=delete;cleanup_pool& operator=(cleanup_pool const&)=delete;
        template<class Goal> bool track(std::uint64_t id,std::shared_ptr<Goal> const& goal)
        {if(!id||!goal) {return false;}std::lock_guard<std::mutex> lock{mutex_};return pending_.size()<maximum_&&pending_.emplace(id,goal).second;}
        void release(std::uint64_t id) {std::shared_ptr<void> retired;{std::lock_guard<std::mutex> lock{mutex_};auto it{pending_.find(id)};if(it!=pending_.end()) {retired=std::move(it->second);pending_.erase(it);}}}
        size_t count() const {std::lock_guard<std::mutex> lock{mutex_};return pending_.size();}
    };
    inline bool known(ros::capability_spec const& cap)
    {
        unsigned matches{};bool accepted{};
#define UWVM_CUSTOM_ACTION_KNOWN(Native,Type,Identity,Canonical,Goal,Result,Feedback) if(cap.type==Type){++matches;accepted=qualified<Native,Goal,Result,Feedback>(cap)&&cap.type_identity==Identity&&field(cap,"adapter-codec-identity")==Canonical;}
        UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(UWVM_CUSTOM_ACTION_KNOWN)
#undef UWVM_CUSTOM_ACTION_KNOWN
        return matches==1&&accepted;
    }
    struct response_header {std::uint64_t goal;std::uint8_t kind;};
    template<class Codec,class Value> bool frame(std::uint64_t id,std::uint8_t kind,Value const& value,bool result,std::vector<std::uint8_t>& output)
    {
        size_t n{};if(!id||!Codec::encoded_size(value,n)) {return false;}std::vector<std::uint8_t> next(n+(result?9:8));
        for(unsigned i{};i!=8;++i) {next[i]=static_cast<std::uint8_t>(id>>(8*i));}if(result) {next[8]=kind;}
        if(!Codec::encode(value,next.data()+(result?9:8),n)) {return false;}output=std::move(next);return true;
    }
    /** The complete builtin lifecycle is generalized only at typed payload
     * conversion points. Native ownership, deadline/cancel, five explicit QoS
     * channels, pending cleanup and non-consuming event credit checks remain. */
    template<class Native,class GoalCodec,class ResultCodec,class FeedbackCodec> struct implementation
    {
    using fibonacci=Native;
    using NativeGoal=typename Native::Goal;using NativeResult=typename Native::Result;using NativeFeedback=typename Native::Feedback;
    using client_goal=rclcpp_action::ClientGoalHandle<fibonacci>;
    using server_goal=rclcpp_action::ServerGoalHandle<fibonacci>;
    using client_pointer=typename rclcpp_action::Client<fibonacci>::SharedPtr;
    using client_goal_pointer=typename client_goal::SharedPtr;
    using wrapped_result=typename client_goal::WrappedResult;
    using send_options=typename rclcpp_action::Client<fibonacci>::SendGoalOptions;
    using server_pointer=typename rclcpp_action::Server<fibonacci>::SharedPtr;
    inline static rmw_qos_profile_t explicit_qos(ros::capability_spec const& cap,std::string const& channel)
    {
        auto key=[&](char const* property) { return "qos."+channel+"."+property; };
        rmw_qos_profile_t profile{};profile.history=RMW_QOS_POLICY_HISTORY_KEEP_LAST;profile.depth=limit(cap,key("depth"));
        profile.reliability=field(cap,key("reliability"))=="reliable"?RMW_QOS_POLICY_RELIABILITY_RELIABLE:RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
        profile.durability=field(cap,key("durability"))=="volatile"?RMW_QOS_POLICY_DURABILITY_VOLATILE:RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
        auto duration=[](std::string const& value)->rmw_time_t { if(value=="infinite") { return RMW_DURATION_INFINITE; }auto ns{integer(value,86400000000000ULL)};return {ns/1000000000ULL,ns%1000000000ULL}; };
        profile.deadline=duration(field(cap,key("deadline")));profile.lifespan=duration(field(cap,key("lifespan")));
        profile.liveliness=field(cap,key("liveliness"))=="automatic"?RMW_QOS_POLICY_LIVELINESS_AUTOMATIC:RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
        profile.liveliness_lease_duration=duration(field(cap,key("liveliness-lease-duration")));profile.avoid_ros_namespace_conventions=false;return profile;
    }
    static bool known(ros::capability_spec const& cap) {return qualified<Native,GoalCodec,ResultCodec,FeedbackCodec>(cap);}
    struct owner_state;
    struct goal_state
    {
        std::mutex mutex;
        std::weak_ptr<owner_state> owner;
        ros::capability_spec capability;
        std::uint64_t id{};
        bool active{true},terminal{},incoming{},response_pending{},cleanup_pending{},native_started{},native_terminal{},guest_retired{};
        std::weak_ptr<cleanup_pool> cleanup;
        std::uint32_t error{};
        std::chrono::steady_clock::time_point deadline;
        std::shared_ptr<rclcpp_action::Client<fibonacci>> client;
        client_goal_pointer outgoing;
        std::shared_ptr<server_goal> server;
        std::deque<std::vector<std::uint8_t>> feedback;
        std::size_t feedback_bytes{};
        std::optional<std::vector<std::uint8_t>> feedback_pending;event_queue_statistics feedback_stats;bucket feedback_quota;
        std::vector<std::uint8_t> result;
        rclcpp::TimerBase::SharedPtr timer;
        std::function<void()> retire_incoming;
    };
    struct owner_state
    {
        std::mutex mutex;
        bool active{true};
        std::map<std::uint64_t,std::shared_ptr<goal_state>> goals;
    };
    inline static std::mutex registry_mutex;
    inline static std::map<std::uint64_t,std::weak_ptr<goal_state>> registry;
    static std::uint64_t register_goal(std::shared_ptr<goal_state> const& goal)
    {
        auto id{allocate_id()};if(!id) {return 0;}
        std::lock_guard<std::mutex> guard{registry_mutex};registry.emplace(id,goal);return id;
    }
    inline static void unregister_goal(std::uint64_t id) { std::lock_guard<std::mutex> guard{registry_mutex};registry.erase(id); }
    inline static std::string uuid(rclcpp_action::GoalUUID const& id)
    { return std::string(reinterpret_cast<char const*>(id.data()),id.size()); }
    inline static void cancel_deadline_timer(rclcpp::TimerBase::SharedPtr const& timer) noexcept
    {
        if(timer) { try { timer->cancel(); } catch(std::exception const&) {} }
    }
    inline static bool publish_deadline_timer(std::shared_ptr<goal_state> const& goal,rclcpp::TimerBase::SharedPtr timer)
    {
        if(!timer) { return false; }
        bool published{},same_timer{};
        {
            std::lock_guard<std::mutex> guard{goal->mutex};
            if(goal->active&&!goal->terminal)
            {
                same_timer=goal->timer==timer;
                if(!same_timer) { goal->timer.swap(timer); }
                published=true;
            }
        }
        // Creation registers the timer with the Node before this publication.
        // A result/close/deadline callback may have retired the goal meanwhile.
        // Cancel and release rejected/replaced timers without holding goal->mutex.
        if(!same_timer) { cancel_deadline_timer(timer); }
        return published;
    }
    inline static bool stop_goal(std::shared_ptr<goal_state> const& goal)
    {
        client_pointer client;client_goal_pointer outgoing;std::shared_ptr<server_goal> server;rclcpp::TimerBase::SharedPtr timer;
        bool pending{},request_cancel{};
        {
            std::lock_guard<std::mutex> guard{goal->mutex};
            goal->active=false;timer=std::move(goal->timer);feedback_discard(*goal);
            client=goal->client;outgoing=goal->outgoing;server=goal->server;
            pending=goal->native_started&&!goal->native_terminal;
            request_cancel=pending&&client&&outgoing&&!goal->cleanup_pending;
            if(request_cancel) { goal->cleanup_pending=true; }
        }
        cancel_deadline_timer(timer);timer.reset();
        try
        {
            // Preserve result callbacks and shared cleanup capacity until an
            // actual terminal result. A cancellation reply can reject the request
            // or leave a goal canceling; neither proves native completion.
            if(request_cancel) { auto request=client->async_cancel_goal(outgoing);(void)request; }
            if(server&&server->is_active()) { auto result=std::make_shared<NativeResult>();if(server->is_canceling()) { server->canceled(result); }else { server->abort(result); } }
        }
        catch(std::exception const&) {}
        return pending;
    }
    inline static void retire_goal(std::shared_ptr<goal_state> const& goal)
    {
        bool native_done{};
        {
            std::lock_guard<std::mutex> guard{goal->mutex};
            goal->active=false;feedback_discard(*goal);
            native_done=!goal->native_started||goal->native_terminal;
        }
        // Retire Guest access immediately, but retain an unresolved goal in
        // its live owner's quota until the native terminal result is confirmed.
        { std::lock_guard<std::mutex> guard{goal->mutex};goal->guest_retired=true; }
        unregister_goal(goal->id);
        if(native_done)
        {
            if(auto owner=goal->owner.lock()) { std::lock_guard<std::mutex> guard{owner->mutex};owner->goals.erase(goal->id); }
            if(auto pool=goal->cleanup.lock()) { pool->release(goal->id); }
        }
    }
    class adapter
    {
        ::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node;
        std::shared_ptr<owner_state> owner{std::make_shared<owner_state>()};
        std::shared_ptr<cleanup_pool> pending_cleanup;
        struct advertised
        {
            std::mutex mutex;
            bool active{true};
            ros::capability_spec cap;
            std::weak_ptr<owner_state> owner;
            std::weak_ptr<event_queue> events;
            std::map<std::string,std::shared_ptr<goal_state>> reserved;
            server_pointer server;
        };
        std::map<std::string,std::shared_ptr<advertised>> servers;
        std::shared_ptr<goal_state> own(std::uint64_t id,ros::capability_spec const& cap)
        {
            std::lock_guard<std::mutex> guard{owner->mutex};auto it{owner->goals.find(id)};
            if(it==owner->goals.end()||it->second->capability.resource!=cap.resource||it->second->capability.type_identity!=cap.type_identity) { return nullptr; }
            { std::lock_guard<std::mutex> goal_guard{it->second->mutex};if(it->second->guest_retired) { return nullptr; } }
            return it->second;
        }
        static std::uint32_t ownership_error(std::uint64_t id)
        { std::lock_guard<std::mutex> guard{registry_mutex};auto it{registry.find(id)};return it!=registry.end()&&!it->second.expired()?13:9; }
        void release(std::shared_ptr<goal_state> const& goal)
        { retire_goal(goal); }
        static void deadline_timer(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr const& node,std::shared_ptr<goal_state> const& goal)
        {
            std::weak_ptr<goal_state> weak{goal};
            auto timer=node->create_wall_timer(std::chrono::milliseconds(std::min<std::uint64_t>(100,limit(goal->capability,"timeout-ms"))),[weak] {
                auto goal{weak.lock()};if(!goal) { return; }
                bool expired{};{ std::lock_guard<std::mutex> guard{goal->mutex};expired=goal->active&&!goal->terminal&&std::chrono::steady_clock::now()>=goal->deadline;if(expired) { goal->error=110;goal->terminal=true; } }
                if(expired) { stop_goal(goal);if(goal->retire_incoming) { goal->retire_incoming(); } }
            });
            publish_deadline_timer(goal,std::move(timer));
        }
        ipc::call_result send(ros::capability_spec const& cap,std::vector<std::uint8_t> const& data,std::uint32_t timeout)
        {
            NativeGoal request{};if(!GoalCodec::decode(data.data(),data.size(),request)) {return {22,{}};}if(data.size()>limit(cap,"max-goal-bytes")) {return {90,{}};}
            auto started{std::chrono::steady_clock::now()};
            auto until{started+std::chrono::milliseconds(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms")))};
            if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
            auto goal{std::make_shared<goal_state>()};goal->owner=owner;goal->capability=cap;goal->cleanup=pending_cleanup;goal->deadline=started+std::chrono::milliseconds(limit(cap,"timeout-ms"));
            {
                std::lock_guard<std::mutex> guard{owner->mutex};std::size_t count{};for(auto const& it:owner->goals) { if(it.second->capability.resource==cap.resource) { ++count; } }
                if(!owner->active||count>=limit(cap,"max-active-goals")) { return {11,{}}; }
                goal->id=register_goal(goal);if(!goal->id) { return {90,{}}; }owner->goals.emplace(goal->id,goal);
            }
            // Reserve shared cleanup capacity before any native send. Detached
            // sessions must not bypass this explicitly configured global bound.
            if(pending_cleanup&&!pending_cleanup->track(goal->id,goal)) { release(goal);return {11,{}}; }
            rcl_action_client_options_t client_options{};client_options.allocator=rcl_get_default_allocator();
            client_options.goal_service_qos=explicit_qos(cap,"goal-service");client_options.result_service_qos=explicit_qos(cap,"result-service");client_options.cancel_service_qos=explicit_qos(cap,"cancel-service");client_options.feedback_topic_qos=explicit_qos(cap,"feedback");client_options.status_topic_qos=explicit_qos(cap,"status");
            auto client{ros2_compat::create_action_client<fibonacci>(node,cap.resource,client_options)};
            { std::lock_guard<std::mutex> guard{goal->mutex};goal->client=client; }
            while(!goal->client->action_server_is_ready())
            {
                if(!node->get_node_base_interface()->get_context()->is_valid()||std::chrono::steady_clock::now()>=until) { release(goal);return {110,{}}; }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            std::weak_ptr<goal_state> weak{goal};send_options options;
            options.goal_response_callback=[weak](auto const& native_response) {
                auto handle{ros2_compat::goal_response<client_goal>(native_response)};
                auto goal{weak.lock()};if(!goal) { return; }bool inactive{};
                {
                    std::lock_guard<std::mutex> guard{goal->mutex};
                    goal->response_pending=false;goal->outgoing=handle;inactive=!goal->active;
                    if(!handle) { goal->native_terminal=true; }
                }
                if(inactive) { if(!stop_goal(goal)) { retire_goal(goal); } }
                else if(!handle) { if(auto pool=goal->cleanup.lock()) { pool->release(goal->id); } }
            };
            options.feedback_callback=[weak](client_goal_pointer,std::shared_ptr<NativeFeedback const> feedback) {
                auto goal{weak.lock()};if(!goal||!feedback) { return; }size_t length{};if(!FeedbackCodec::encoded_size(*feedback,length)||length+8>limit(goal->capability,"max-feedback-bytes")) {return;}
                // Result/feedback are independently ordered native channels. A terminal
                // callback does not retire unconsumed local ownership; bounded
                // in-flight feedback may still arrive before the same deadline.
                std::lock_guard<std::mutex> guard{goal->mutex};if(!feedback_callback_live(goal->active,goal->error,std::chrono::steady_clock::now()<goal->deadline)) { return; }
                std::vector<std::uint8_t> bytes;if(!frame<FeedbackCodec>(goal->id,0,*feedback,false,bytes)) {return;}
                if(bytes.size()>limit(goal->capability,"max-feedback-bytes")) { return; }
                if(goal->feedback_quota.take(goal->capability,bytes.size())) { (void)feedback_admit(*goal,std::move(bytes)); }
            };
            options.result_callback=[weak](wrapped_result const& result) {
                auto goal{weak.lock()};if(!goal) { return; }rclcpp::TimerBase::SharedPtr timer;bool inactive{},native_done{};
                {
                    std::lock_guard<std::mutex> guard{goal->mutex};
                    if(goal->native_terminal) { return; }
                    native_done=result.code==rclcpp_action::ResultCode::SUCCEEDED||result.code==rclcpp_action::ResultCode::ABORTED||result.code==rclcpp_action::ResultCode::CANCELED;
                    goal->native_terminal=native_done;inactive=!goal->active;
                    if(!inactive&&!goal->terminal)
                    {
                        goal->terminal=true;timer=std::move(goal->timer);
                        if(std::chrono::steady_clock::now()>=goal->deadline) { goal->error=110; }
                        else
                        {
                            auto kind{result.code==rclcpp_action::ResultCode::SUCCEEDED?2:result.code==rclcpp_action::ResultCode::ABORTED?3:result.code==rclcpp_action::ResultCode::CANCELED?4:0};
                            if(!kind||!result.result||!ResultCodec::valid(*result.result)) { goal->error=22; }
                            else
                            {
                                std::vector<std::uint8_t> bytes;size_t length{};if(!ResultCodec::encoded_size(*result.result,length)||length+9>limit(goal->capability,"max-result-bytes")) {goal->error=90;}else if(!frame<ResultCodec>(goal->id,static_cast<std::uint8_t>(kind),*result.result,true,bytes)) {goal->error=22;}
                                if(bytes.size()>limit(goal->capability,"max-result-bytes")) { goal->error=90; }
                                else if(!goal->error) { goal->result=std::move(bytes); }
                            }
                        }
                    }
                    else if(native_done) { goal->terminal=true;timer=std::move(goal->timer); }
                }
                cancel_deadline_timer(timer);
                if(inactive&&native_done) { retire_goal(goal); }
                else if(native_done) { if(auto pool=goal->cleanup.lock()) { pool->release(goal->id); } }
            };
            // The complete generated Goal was validated before allocating any native entity.
            if(!node->get_node_base_interface()->get_context()->is_valid()||std::chrono::steady_clock::now()>=until) { release(goal);return {110,{}}; }
            {
                std::lock_guard<std::mutex> guard{goal->mutex};
                if(!goal->active) { return {110,{}}; }
                goal->response_pending=true;goal->native_started=true;
            }
            std::shared_future<client_goal_pointer> pending;
            try { pending=goal->client->async_send_goal(request,options); }
            catch(...)
            {
                { std::lock_guard<std::mutex> guard{goal->mutex};goal->response_pending=false; }
                release(goal);throw;
            }
            // Retain an inactive, quota-counted goal through its actual native
            // terminal result. Cancellation acknowledgement alone is insufficient.
            if(pending.wait_until(until)!=std::future_status::ready) { if(!stop_goal(goal)) { release(goal); }return {110,{}}; }
            auto accepted{pending.get()};
            { std::lock_guard<std::mutex> guard{goal->mutex};goal->outgoing=accepted; }
            if(std::chrono::steady_clock::now()>=until) { if(!stop_goal(goal)) { release(goal); }return {110,{}}; }
            if(!accepted) { release(goal);return {13,{}}; }
            deadline_timer(node,goal);std::vector<std::uint8_t> answer;ipc::put(answer,goal->id,8);
            if(std::chrono::steady_clock::now()>=until) { if(!stop_goal(goal)) { release(goal); }return {110,{}}; }
            return {0,std::move(answer)};
        }
        ipc::call_result cancel(ros::capability_spec const& cap,std::uint64_t id,std::uint32_t timeout)
        {
            bool any{cap.operation=="action.cancel-any-goal"};if(field(cap,"cancel-scope")!=(any?"any":"own")) { return {13,{}}; }
            auto until{std::chrono::steady_clock::now()+std::chrono::milliseconds(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms")))};
            std::vector<std::shared_ptr<goal_state>> selected;
            if(any)
            {
                std::lock_guard<std::mutex> guard{registry_mutex};
                for(auto const& it:registry) { if(id&&it.first!=id) { continue; }if(auto goal=it.second.lock()) { if(goal->capability.resource==cap.resource&&goal->capability.type_identity==cap.type_identity) { selected.push_back(std::move(goal)); } } }
            }
            else { auto goal{own(id,cap)};if(goal) { selected.push_back(std::move(goal)); } }
            if(selected.empty()) { return {ownership_error(id),{}}; }if(selected.size()>limit(cap,"max-active-goals")) { return {11,{}}; }
            if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
            // Preflight the entire selection before the first native effect.
            // A later mismatch must not leave an earlier Goal canceled. These
            // capability fields are immutable after Goal registration. Include
            // pending outgoing Goals even before their native handle arrives;
            // arrival between this pass and delivery must not bypass the check.
            for(auto const& goal:selected)
            {
                std::lock_guard<std::mutex> guard{goal->mutex};
                if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
                if(goal->active&&!goal->terminal&&!goal->incoming&&
                   !ros::equal_action_cancel_qos_contract(goal->capability,cap)) { return {13,{}}; }
            }
            bool accepted{};
            for(auto const& goal:selected)
            {
                client_pointer client;client_goal_pointer handle;std::shared_ptr<server_goal> incoming;std::chrono::steady_clock::time_point goal_until;
                { std::lock_guard<std::mutex> guard{goal->mutex};if(!goal->active||goal->terminal) { continue; }client=goal->client;handle=goal->outgoing;incoming=goal->server;goal_until=std::min(until,goal->deadline); }
                try
                {
                    if(client&&handle)
                    {
                        if(std::chrono::steady_clock::now()>=goal_until) { return {110,{}}; }
                        auto pending{client->async_cancel_goal(handle)};
                        if(pending.wait_until(goal_until)!=std::future_status::ready||std::chrono::steady_clock::now()>=goal_until) { return {110,{}}; }
                        auto result{pending.get()};accepted=accepted||!result->goals_canceling.empty();
                    }
                    else if(incoming)
                    {
                        if(std::chrono::steady_clock::now()>=goal_until) { return {110,{}}; }
                        auto profile{explicit_qos(cap,"cancel-service")};
                        rclcpp::QoS configured{rclcpp::QoSInitialization::from_rmw(profile),profile};
                        auto cancel_client{ros2_compat::create_client<action_msgs::srv::CancelGoal>(node,cap.resource+"/_action/cancel_goal",configured)};
                        // This path creates an ordinary service client instead
                        // of reusing a Goal's Action client. Qualify both native
                        // endpoints before waiting for or calling the service.
                        if(!ros2_qos::client(*cancel_client,configured)) { return {95,{}}; }
                        auto now{std::chrono::steady_clock::now()};if(now>=goal_until) { return {110,{}}; }
                        if(!cancel_client->wait_for_service(goal_until-now)) { return {110,{}}; }
                        auto request{std::make_shared<action_msgs::srv::CancelGoal::Request>()};request->goal_info.goal_id.uuid=incoming->get_goal_id();
                        if(std::chrono::steady_clock::now()>=goal_until) { return {110,{}}; }
                        auto pending{cancel_client->async_send_request(request)};if(pending.wait_until(goal_until)!=std::future_status::ready||std::chrono::steady_clock::now()>=goal_until) { ros2_compat::remove_pending(*cancel_client,pending);return {110,{}}; }
                        accepted=accepted||!pending.get()->goals_canceling.empty();
                    }
                    else { return {9,{}}; }
                }
                catch(rclcpp_action::exceptions::UnknownGoalHandleError const&) { return {9,{}}; }
            }
            if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }
            return {0,{static_cast<std::uint8_t>(accepted?1:0)}};
        }
    public:
        explicit adapter(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr n,std::shared_ptr<cleanup_pool> cleanup={}):
            node{std::move(n)},pending_cleanup{std::move(cleanup)} {if(!node||!pending_cleanup) {throw std::invalid_argument("custom Action requires a Node and bounded global cleanup pool");}}
        ~adapter()
        {
            std::vector<std::string> aliases;for(auto const& it:servers) { aliases.push_back(it.first); }for(auto const& alias:aliases) { close(alias); }
            std::vector<std::shared_ptr<goal_state>> goals;{ std::lock_guard<std::mutex> guard{owner->mutex};owner->active=false;for(auto const& it:owner->goals) { goals.push_back(it.second); }owner->goals.clear(); }
            for(auto const& goal:goals) { if(!stop_goal(goal)) { retire_goal(goal); }else { unregister_goal(goal->id); } }
        }
        void close(std::string const& alias)
        {
            auto found{servers.find(alias)};std::shared_ptr<advertised> ad;
            if(found!=servers.end()) { ad=found->second;{ std::lock_guard<std::mutex> guard{ad->mutex};ad->active=false; } }
            std::vector<std::shared_ptr<goal_state>> goals;{ std::lock_guard<std::mutex> guard{owner->mutex};for(auto const& entry:owner->goals) { if(entry.second->capability.alias==alias) { goals.push_back(entry.second); } } }
            // Keep unresolved native goals quota-counted through their terminal
            // result. Retire Guest access immediately; release preserves the
            // pending client and owner quota until native completion.
            for(auto const& goal:goals) { stop_goal(goal);release(goal); }
            if(ad) { ad->server.reset();servers.erase(alias); }
        }
        ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {
            if(!known(cap)||cap.operation!="action.advertise") { return {13,{}}; }
            if(!events||events->capability.alias!=cap.alias||events->capability.resource!=cap.resource||events->capability.type!=cap.type||events->capability.type_identity!=cap.type_identity||events->capability.limits!=cap.limits) {return {13,{}};}
            for(auto const& it:servers) { if(it.second->cap.resource==cap.resource) { return {17,{}}; } }
            auto ad{std::make_shared<advertised>()};ad->cap=cap;ad->owner=owner;ad->events=events;std::weak_ptr<advertised> weak{ad};
            auto accept=[weak](rclcpp_action::GoalUUID const& id,std::shared_ptr<NativeGoal const> request) {
                auto ad{weak.lock()};size_t length{};if(!ad||!request||!GoalCodec::encoded_size(*request,length)||length>limit(ad->cap,"max-goal-bytes")) { return rclcpp_action::GoalResponse::REJECT; }
                auto owner{ad->owner.lock()};if(!owner) { return rclcpp_action::GoalResponse::REJECT; }
                auto goal{std::make_shared<goal_state>()};goal->owner=owner;goal->capability=ad->cap;goal->incoming=true;goal->deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(ad->cap,"timeout-ms"));
                std::scoped_lock guard{ad->mutex,owner->mutex};if(!ad->active||!owner->active||ad->reserved.size()>=limit(ad->cap,"max-active-goals")) { return rclcpp_action::GoalResponse::REJECT; }
                goal->id=register_goal(goal);if(!goal->id) { return rclcpp_action::GoalResponse::REJECT; }ad->reserved.emplace(uuid(id),goal);owner->goals.emplace(goal->id,goal);return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
            };
            auto canceled=[weak](std::shared_ptr<server_goal> handle) { auto ad{weak.lock()};if(!ad) { return rclcpp_action::CancelResponse::REJECT; }std::lock_guard<std::mutex> guard{ad->mutex};return ad->active&&ad->reserved.count(uuid(handle->get_goal_id()))?rclcpp_action::CancelResponse::ACCEPT:rclcpp_action::CancelResponse::REJECT; };
            std::weak_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> weak_node{node};
            auto accepted=[weak,weak_node](std::shared_ptr<server_goal> handle) {
                auto ad{weak.lock()};if(!ad) { return; }std::shared_ptr<goal_state> goal;
                { std::lock_guard<std::mutex> guard{ad->mutex};auto it{ad->reserved.find(uuid(handle->get_goal_id()))};if(it==ad->reserved.end()) { return; }goal=it->second; }
                { std::lock_guard<std::mutex> guard{goal->mutex};goal->server=handle; }
                std::weak_ptr<owner_state> weak_owner{ad->owner};auto id{goal->id};auto key{uuid(handle->get_goal_id())};
                goal->retire_incoming=[weak,weak_owner,id,key] {
                    if(auto ad=weak.lock()) { std::lock_guard<std::mutex> guard{ad->mutex};ad->reserved.erase(key); }
                    if(auto owner=weak_owner.lock()) { std::lock_guard<std::mutex> guard{owner->mutex};owner->goals.erase(id); }
                    unregister_goal(id);
                };
                std::vector<std::uint8_t> bytes;auto queue{ad->events.lock()};if(!queue||!frame<GoalCodec>(goal->id,0,*handle->get_goal(),false,bytes)||!queue->enqueue(std::move(bytes))) { stop_goal(goal);goal->retire_incoming(); }
                else if(auto node=weak_node.lock()) { deadline_timer(node,goal); }
            };
            rcl_action_server_options_t options{};options.allocator=rcl_get_default_allocator();options.goal_service_qos=explicit_qos(cap,"goal-service");options.result_service_qos=explicit_qos(cap,"result-service");options.cancel_service_qos=explicit_qos(cap,"cancel-service");options.feedback_topic_qos=explicit_qos(cap,"feedback");options.status_topic_qos=explicit_qos(cap,"status");options.result_timeout.nanoseconds=static_cast<std::int64_t>(limit(cap,"timeout-ms"))*1000000;
            ad->server=rclcpp_action::create_server<fibonacci>(node,cap.resource,std::move(accept),std::move(canceled),std::move(accepted),options);servers.emplace(cap.alias,std::move(ad));return {};
        }
        ipc::call_result observe(ros::capability_spec const& cap,std::uint64_t id)
        {
            if(!known(cap)||!id) { return {13,{}}; }auto goal{own(id,cap)};if(!goal) { return {ownership_error(id),{}}; }
            std::lock_guard<std::mutex> guard{goal->mutex};if(!goal->active) { return {125,{}}; }
            return !goal->incoming&&action_receive_contract(goal->capability,cap,2)?ipc::call_result{}:ipc::call_result{13,{}};
        }
        ipc::call_result operation(ros::capability_spec const& cap,std::vector<std::uint8_t> const& data,std::uint32_t timeout)
        {
            auto started{std::chrono::steady_clock::now()};
            if(!known(cap)) { return {95,{}}; }
            if(cap.operation=="action.send-goal") { return send(cap,data,timeout); }
            if(cap.operation=="action.cancel-own-goal"||cap.operation=="action.cancel-any-goal") { return data.size()==8?cancel(cap,ipc::get(data.data(),8),timeout):ipc::call_result{22,{}}; }
            if(cap.operation=="action.receive-feedback"||cap.operation=="action.receive-result")
            {
                if(data.size()!=8) { return {22,{}}; }auto id{ipc::get(data.data(),8)};auto goal{own(id,cap)};if(!goal) { return {ownership_error(id),{}}; }
                ipc::call_result result;bool expired{};
                { std::lock_guard<std::mutex> guard{goal->mutex};if(goal->incoming||!action_receive_contract(goal->capability,cap,2)) { return {13,{}}; }if(goal->error) { result.error=goal->error; }
                  else
                  {
                      // Executor dispatch can be stalled independently of IPC.
                      // Unresolved goals expire at their stored deadline even
                      // when their periodic timer has not run yet. A result
                      // completed before that deadline remains available.
                      if(goal->active&&!goal->terminal&&std::chrono::steady_clock::now()>=goal->deadline)
                      { goal->terminal=true;goal->error=110;expired=true;result.error=110; }
                      else if(cap.operation=="action.receive-feedback") { result=feedback_take(*goal);if(result.error) { return result; } }
                      else { if(!goal->terminal) { return {11,{}}; }result.payload=goal->result; }
                  }
                  if(result.payload.size()>limit(cap,cap.operation=="action.receive-feedback"?"max-feedback-bytes":"max-result-bytes")) { result={90,{}}; } }
                if(expired) { stop_goal(goal); }
                if(cap.operation=="action.receive-result") { release(goal); }return result;
            }
            if(cap.operation=="action.respond")
            {
                auto until{started+std::chrono::milliseconds(std::min<std::uint64_t>(timeout,limit(cap,"timeout-ms")))};
                auto descriptor{ipc::find_custom_action(2,cap.type)};if(!descriptor||!ipc::validate_custom_action_request(*descriptor,"action.respond",data.data(),data.size())) {return {22,{}};}response_header response{ipc::get(data.data(),8),data[8]};if(data.size()>limit(cap,response.kind==1?"max-feedback-bytes":"max-result-bytes")) {return {90,{}};}
                auto goal{own(response.goal,cap)};if(!goal) { return {ownership_error(response.goal),{}}; }
                bool completed{},expired{};
                {
                    std::lock_guard<std::mutex> guard{goal->mutex};
                    if(goal->error==110) { return {110,{}}; }
                    if(!goal->active||goal->terminal||!goal->incoming||!goal->server) { return {13,{}}; }
                    // Responses reuse the advertiser's five native endpoints.
                    // Reject a different explicit contract before any native effect.
                    if(!ros::equal_action_server_qos_contract(goal->capability,cap)) { return {13,{}}; }
                    auto now{std::chrono::steady_clock::now()};
                    if(now>=goal->deadline) { goal->terminal=true;goal->error=110;expired=true; }
                    else if(now>=until) { return {110,{}}; }
                    // The incoming endpoint retains its own response quota.
                    // Respond includes an outcome byte; native feedback\'s
                    // canonical envelope omits it, while Result keeps it.
                    else if(data.size()-(response.kind==1?1u:0u)>limit(goal->capability,response.kind==1?"max-feedback-bytes":"max-result-bytes")) { return {90,{}}; }
                }
                rclcpp::TimerBase::SharedPtr timer;
                if(!expired)
                {
                    // Preparation and waiting for the goal lock consume this
                    // call's budget. A short call timeout does not retire the
                    // still-live goal, so the caller can retry its response.
                    std::shared_ptr<NativeFeedback> feedback;
                    std::shared_ptr<NativeResult> result;
                    if(response.kind==1) { feedback=std::make_shared<NativeFeedback>();if(!FeedbackCodec::decode(data.data()+9,data.size()-9,*feedback)) {return {22,{}};} }
                    else { result=std::make_shared<NativeResult>();if(!ResultCodec::decode(data.data()+9,data.size()-9,*result)) {return {22,{}};} }
                    std::lock_guard<std::mutex> guard{goal->mutex};
                    if(goal->error==110) { return {110,{}}; }
                    if(!goal->active||goal->terminal||!goal->incoming||!goal->server) { return {13,{}}; }
                    auto now{std::chrono::steady_clock::now()};
                    if(now>=goal->deadline) { goal->terminal=true;goal->error=110;expired=true; }
                    else if(now>=until) { return {110,{}}; }
                    else if(response.kind==1) { goal->server->publish_feedback(feedback); }
                    else
                    {
                        if(response.kind==2) { goal->server->succeed(result); }
                        else if(response.kind==3) { goal->server->abort(result); }
                        else { if(!goal->server->is_canceling()) { return {17,{}}; }goal->server->canceled(result); }
                        goal->terminal=true;goal->active=false;timer=std::move(goal->timer);completed=true;
                    }
                }
                if(expired) { stop_goal(goal);if(goal->retire_incoming) { goal->retire_incoming(); }release(goal);return {110,{}}; }
                cancel_deadline_timer(timer);
                if(completed) { for(auto const& it:servers) { auto ad{it.second};std::lock_guard<std::mutex> guard{ad->mutex};for(auto pending=ad->reserved.begin();pending!=ad->reserved.end();) { if(pending->second==goal) { pending=ad->reserved.erase(pending); }else { ++pending; } } }release(goal); }
                if(std::chrono::steady_clock::now()>=until) { return {110,{}}; }return {};
            }
            return {95,{}};
        }
    };
    };
    class adapter
    {
        struct erased {virtual ~erased()=default;virtual ipc::call_result operation(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t)=0;virtual ipc::call_result advertise(ros::capability_spec const&,std::shared_ptr<event_queue>)=0;virtual ipc::call_result observe(ros::capability_spec const&,std::uint64_t)=0;virtual void close(std::string const&)=0;};
        template<class Native,class Goal,class Result,class Feedback> struct selected final:erased
        {
            using typed=typename implementation<Native,Goal,Result,Feedback>::adapter;typed value;
            selected(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,std::shared_ptr<cleanup_pool> pool):value{std::move(node),std::move(pool)} {}
            ipc::call_result operation(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout) override {return value.operation(cap,bytes,timeout);}
            ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events) override {return value.advertise(cap,std::move(events));}
            ipc::call_result observe(ros::capability_spec const& cap,std::uint64_t id) override {return value.observe(cap,id);}
            void close(std::string const& alias) override {value.close(alias);}
        };
        std::map<std::string,std::unique_ptr<erased>> types_;
    public:
        explicit adapter(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr node,std::shared_ptr<cleanup_pool> pool)
        {
            if(!node||!pool) {throw std::invalid_argument("custom Action adapter requires bounded cleanup pool");}
#define UWVM_CUSTOM_ACTION_SELECT(Native,Type,Identity,Canonical,Goal,Result,Feedback) if(!types_.emplace(Type,std::make_unique<selected<Native,Goal,Result,Feedback>>(node,pool)).second) {throw std::invalid_argument("duplicate custom Action provider");}
            UWVM_ROS_CUSTOM_ROS2_ACTION_TYPES(UWVM_CUSTOM_ACTION_SELECT)
#undef UWVM_CUSTOM_ACTION_SELECT
        }
        ipc::call_result operation(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout)
        {auto found{types_.find(cap.type)};return known(cap)&&found!=types_.end()?found->second->operation(cap,bytes,timeout):ipc::call_result{13,{}};}
        ipc::call_result call(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout) {return operation(cap,bytes,timeout);}
        ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {auto found{types_.find(cap.type)};return known(cap)&&found!=types_.end()?found->second->advertise(cap,std::move(events)):ipc::call_result{13,{}};}
        ipc::call_result observe(ros::capability_spec const& cap,std::uint64_t id)
        {auto found{types_.find(cap.type)};return known(cap)&&found!=types_.end()?found->second->observe(cap,id):ipc::call_result{13,{}};}
        void close(std::string const& alias) {for(auto& item:types_) {item.second->close(alias);}}
    };
}
#else
namespace uwvm2::ros_gateway::ros2_custom_actions
{
    /** No complete selected custom Action lifecycle means no native endpoint.
     * Tutorial package availability is irrelevant. These source-compatible
     * refusals grant no authority and report ENOTSUP. */
    class cleanup_pool { public: explicit cleanup_pool(std::uint64_t) {} };
    inline bool known(ros::capability_spec const&) { return false; }
    class adapter
    {
    public:
        explicit adapter(::uwvm2::ros_gateway::ros2_compat::node_type::SharedPtr,std::shared_ptr<cleanup_pool>) {}
        void close(std::string const&) noexcept {}
        ipc::call_result operation(ros::capability_spec const&,std::vector<std::uint8_t> const&,std::uint32_t) { return {95,{}}; }
        ipc::call_result call(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,std::uint32_t timeout) { return operation(cap,bytes,timeout); }
        ipc::call_result advertise(ros::capability_spec const&,std::shared_ptr<event_queue>) { return {95,{}}; }
        ipc::call_result observe(ros::capability_spec const&,std::uint64_t) { return {95,{}}; }
    };
}
#endif
