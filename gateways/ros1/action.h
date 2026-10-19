/*************************************************************
 * UWVM2 typed ROS1 Fibonacci Action adapter. APL-2.0.        *
 *************************************************************/
#pragma once
#include "../common/server.h"
#include "../common/nonreusing_ids.h"
#include "../common/bounded_feedback.h"
#include <ros/ros.h>
#include <uwvm2/imported/ros/ipc/action.h>
#include <actionlib_tutorials/FibonacciAction.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <actionlib_msgs/GoalID.h>

namespace uwvm2::ros_gateway::ros1_action
{
    using action=actionlib_tutorials::FibonacciAction;
    inline bool known(ros::capability_spec const& c)
    { return c.operation.rfind("action.",0)==0&&c.type=="actionlib_tutorials/Fibonacci"&&c.type_identity==::ros::message_traits::MD5Sum<action>::value(); }
    // Retired/failed goals never grow a lifetime tombstone set or make a
    // successful later goal reuse an earlier correlation ID.
    inline nonreusing_id_allocator allocated_ids;
    inline const std::uint64_t process_nonce{ipc::random_session()};
    inline std::uint64_t allocate_id()
    { return allocated_ids.allocate(ipc::random_session); }
    struct slot
    {
        std::uint64_t id{};void const* owner{};std::string resource,type_identity,native_id;bool server{},terminal{},cancelled{};
        std::chrono::steady_clock::time_point deadline;
        ros::capability_spec capability;
        std::deque<std::vector<std::uint8_t>> feedback;std::size_t feedback_bytes{};std::vector<std::uint8_t> result;
        std::optional<std::vector<std::uint8_t>> feedback_pending;event_queue_statistics feedback_stats;
        std::uint32_t error{};bucket quota;
        std::function<void()> cancel_publish;
        std::function<std::uint32_t()> cancel;
        actionlib_msgs::GoalStatus status;
    };
    inline std::mutex registry_mutex;
    inline std::map<std::uint64_t,std::weak_ptr<slot>> registry;
    inline std::vector<std::uint8_t> sequence(std::uint64_t id,std::uint8_t kind,std::vector<std::int32_t> const& values,bool result)
    {
        std::vector<std::uint8_t> d;ipc::put(d,id,8);if(result){d.push_back(kind);}ipc::put(d,values.size(),4);
        for(auto value:values){ipc::put(d,static_cast<std::uint32_t>(value),4);}return d;
    }
    inline bool decode_sequence(std::vector<std::uint8_t> const& data,std::uint64_t& id,std::uint8_t& kind,std::vector<std::int32_t>& values)
    {
        if(data.size()<13){return false;}id=ipc::get(data.data(),8);kind=data[8];auto n{ipc::get(data.data()+9,4)};
        if(!id||kind<1||kind>4||n>48||n!=(data.size()-13)/4||(data.size()-13)%4){return false;}
        values.clear();values.reserve(n);for(std::size_t i{};i<n;++i){auto v{static_cast<std::int32_t>(ipc::get(data.data()+13+4*i,4))};if(v<0){return false;}values.push_back(v);}return true;
    }
    struct endpoint
    {
        ros::capability_spec capability;bool server{};std::shared_ptr<event_queue> events;
        ::ros::Publisher goal,cancel,feedback,result,status;
        ::ros::Subscriber feedback_sub,result_sub,goal_sub,cancel_sub;
        ::ros::WallTimer timer;bucket status_quota;
    };
    struct state:std::enable_shared_from_this<state>
    {
        ::ros::NodeHandle& node;std::mutex mutex;bool active{true};std::map<std::uint64_t,std::shared_ptr<slot>> goals;
        std::map<std::string,std::shared_ptr<endpoint>> endpoints;
        explicit state(::ros::NodeHandle& n):node{n}{}
        void erase(std::uint64_t id)
        { goals.erase(id);std::lock_guard<std::mutex> lock{registry_mutex};registry.erase(id); }
        std::shared_ptr<slot> find_native(std::string const& resource,std::string const& native)
        { for(auto const& g:goals){if(g.second->resource==resource&&g.second->native_id==native){return g.second;}}return {}; }
        void publish_status(std::shared_ptr<endpoint> const& e)
        {
            actionlib_msgs::GoalStatusArray array;array.header.stamp=::ros::Time::now();
            for(auto const& g:goals){if(g.second->server&&g.second->resource==e->capability.resource){array.status_list.push_back(g.second->status);}}
            std::size_t bytes{16};for(auto const& status:array.status_list){bytes+=status.goal_id.id.size()+64;}if(bytes<=limit(e->capability,"max-event-queue-bytes")&&e->status_quota.take(e->capability,bytes)){e->status.publish(array);}
        }
        void emit_result(std::shared_ptr<endpoint> const& e,std::shared_ptr<slot> const& g,std::uint8_t native_status,std::vector<std::int32_t> const& values)
        {
            actionlib_tutorials::FibonacciActionResult result;result.header.stamp=::ros::Time::now();g->status.status=native_status;result.status=g->status;result.result.sequence=values;e->result.publish(result);g->terminal=true;publish_status(e);
        }
        bool save_feedback(std::shared_ptr<slot> const& g,std::vector<std::uint8_t> bytes)
        {
            auto const& c{g->capability};auto maximum{limit(c,"max-event-queue-bytes")};
            if(bytes.size()>limit(c,"max-feedback-bytes")||bytes.size()>maximum||!g->quota.take(c,bytes.size())){return false;}
            return feedback_admit(*g,std::move(bytes));
        }
        std::shared_ptr<endpoint> client(ros::capability_spec const& c)
        {
            auto old{endpoints.find(c.alias)};if(old!=endpoints.end()){return old->second;}
            auto e{std::make_shared<endpoint>()};e->capability=c;auto depth{static_cast<std::uint32_t>(limit(c,"max-event-queue-depth"))};
            e->goal=node.advertise<actionlib_tutorials::FibonacciActionGoal>(c.resource+"/goal",depth,false);
            e->cancel=node.advertise<actionlib_msgs::GoalID>(c.resource+"/cancel",depth,false);
            std::weak_ptr<state> weak{shared_from_this()};auto resource{c.resource};
            boost::function<void(actionlib_tutorials::FibonacciActionFeedback::ConstPtr const&)> feedback{[weak,resource](auto const& message)
            {
                auto s{weak.lock()};if(!s){return;}std::lock_guard<std::mutex> lock{s->mutex};auto g{s->find_native(resource,message->status.goal_id.id)};
                if(!g||g->server||!feedback_callback_live(s->active,g->error,std::chrono::steady_clock::now()<g->deadline)||message->feedback.sequence.size()>48||std::any_of(message->feedback.sequence.begin(),message->feedback.sequence.end(),[](auto value){return value<0;})){return;}
                s->save_feedback(g,sequence(g->id,1,message->feedback.sequence,false));
            }};
            boost::function<void(actionlib_tutorials::FibonacciActionResult::ConstPtr const&)> result{[weak,resource](auto const& message)
            {
                auto s{weak.lock()};if(!s){return;}std::lock_guard<std::mutex> lock{s->mutex};auto g{s->find_native(resource,message->status.goal_id.id)};
                if(!s->active||!g||g->server||g->terminal){return;}g->terminal=true;
                if(message->result.sequence.size()>48||std::any_of(message->result.sequence.begin(),message->result.sequence.end(),[](auto value){return value<0;})){g->error=90;return;}auto status{message->status.status};
                auto kind{static_cast<std::uint8_t>(status==3?2:status==2||status==8?4:3)};auto bytes{sequence(g->id,kind,message->result.sequence,true)};
                if(bytes.size()>limit(g->capability,"max-result-bytes")||bytes.size()>limit(g->capability,"max-event-queue-bytes")){g->error=90;return;}g->result=std::move(bytes);
            }};
            e->feedback_sub=node.subscribe<actionlib_tutorials::FibonacciActionFeedback>(c.resource+"/feedback",depth,feedback,::ros::VoidConstPtr{},::ros::TransportHints{}.tcp());
            e->result_sub=node.subscribe<actionlib_tutorials::FibonacciActionResult>(c.resource+"/result",depth,result,::ros::VoidConstPtr{},::ros::TransportHints{}.tcp());
            std::weak_ptr<endpoint> weak_e{e};e->timer=node.createWallTimer(::ros::WallDuration{.1},[weak,weak_e](::ros::WallTimerEvent const&)
            {auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};auto now{std::chrono::steady_clock::now()};for(auto const& pair:s->goals){auto const& g{pair.second};if(!g->server&&!g->terminal&&g->capability.alias==e->capability.alias&&g->deadline<=now){g->terminal=true;g->error=110;if(g->cancel_publish){g->cancel_publish();}feedback_discard(*g);}}});
            endpoints.emplace(c.alias,e);return e;
        }
        ipc::call_result advertise(ros::capability_spec const& c,std::shared_ptr<event_queue> events)
        {
            std::lock_guard<std::mutex> lock{mutex};if(!active){return {125,{}};}
            for(auto const& e:endpoints){if(e.second->server&&e.second->capability.resource==c.resource){return {17,{}};}}
            auto e{std::make_shared<endpoint>()};e->server=true;e->capability=c;e->events=std::move(events);auto depth{static_cast<std::uint32_t>(limit(c,"max-event-queue-depth"))};
            e->feedback=node.advertise<actionlib_tutorials::FibonacciActionFeedback>(c.resource+"/feedback",depth,false);
            e->result=node.advertise<actionlib_tutorials::FibonacciActionResult>(c.resource+"/result",depth,false);
            e->status=node.advertise<actionlib_msgs::GoalStatusArray>(c.resource+"/status",depth,false);
            std::weak_ptr<state> weak{shared_from_this()};std::weak_ptr<endpoint> weak_e{e};
            boost::function<void(actionlib_tutorials::FibonacciActionGoal::ConstPtr const&)> goal{[weak,weak_e](auto const& message)
            {
                auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};
                if(!s->active||message->goal.order<0||static_cast<std::uint64_t>(message->goal.order)>limit(e->capability,"fibonacci.max-order")||message->goal_id.id.empty()||message->goal_id.id.size()>256||s->find_native(e->capability.resource,message->goal_id.id)){return;}
                std::uint64_t count{};for(auto const& g:s->goals){if(g.second->server&&g.second->resource==e->capability.resource){++count;}}
                if(count>=limit(e->capability,"max-active-goals")){return;}auto id{allocate_id()};if(!id){return;}
                auto g{std::make_shared<slot>()};g->id=id;g->owner=s.get();g->resource=e->capability.resource;g->type_identity=e->capability.type_identity;g->native_id=message->goal_id.id;g->server=true;g->capability=e->capability;g->deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(g->capability,"timeout-ms"));g->status.goal_id=message->goal_id;g->status.status=1;
                g->cancel=[weak,weak_e,id]()->std::uint32_t{auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return 9;}std::lock_guard<std::mutex> lock{s->mutex};auto found{s->goals.find(id)};if(found==s->goals.end()){return 9;}auto g{found->second};if(g->terminal){return 17;}g->cancelled=true;s->emit_result(e,g,2,{});s->erase(id);return 0;};
                std::vector<std::uint8_t> bytes;ipc::put(bytes,id,8);ipc::put(bytes,static_cast<std::uint32_t>(message->goal.order),4);
                if(!e->events->enqueue(std::move(bytes))){return;}s->goals.emplace(id,g);{std::lock_guard<std::mutex> global{registry_mutex};registry.emplace(id,g);}s->publish_status(e);
            }};
            boost::function<void(actionlib_msgs::GoalID::ConstPtr const&)> cancel{[weak,weak_e](auto const& message)
            {
                auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};
                std::vector<std::uint64_t> erase;for(auto const& pair:s->goals){auto const& g{pair.second};if(g->server&&g->resource==e->capability.resource&&!g->terminal&&(message->id==g->native_id||(message->id.empty()&&(message->stamp.isZero()||g->status.goal_id.stamp<=message->stamp)))){g->cancelled=true;s->emit_result(e,g,2,{});erase.push_back(g->id);}}
                for(auto id:erase){s->erase(id);}
            }};
            e->goal_sub=node.subscribe<actionlib_tutorials::FibonacciActionGoal>(c.resource+"/goal",depth,goal,::ros::VoidConstPtr{},::ros::TransportHints{}.tcp());
            e->cancel_sub=node.subscribe<actionlib_msgs::GoalID>(c.resource+"/cancel",depth,cancel,::ros::VoidConstPtr{},::ros::TransportHints{}.tcp());
            e->timer=node.createWallTimer(::ros::WallDuration{.1},[weak,weak_e](::ros::WallTimerEvent const&)
            {
                auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};if(!s->active){return;}
                std::vector<std::uint64_t> expired;auto now{std::chrono::steady_clock::now()};for(auto const& pair:s->goals){auto const& g{pair.second};if(g->server&&g->resource==e->capability.resource&&g->deadline<=now){s->emit_result(e,g,4,{});expired.push_back(g->id);}}
                for(auto id:expired){s->erase(id);}s->publish_status(e);
            });endpoints.emplace(c.alias,e);return {};
        }
        ipc::call_result call(ros::capability_spec const& c,std::vector<std::uint8_t> const& data,std::uint32_t timeout)
        {
            if(!known(c)){return {13,{}};}
            if(c.operation=="action.send-goal")
            {
                if(data.size()!=4||ipc::get(data.data(),4)>limit(c,"fibonacci.max-order")||data.size()>limit(c,"max-goal-bytes")){return {22,{}};}
                std::unique_lock<std::mutex> lock{mutex};if(!active){return {125,{}};}std::uint64_t count{};for(auto const& g:goals){if(!g.second->server&&g.second->resource==c.resource){++count;}}if(count>=limit(c,"max-active-goals")){return {11,{}};}
                auto e{client(c)};lock.unlock();auto until{std::chrono::steady_clock::now()+std::chrono::milliseconds(timeout)};
                while(!e->goal.getNumSubscribers()||!e->feedback_sub.getNumPublishers()||!e->result_sub.getNumPublishers()){if(std::chrono::steady_clock::now()>=until){return {110,{}};}std::this_thread::sleep_for(std::chrono::milliseconds(5));}
                lock.lock();auto id{allocate_id()};if(!id){return {90,{}};}auto g{std::make_shared<slot>()};g->id=id;g->owner=this;g->resource=c.resource;g->type_identity=c.type_identity;g->capability=c;g->native_id=native_io::goal_identity(process_nonce,id);g->deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(c,"timeout-ms"));
                auto publisher{e->cancel};auto native{g->native_id};g->cancel_publish=[publisher,native]{actionlib_msgs::GoalID message;message.id=native;publisher.publish(message);};
                std::weak_ptr<state> weak{shared_from_this()};g->cancel=[weak,id]()->std::uint32_t{auto s{weak.lock()};if(!s){return 9;}std::lock_guard<std::mutex> lock{s->mutex};auto found{s->goals.find(id)};if(found==s->goals.end()){return 9;}auto g{found->second};if(g->terminal||!g->cancel_publish){return 17;}g->cancel_publish();return 0;};goals.emplace(id,g);{std::lock_guard<std::mutex> global{registry_mutex};registry.emplace(id,g);}
                actionlib_tutorials::FibonacciActionGoal goal;goal.header.stamp=::ros::Time::now();goal.goal_id.stamp=goal.header.stamp;goal.goal_id.id=g->native_id;goal.goal.order=static_cast<std::int32_t>(ipc::get(data.data(),4));e->goal.publish(goal);std::vector<std::uint8_t> reply;ipc::put(reply,id,8);return {0,std::move(reply)};
            }
            if(c.operation=="action.cancel-any-goal"||c.operation=="action.cancel-own-goal")
            {
                if(data.size()!=8){return {22,{}};}auto id{ipc::get(data.data(),8)};bool any{c.operation=="action.cancel-any-goal"};if(field(c,"cancel-scope")!=(any?"any":"own")||(!id&&!any)){return {13,{}};}
                if(!id){std::lock_guard<std::mutex> lock{mutex};auto entry{endpoints.find(c.alias)};if(entry==endpoints.end()){auto e{std::make_shared<endpoint>()};e->capability=c;e->cancel=node.advertise<actionlib_msgs::GoalID>(c.resource+"/cancel",static_cast<std::uint32_t>(limit(c,"max-event-queue-depth")),false);entry=endpoints.emplace(c.alias,e).first;}auto publisher{entry->second->cancel};auto until{std::chrono::steady_clock::now()+std::chrono::milliseconds(timeout)};while(!publisher.getNumSubscribers()){if(std::chrono::steady_clock::now()>=until){return {110,{}};}std::this_thread::sleep_for(std::chrono::milliseconds(5));}publisher.publish(actionlib_msgs::GoalID{});return {0,{1}};}
                std::shared_ptr<slot> g;{std::lock_guard<std::mutex> global{registry_mutex};auto it{registry.find(id)};if(it!=registry.end()){g=it->second.lock();}}
                if(!g){return {9,{}};}if(g->resource!=c.resource||g->type_identity!=c.type_identity||(!any&&g->owner!=this)){return {13,{}};}if(!g->cancel){return {17,{}};}auto error{g->cancel()};return error?ipc::call_result{error,{}}:ipc::call_result{0,{1}};
            }
            std::lock_guard<std::mutex> lock{mutex};if(!active){return {125,{}};}
            if(c.operation=="action.respond")
            {
                std::uint64_t id{};std::uint8_t kind{};std::vector<std::int32_t> values;if(!decode_sequence(data,id,kind,values)){return {22,{}};}auto it{goals.find(id)};if(it==goals.end()){return {9,{}};}auto g{it->second};
                if(!g->server||g->resource!=c.resource||g->type_identity!=c.type_identity){return {13,{}};}if(g->terminal||g->deadline<=std::chrono::steady_clock::now()){return {110,{}};}
                if(kind==4&&!g->cancelled){return {13,{}};}
                // The responder authorizes the complete request, including
                // its outcome byte. The advertiser bounds canonical native
                // feedback, which omits that byte; terminal results retain it.
                auto maximum{limit(c,kind==1?"max-feedback-bytes":"max-result-bytes")};if(data.size()>maximum||data.size()-(kind==1?1u:0u)>limit(g->capability,kind==1?"max-feedback-bytes":"max-result-bytes")){return {90,{}};}
                std::shared_ptr<endpoint> e;for(auto const& pair:endpoints){if(pair.second->server&&pair.second->capability.resource==c.resource){e=pair.second;break;}}if(!e){return {9,{}};}
                if(kind==1){actionlib_tutorials::FibonacciActionFeedback feedback;feedback.header.stamp=::ros::Time::now();feedback.status=g->status;feedback.feedback.sequence=std::move(values);e->feedback.publish(feedback);}
                else{emit_result(e,g,kind==2?3:kind==3?4:2,values);erase(id);}return {};
            }
            if((c.operation=="action.receive-feedback"||c.operation=="action.receive-result")&&data.size()==8)
            {
                auto id{ipc::get(data.data(),8)};auto it{goals.find(id)};if(it==goals.end()){return {9,{}};}auto g{it->second};if(g->server||g->resource!=c.resource||g->type_identity!=c.type_identity){return {13,{}};}
                if(!action_receive_contract(g->capability,c,1)){return {13,{}};}
                if(c.operation=="action.receive-feedback"){auto value{feedback_take(*g)};if(value.payload.size()>limit(c,"max-feedback-bytes")){return {90,{}};}return value;}
                if(g->error){auto error{g->error};erase(id);return {error,{}};}if(g->result.empty()){if(g->deadline<=std::chrono::steady_clock::now()){if(g->cancel_publish){g->cancel_publish();}erase(id);return {110,{}};}return {11,{}};}auto bytes{std::move(g->result)};erase(id);if(bytes.size()>limit(c,"max-result-bytes")){return {90,{}};}return {0,std::move(bytes)};
            }
            return {22,{}};
        }
        ipc::call_result observe(ros::capability_spec const& cap,std::uint64_t id)
        {
            if(!known(cap)||!id) { return {13,{}}; }std::lock_guard<std::mutex> guard{mutex};if(!active) { return {125,{}}; }
            auto found{goals.find(id)};if(found==goals.end()) { return {9,{}}; }auto const& goal{*found->second};
            return !goal.server&&action_receive_contract(goal.capability,cap,1)?ipc::call_result{}:ipc::call_result{13,{}};
        }
        void close(std::string const& alias)
        {
            std::unique_lock<std::mutex> lock{mutex};auto it{endpoints.find(alias)};if(it==endpoints.end()){return;}auto e{it->second};std::vector<std::uint64_t> remove;
            for(auto const& pair:goals){auto const& g{pair.second};if(g->capability.alias==alias){if(g->server&&!g->terminal){emit_result(e,g,2,{});}else if(!g->terminal&&g->cancel_publish){g->cancel_publish();}remove.push_back(g->id);}}
            for(auto id:remove){erase(id);}if(e->events){e->events->deactivate();}endpoints.erase(it);lock.unlock();e->timer.stop();
        }
        void shutdown()
        {std::vector<std::string> aliases;{std::lock_guard<std::mutex> lock{mutex};for(auto const& e:endpoints){aliases.push_back(e.first);}}for(auto const& alias:aliases){close(alias);}std::lock_guard<std::mutex> lock{mutex};active=false;}
    };
    class adapter
    {
        std::shared_ptr<state> value;
    public:
        explicit adapter(::ros::NodeHandle& node):value{std::make_shared<state>(node)}{if(!process_nonce){throw std::runtime_error("cannot allocate unpredictable native Action identity");}}
        ~adapter(){value->shutdown();}
        ipc::call_result call(ros::capability_spec const& cap,std::vector<std::uint8_t> const& data,std::uint32_t timeout){return value->call(cap,data,timeout);}
        ipc::call_result observe(ros::capability_spec const& cap,std::uint64_t id){return value->observe(cap,id);}
        ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events){return known(cap)?value->advertise(cap,std::move(events)):ipc::call_result{13,{}};}
        void close(std::string const& alias){value->close(alias);}
    };
}
