/*************************************************************
 * UWVM2 concrete bounded ROS1 Actions. APL-2.0.             *
 *************************************************************/
#pragma once
#include "custom_topics.h"
#include "../common/bounded_feedback.h"
#include "../common/nonreusing_ids.h"
#include <actionlib_msgs/GoalStatusArray.h>
#include <actionlib_msgs/GoalID.h>
#ifndef UWVM_ROS_CUSTOM_ROS1_ACTION_TYPES
#define UWVM_ROS_CUSTOM_ROS1_ACTION_TYPES(M)
#endif
namespace uwvm2::ros_gateway::ros1_custom_actions
{
    // Every selected custom Action type/session shares this process-lifetime
    // domain. Goal cleanup must not reset the ordering or retain tombstones.
    inline nonreusing_id_allocator allocated_ids;
    inline const std::uint64_t process_nonce{ipc::random_session()};
    inline std::uint64_t allocate_id()
    { return allocated_ids.allocate(ipc::random_session); }
    /** C++17 ROS builds use the existing bounded, locale-independent native
     * formatter. Vendored fast_io requires C++20; no stream/locale I/O is added. */
    inline std::string native_id(std::uint64_t id) {return native_io::goal_identity(process_nonce,id);}
    template<class Native,class GoalCodec,class ResultCodec,class FeedbackCodec>
    bool qualified(ros::capability_spec const& cap)
    {
        using GoalMessage=typename Native::_action_goal_type;using ResultMessage=typename Native::_action_result_type;using FeedbackMessage=typename Native::_action_feedback_type;
        static_assert(std::is_same<typename GoalMessage::_goal_type,typename GoalCodec::Native>::value&&std::is_same<typename ResultMessage::_result_type,typename ResultCodec::Native>::value&&std::is_same<typename FeedbackMessage::_feedback_type,typename FeedbackCodec::Native>::value,"Action codecs must cover official components");
        auto item{ipc::find_custom_action(1,cap.type)};if(!item||cap.operation.rfind("action.",0)!=0||cap.type_identity!=item->native_identity||field(cap,"adapter-codec-identity")!=item->codec_identity) {return false;}
        std::string_view native_type{::ros::message_traits::DataType<Native>::value()};
        if(native_type.size()!=cap.type.size()+6||native_type.substr(0,cap.type.size())!=cap.type||native_type.substr(cap.type.size())!="Action"||std::string_view{::ros::message_traits::MD5Sum<Native>::value()}!=item->native_identity) {return false;}
#define UWVM_CUSTOM_COMPONENT(Role,Codec) if(item->Role->complete_vendor!=ipc::complete_vendor_codec<Codec>::value||item->Role->type!=Codec::type||item->Role->native_identity!=Codec::native_identity||item->Role->codec_identity!=Codec::codec_identity||item->Role->wire_bytes!=Codec::wire_bytes||!ros1_custom::binding<typename Codec::Native,Codec>::native_contract()) {return false;}
        if(ipc::complete_vendor_codec<GoalCodec>::value!=ipc::complete_vendor_codec<ResultCodec>::value||
           ipc::complete_vendor_codec<GoalCodec>::value!=ipc::complete_vendor_codec<FeedbackCodec>::value)return false;
        UWVM_CUSTOM_COMPONENT(goal,GoalCodec) UWVM_CUSTOM_COMPONENT(result,ResultCodec) UWVM_CUSTOM_COMPONENT(feedback,FeedbackCodec)
#undef UWVM_CUSTOM_COMPONENT
        return true;
    }
    inline bool known(ros::capability_spec const& cap)
    {
        unsigned matches{};bool accepted{};
#define UWVM_CUSTOM_ACTION_KNOWN(Native,Type,Identity,Canonical,Goal,Result,Feedback) if(cap.type==Type){++matches;accepted=qualified<Native,Goal,Result,Feedback>(cap)&&cap.type_identity==Identity&&field(cap,"adapter-codec-identity")==Canonical;}
        UWVM_ROS_CUSTOM_ROS1_ACTION_TYPES(UWVM_CUSTOM_ACTION_KNOWN)
#undef UWVM_CUSTOM_ACTION_KNOWN
        return matches==1&&accepted;
    }
    template<class Codec,class Value> bool frame(std::uint64_t id,std::uint8_t kind,Value const& value,bool result,std::vector<std::uint8_t>& output)
    {size_t n{};if(!id||!Codec::encoded_size(value,n)) {return false;}std::vector<std::uint8_t> next(n+(result?9:8));for(unsigned i{};i!=8;++i) {next[i]=static_cast<uint8_t>(id>>(8*i));}if(result) {next[8]=kind;}if(!Codec::encode(value,next.data()+(result?9:8),n)) {return false;}output=std::move(next);return true;}
    /** Concrete Actionlib topic transport generalized at complete typed payload
     * boundaries. Native callbacks are bounded and retain canonical active bytes,
     * with the shared pending-one feedback policy; no dynamic serialization plugin
     * is accepted and no hidden endpoint permission is installed. */
    template<class Native,class GoalCodec,class ResultCodec,class FeedbackCodec> struct implementation
    {
        using ActionGoal=typename Native::_action_goal_type;using ActionResult=typename Native::_action_result_type;using ActionFeedback=typename Native::_action_feedback_type;
        using NativeGoal=typename GoalCodec::Native;using NativeResult=typename ResultCodec::Native;
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
    inline static std::mutex registry_mutex;
    inline static std::map<std::uint64_t,std::weak_ptr<slot>> registry;
    struct endpoint
    {
        ros::capability_spec capability;bool server{},active{true};std::shared_ptr<event_queue> events;
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
        void emit_result(std::shared_ptr<endpoint> const& e,std::shared_ptr<slot> const& g,std::uint8_t native_status,NativeResult const& value)
        {
            ActionResult result;result.header.stamp=::ros::Time::now();g->status.status=native_status;result.status=g->status;result.result=value;e->result.publish(result);g->terminal=true;publish_status(e);
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
            e->goal=node.advertise<ActionGoal>(c.resource+"/goal",depth,false);
            e->cancel=node.advertise<actionlib_msgs::GoalID>(c.resource+"/cancel",depth,false);
            std::weak_ptr<state> weak{this->shared_from_this()};auto resource{c.resource};
            // Feedback/result channels do not preserve each other's delivery order.
            // Retain already published bounded feedback until local ownership is
            // actually retired, even when the result callback arrived first.
            boost::function<void(typename ActionFeedback::ConstPtr const&)> feedback{[weak,resource](auto const& message)
            {
                auto s{weak.lock()};if(!s||!message){return;}std::lock_guard<std::mutex> lock{s->mutex};auto g{s->find_native(resource,message->status.goal_id.id)};
                if(!s->active||!g||g->server||!feedback_callback_live(true,g->error,std::chrono::steady_clock::now()<g->deadline)||!FeedbackCodec::valid(message->feedback)){return;}
                size_t n{};if(!FeedbackCodec::encoded_size(message->feedback,n)||n+8>limit(g->capability,"max-feedback-bytes")) {return;}std::vector<std::uint8_t> bytes;if(frame<FeedbackCodec>(g->id,0,message->feedback,false,bytes)) {s->save_feedback(g,std::move(bytes));}
            }};
            boost::function<void(typename ActionResult::ConstPtr const&)> result{[weak,resource](auto const& message)
            {
                auto s{weak.lock()};if(!s||!message){return;}std::lock_guard<std::mutex> lock{s->mutex};auto g{s->find_native(resource,message->status.goal_id.id)};
                if(!s->active||!g||g->server||g->terminal){return;}g->terminal=true;
                if(!ResultCodec::valid(message->result)){g->error=90;return;}auto status{message->status.status};
                auto kind{static_cast<std::uint8_t>(status==3?2:status==2||status==8?4:3)};size_t n{};if(!ResultCodec::encoded_size(message->result,n)||n+9>limit(g->capability,"max-result-bytes")) {g->error=90;return;}std::vector<std::uint8_t> bytes;if(!frame<ResultCodec>(g->id,kind,message->result,true,bytes)) {g->error=22;return;}
                if(bytes.size()>limit(g->capability,"max-result-bytes")||bytes.size()>limit(g->capability,"max-event-queue-bytes")){g->error=90;return;}g->result=std::move(bytes);
            }};
            auto descriptor{ipc::find_custom_action(1,c.type)};if(!descriptor) {throw std::invalid_argument{"unregistered custom Action"};}
            auto feedback_max{limit(c,"max-feedback-bytes")},result_max{limit(c,"max-result-bytes")};
            e->feedback_sub=ros1_custom_input::subscribe<ActionFeedback>(node,c.resource+"/feedback",depth,feedback,[descriptor,feedback_max](auto bytes,auto size){return feedback_max>=8&&ros1_custom_input::action_body(bytes,size,true,*descriptor->feedback,std::min<std::uint64_t>(descriptor->feedback->wire_bytes,feedback_max-8));});
            e->result_sub=ros1_custom_input::subscribe<ActionResult>(node,c.resource+"/result",depth,result,[descriptor,result_max](auto bytes,auto size){return result_max>=9&&ros1_custom_input::action_body(bytes,size,true,*descriptor->result,std::min<std::uint64_t>(descriptor->result->wire_bytes,result_max-9));});
            std::weak_ptr<endpoint> weak_e{e};e->timer=node.createWallTimer(::ros::WallDuration{.1},[weak,weak_e](::ros::WallTimerEvent const&)
            {auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};if(!s->active||!e->active){return;}auto now{std::chrono::steady_clock::now()};for(auto const& pair:s->goals){auto const& g{pair.second};if(!g->server&&!g->terminal&&g->capability.alias==e->capability.alias&&g->deadline<=now){g->terminal=true;g->error=110;if(g->cancel_publish){g->cancel_publish();}feedback_discard(*g);}}});
            endpoints.emplace(c.alias,e);return e;
        }
        ipc::call_result advertise(ros::capability_spec const& c,std::shared_ptr<event_queue> events)
        {
            std::lock_guard<std::mutex> lock{mutex};if(!active){return {125,{}};}
            if(!events||events->capability.alias!=c.alias||events->capability.resource!=c.resource||events->capability.type!=c.type||events->capability.type_identity!=c.type_identity||events->capability.limits!=c.limits) {return {13,{}};}
            for(auto const& e:endpoints){if(e.second->server&&e.second->capability.resource==c.resource){return {17,{}};}}
            auto e{std::make_shared<endpoint>()};e->server=true;e->capability=c;e->events=std::move(events);auto depth{static_cast<std::uint32_t>(limit(c,"max-event-queue-depth"))};
            e->feedback=node.advertise<ActionFeedback>(c.resource+"/feedback",depth,false);
            e->result=node.advertise<ActionResult>(c.resource+"/result",depth,false);
            e->status=node.advertise<actionlib_msgs::GoalStatusArray>(c.resource+"/status",depth,false);
            std::weak_ptr<state> weak{this->shared_from_this()};std::weak_ptr<endpoint> weak_e{e};
            boost::function<void(typename ActionGoal::ConstPtr const&)> goal{[weak,weak_e](auto const& message)
            {
                auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};
                if(!s->active||!e->active||!message||!GoalCodec::valid(message->goal)||message->goal_id.id.empty()||message->goal_id.id.size()>256||s->find_native(e->capability.resource,message->goal_id.id)){return;}
                std::uint64_t count{};for(auto const& g:s->goals){if(g.second->server&&g.second->resource==e->capability.resource){++count;}}
                size_t goal_size{};if(!GoalCodec::encoded_size(message->goal,goal_size)||goal_size>limit(e->capability,"max-goal-bytes")) {return;}if(count>=limit(e->capability,"max-active-goals")){return;}auto id{allocate_id()};if(!id){return;}
                auto g{std::make_shared<slot>()};g->id=id;g->owner=s.get();g->resource=e->capability.resource;g->type_identity=e->capability.type_identity;g->native_id=message->goal_id.id;g->server=true;g->capability=e->capability;g->deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(g->capability,"timeout-ms"));g->status.goal_id=message->goal_id;g->status.status=1;
                g->cancel=[weak,weak_e,id]()->std::uint32_t{auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return 9;}std::lock_guard<std::mutex> lock{s->mutex};auto found{s->goals.find(id)};if(found==s->goals.end()){return 9;}auto g{found->second};if(g->terminal){return 17;}g->cancelled=true;s->emit_result(e,g,2,{});s->erase(id);return 0;};
                std::vector<std::uint8_t> bytes;if(!frame<GoalCodec>(id,0,message->goal,false,bytes)) {return;}
                if(!e->events->enqueue(std::move(bytes))){return;}s->goals.emplace(id,g);{std::lock_guard<std::mutex> global{registry_mutex};registry.emplace(id,g);}s->publish_status(e);
            }};
            boost::function<void(actionlib_msgs::GoalID::ConstPtr const&)> cancel{[weak,weak_e](auto const& message)
            {
                auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};
                if(!s->active||!e->active) {return;}std::vector<std::uint64_t> erase;for(auto const& pair:s->goals){auto const& g{pair.second};if(g->server&&g->resource==e->capability.resource&&!g->terminal&&(message->id==g->native_id||(message->id.empty()&&(message->stamp.isZero()||g->status.goal_id.stamp<=message->stamp)))){g->cancelled=true;s->emit_result(e,g,2,{});erase.push_back(g->id);}}
                for(auto id:erase){s->erase(id);}
            }};
            auto descriptor{ipc::find_custom_action(1,c.type)};if(!descriptor) {return {95,{}};}auto goal_max{limit(c,"max-goal-bytes")};
            e->goal_sub=ros1_custom_input::subscribe<ActionGoal>(node,c.resource+"/goal",depth,goal,[descriptor,goal_max](auto bytes,auto size){return ros1_custom_input::action_body(bytes,size,false,*descriptor->goal,std::min<std::uint64_t>(descriptor->goal->wire_bytes,goal_max));});
            e->cancel_sub=ros1_custom_input::subscribe<actionlib_msgs::GoalID>(node,c.resource+"/cancel",depth,cancel,ros1_custom_input::cancel_body);
            e->timer=node.createWallTimer(::ros::WallDuration{.1},[weak,weak_e](::ros::WallTimerEvent const&)
            {
                auto s{weak.lock()};auto e{weak_e.lock()};if(!s||!e){return;}std::lock_guard<std::mutex> lock{s->mutex};if(!s->active||!e->active){return;}
                std::vector<std::uint64_t> expired;auto now{std::chrono::steady_clock::now()};for(auto const& pair:s->goals){auto const& g{pair.second};if(g->server&&g->resource==e->capability.resource&&g->deadline<=now){s->emit_result(e,g,4,{});expired.push_back(g->id);}}
                for(auto id:expired){s->erase(id);}s->publish_status(e);
            });endpoints.emplace(c.alias,e);return {};
        }
        ipc::call_result call(ros::capability_spec const& c,std::vector<std::uint8_t> const& data,std::uint32_t timeout)
        {
            if(!qualified<Native,GoalCodec,ResultCodec,FeedbackCodec>(c)){return {13,{}};}
            if(c.operation=="action.send-goal")
            {
                NativeGoal request{};if(!GoalCodec::decode(data.data(),data.size(),request)){return {22,{}};}if(data.size()>limit(c,"max-goal-bytes")){return {90,{}};}
                std::unique_lock<std::mutex> lock{mutex};if(!active){return {125,{}};}std::uint64_t count{};for(auto const& g:goals){if(!g.second->server&&g.second->resource==c.resource){++count;}}if(count>=limit(c,"max-active-goals")){return {11,{}};}
                auto e{client(c)};lock.unlock();auto until{ipc::after(static_cast<uint32_t>(std::min<std::uint64_t>(timeout,limit(c,"timeout-ms"))))};
                while(!e->goal.getNumSubscribers()||!e->feedback_sub.getNumPublishers()||!e->result_sub.getNumPublishers()){if(std::chrono::steady_clock::now()>=until){return {110,{}};}(void)::poll(nullptr,0,5);}
                lock.lock();auto current{endpoints.find(c.alias)};if(!active||!e->active||current==endpoints.end()||current->second!=e) {return {9,{}};}if(std::chrono::steady_clock::now()>=until){return {110,{}};}auto id{allocate_id()};if(!id){return {90,{}};}auto g{std::make_shared<slot>()};g->id=id;g->owner=this;g->resource=c.resource;g->type_identity=c.type_identity;g->capability=c;g->native_id=native_io::goal_identity(process_nonce,id);g->deadline=std::chrono::steady_clock::now()+std::chrono::milliseconds(limit(c,"timeout-ms"));
                auto publisher{e->cancel};auto native{g->native_id};g->cancel_publish=[publisher,native]{actionlib_msgs::GoalID message;message.id=native;publisher.publish(message);};
                std::weak_ptr<state> weak{this->shared_from_this()};g->cancel=[weak,id]()->std::uint32_t{auto s{weak.lock()};if(!s){return 9;}std::lock_guard<std::mutex> lock{s->mutex};auto found{s->goals.find(id)};if(found==s->goals.end()){return 9;}auto g{found->second};if(g->terminal||!g->cancel_publish){return 17;}g->cancel_publish();return 0;};goals.emplace(id,g);{std::lock_guard<std::mutex> global{registry_mutex};registry.emplace(id,g);}
                ActionGoal goal;goal.header.stamp=::ros::Time::now();goal.goal_id.stamp=goal.header.stamp;goal.goal_id.id=g->native_id;goal.goal=std::move(request);e->goal.publish(goal);std::vector<std::uint8_t> reply;ipc::put(reply,id,8);return {0,std::move(reply)};
            }
            if(c.operation=="action.cancel-any-goal"||c.operation=="action.cancel-own-goal")
            {
                if(data.size()!=8){return {22,{}};}auto id{ipc::get(data.data(),8)};bool any{c.operation=="action.cancel-any-goal"};if(field(c,"cancel-scope")!=(any?"any":"own")||(!id&&!any)){return {13,{}};}
                if(!id){std::lock_guard<std::mutex> lock{mutex};auto entry{endpoints.find(c.alias)};if(entry==endpoints.end()){auto e{std::make_shared<endpoint>()};e->capability=c;e->cancel=node.advertise<actionlib_msgs::GoalID>(c.resource+"/cancel",static_cast<std::uint32_t>(limit(c,"max-event-queue-depth")),false);entry=endpoints.emplace(c.alias,e).first;}auto publisher{entry->second->cancel};auto until{ipc::after(static_cast<uint32_t>(std::min<std::uint64_t>(timeout,limit(c,"timeout-ms"))))};while(!publisher.getNumSubscribers()){if(std::chrono::steady_clock::now()>=until){return {110,{}};}(void)::poll(nullptr,0,5);}publisher.publish(actionlib_msgs::GoalID{});return {0,{1}};}
                std::shared_ptr<slot> g;{std::lock_guard<std::mutex> global{registry_mutex};auto it{registry.find(id)};if(it!=registry.end()){g=it->second.lock();}}
                if(!g){return {9,{}};}if(g->resource!=c.resource||g->type_identity!=c.type_identity||(!any&&g->owner!=this)){return {13,{}};}if(!g->cancel){return {17,{}};}auto error{g->cancel()};return error?ipc::call_result{error,{}}:ipc::call_result{0,{1}};
            }
            std::lock_guard<std::mutex> lock{mutex};if(!active){return {125,{}};}
            if(c.operation=="action.respond")
            {
                auto until{ipc::after(static_cast<uint32_t>(std::min<std::uint64_t>(timeout,limit(c,"timeout-ms"))))};
                auto descriptor{ipc::find_custom_action(1,c.type)};if(!descriptor||!ipc::validate_custom_action_request(*descriptor,"action.respond",data.data(),data.size())) {return {22,{}};}auto id{ipc::get(data.data(),8)};auto kind{data[8]};auto it{goals.find(id)};if(it==goals.end()){return {9,{}};}auto g{it->second};
                if(!g->server||g->resource!=c.resource||g->type_identity!=c.type_identity){return {13,{}};}if(g->terminal||g->deadline<=std::chrono::steady_clock::now()){return {110,{}};}
                if(kind==4&&!g->cancelled){return {13,{}};}
                // The responder authorizes the complete request, including
                // its outcome byte. The advertiser bounds canonical native
                // feedback, which omits that byte; terminal results retain it.
                auto maximum{limit(c,kind==1?"max-feedback-bytes":"max-result-bytes")};if(data.size()>maximum||data.size()-(kind==1?1u:0u)>limit(g->capability,kind==1?"max-feedback-bytes":"max-result-bytes")){return {90,{}};}
                std::shared_ptr<endpoint> e;for(auto const& pair:endpoints){if(pair.second->server&&pair.second->capability.resource==c.resource){e=pair.second;break;}}if(!e){return {9,{}};}
                if(std::chrono::steady_clock::now()>=until) {return {110,{}};}
                if(kind==1){ActionFeedback feedback;feedback.header.stamp=::ros::Time::now();feedback.status=g->status;if(!FeedbackCodec::decode(data.data()+9,data.size()-9,feedback.feedback)) {return {22,{}};}e->feedback.publish(feedback);}
                else{NativeResult value{};if(!ResultCodec::decode(data.data()+9,data.size()-9,value)) {return {22,{}};}emit_result(e,g,kind==2?3:kind==3?4:2,value);erase(id);}return std::chrono::steady_clock::now()>=until?ipc::call_result{110,{}}:ipc::call_result{};
            }
            if((c.operation=="action.receive-feedback"||c.operation=="action.receive-result")&&data.size()==8)
            {
                auto id{ipc::get(data.data(),8)};auto it{goals.find(id)};if(it==goals.end()){return {9,{}};}auto g{it->second};if(g->server||g->resource!=c.resource||g->type_identity!=c.type_identity){return {13,{}};}
                if(!action_receive_contract(g->capability,c,1)){return {13,{}};}
                if(c.operation=="action.receive-feedback"){
                    if(!g->terminal&&std::chrono::steady_clock::now()>=g->deadline) {g->terminal=true;g->error=110;if(g->cancel_publish) {g->cancel_publish();}feedback_discard(*g);}
                    if(g->error) {return {g->error,{}};}auto value{feedback_take(*g)};if(value.payload.size()>limit(c,"max-feedback-bytes")){return {90,{}};}return value;}
                if(g->error){auto error{g->error};erase(id);return {error,{}};}if(g->result.empty()){if(g->deadline<=std::chrono::steady_clock::now()){if(g->cancel_publish){g->cancel_publish();}erase(id);return {110,{}};}return {11,{}};}auto bytes{std::move(g->result)};erase(id);if(bytes.size()>limit(c,"max-result-bytes")){return {90,{}};}return {0,std::move(bytes)};
            }
            return {22,{}};
        }
        ipc::call_result observe(ros::capability_spec const& cap,std::uint64_t id)
        {
            if(!qualified<Native,GoalCodec,ResultCodec,FeedbackCodec>(cap)||!id) { return {13,{}}; }std::lock_guard<std::mutex> guard{mutex};if(!active) { return {125,{}}; }
            auto found{goals.find(id)};if(found==goals.end()) { return {9,{}}; }auto const& goal{*found->second};
            return !goal.server&&action_receive_contract(goal.capability,cap,1)?ipc::call_result{}:ipc::call_result{13,{}};
        }
        void close(std::string const& alias)
        {
            std::unique_lock<std::mutex> lock{mutex};auto it{endpoints.find(alias)};if(it==endpoints.end()){return;}auto e{it->second};e->active=false;std::vector<std::uint64_t> remove;
            for(auto const& pair:goals){auto const& g{pair.second};if(g->capability.alias==alias){if(g->server&&!g->terminal){emit_result(e,g,2,{});}else if(!g->terminal&&g->cancel_publish){g->cancel_publish();}remove.push_back(g->id);}}
            for(auto id:remove){erase(id);}if(e->events){e->events->deactivate();}endpoints.erase(it);lock.unlock();e->timer.stop();
        }
        void shutdown()
        {std::vector<std::string> aliases;{std::lock_guard<std::mutex> lock{mutex};for(auto const& e:endpoints){aliases.push_back(e.first);}}for(auto const& alias:aliases){close(alias);}std::lock_guard<std::mutex> lock{mutex};active=false;}
    };

    };
    class adapter
    {
        struct erased {virtual ~erased()=default;virtual ipc::call_result call(ros::capability_spec const&,std::vector<std::uint8_t> const&,uint32_t)=0;virtual ipc::call_result advertise(ros::capability_spec const&,std::shared_ptr<event_queue>)=0;virtual ipc::call_result observe(ros::capability_spec const&,uint64_t)=0;virtual void close(std::string const&)=0;};
        template<class Native,class Goal,class Result,class Feedback> struct selected final:erased
        {
            using state=typename implementation<Native,Goal,Result,Feedback>::state;std::shared_ptr<state> value;
            explicit selected(::ros::NodeHandle& node):value{std::make_shared<state>(node)} {}
            ~selected() override {value->shutdown();}
            ipc::call_result call(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,uint32_t timeout) override {return value->call(cap,bytes,timeout);}
            ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events) override {return value->advertise(cap,std::move(events));}
            ipc::call_result observe(ros::capability_spec const& cap,uint64_t id) override {return value->observe(cap,id);}
            void close(std::string const& alias) override {value->close(alias);}
        };
        std::map<std::string,std::unique_ptr<erased>> types_;
    public:
        explicit adapter(::ros::NodeHandle& node)
        {
            if(!process_nonce) {throw std::runtime_error("cannot allocate native Action identity");}
#define UWVM_CUSTOM_ACTION_SELECT(Native,Type,Identity,Canonical,Goal,Result,Feedback) if(!types_.emplace(Type,std::make_unique<selected<Native,Goal,Result,Feedback>>(node)).second) {throw std::invalid_argument("duplicate custom Action provider");}
            UWVM_ROS_CUSTOM_ROS1_ACTION_TYPES(UWVM_CUSTOM_ACTION_SELECT)
#undef UWVM_CUSTOM_ACTION_SELECT
        }
        ipc::call_result call(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,uint32_t timeout)
        {auto found{types_.find(cap.type)};return known(cap)&&found!=types_.end()?found->second->call(cap,bytes,timeout):ipc::call_result{13,{}};}
        ipc::call_result operation(ros::capability_spec const& cap,std::vector<std::uint8_t> const& bytes,uint32_t timeout) {return call(cap,bytes,timeout);}
        ipc::call_result advertise(ros::capability_spec const& cap,std::shared_ptr<event_queue> events)
        {auto found{types_.find(cap.type)};return known(cap)&&found!=types_.end()?found->second->advertise(cap,std::move(events)):ipc::call_result{13,{}};}
        ipc::call_result observe(ros::capability_spec const& cap,uint64_t id)
        {auto found{types_.find(cap.type)};return known(cap)&&found!=types_.end()?found->second->observe(cap,id):ipc::call_result{13,{}};}
        void close(std::string const& alias) {for(auto& item:types_) {item.second->close(alias);}}
    };
}
