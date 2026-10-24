#include "ros2_common.h"
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <action_tutorials_interfaces/action/fibonacci.hpp>
using namespace action_probe;
using fibonacci=action_tutorials_interfaces::action::Fibonacci;
#include "witness_ros2.h"
int main(int argc,char** argv)
{
    try
    {
        if(argc>=2&&std::string{argv[1]}.rfind("--",0)==0) { return ros2_action_witness(argc,argv); }
        require(argc==2,"usage ROS2 Action probe gateway");::setenv("ROS_DOMAIN_ID","77",1);::setenv("RMW_IMPLEMENTATION","rmw_fastrtps_cpp",1);::setenv("ROS_SECURITY_ENABLE","false",1);rclcpp::init(argc,argv);
        auto node{std::make_shared<rclcpp::Node>("native","/uwvm_actions")};std::atomic<unsigned> native_goals{},native_cancels{},native_feedback{};
        std::mutex workers_mutex;std::vector<std::thread> workers;
        auto server{rclcpp_action::create_server<fibonacci>(node,"/uwvm_actions/native",[&](auto const&,std::shared_ptr<fibonacci::Goal const> goal) { if(goal->order<0||goal->order>46) { return rclcpp_action::GoalResponse::REJECT; }++native_goals;if(goal->order==31){std::this_thread::sleep_for(std::chrono::milliseconds(600));}return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE; },[&](auto) { ++native_cancels;return rclcpp_action::CancelResponse::ACCEPT; },[&](auto goal) {
            std::lock_guard<std::mutex> guard{workers_mutex};workers.emplace_back([goal] {
                auto feedback{std::make_shared<fibonacci::Feedback>()};feedback->partial_sequence={0,1};
                for(int i{1};i<goal->get_goal()->order;++i) {
                    if(goal->is_canceling()) { auto result=std::make_shared<fibonacci::Result>();result->sequence=feedback->partial_sequence;goal->canceled(result);return; }
                    feedback->partial_sequence.push_back(feedback->partial_sequence[i]+feedback->partial_sequence[i-1]);goal->publish_feedback(feedback);std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
                auto result=std::make_shared<fibonacci::Result>();result->sequence=feedback->partial_sequence;if(goal->is_canceling()) { goal->canceled(result); }else { goal->succeed(result); }
            });
        })};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner{[&] { executor.spin(); }};
        auto cleanup=[&] { executor.cancel();spinner.join();{ std::lock_guard<std::mutex> guard{workers_mutex};for(auto& worker:workers) { worker.join(); } }rclcpp::shutdown(); };
        try
        {
            fixture f{core::version::ros2,argv[1],[](gateway::configuration& config) {
                config.policy.connection["domain-id"]="77";config.policy.capabilities.clear();
                for(auto const& item:std::vector<std::pair<std::string,std::string>>{{"send","send-goal"},{"own","cancel-own-goal"},{"any","cancel-any-goal"},{"feedback","receive-feedback"},{"result","receive-result"}}) { auto c{action(core::version::ros2,item.first,"action."+item.second,"/uwvm_actions/native")};config.policy.capabilities.emplace(c.alias,std::move(c)); }
                for(auto op:{"advertise","respond"}) { auto c{action(core::version::ros2,op,std::string{"action."}+op,"/uwvm_actions/gateway")};config.policy.capabilities.emplace(c.alias,std::move(c)); }
                auto graph{core::capability_spec{"inspect","graph.inspect-action","/uwvm_actions/native","builtin/graph-list-v1","builtin:graph-list-v1",{{"allowed-name-prefix","/uwvm_actions"},{"max-graph-result-bytes","65536"},{"timeout-ms","2000"},{"max-inflight","1"},{"max-rate-hz","100"},{"burst","20"},{"max-bytes-per-second","1048576"},{"audit","allow-and-deny"},{"audit-max-rate-hz","100"},{"audit-burst","20"},{"audit-max-records","10000"},{"audit-max-bytes","1048576"}}}};config.policy.capabilities.emplace(graph.alias,std::move(graph));
            }};
            ipc::client a,b;f.connect(a);f.connect(b);
            require(a.call("action.send-goal","missing",number(5,4),5500).error==13&&native_goals==0,"unauthorized Action causes no native goal side effect");
            auto malformed{number(47,4)};require(a.call("action.send-goal","send",malformed,5500).error==22&&native_goals==0,"out of range goal rejected before side effect");
            auto sent{a.call("action.send-goal","send",number(5,4),5500)};require(!sent.error&&sent.payload.size()==8,"native Fibonacci goal accepted");auto id{ipc::get(sent.payload.data(),8)};require(id&&native_goals==1,"generated unique goal ID with actual native witness");
            auto feedback{poll(a,"action.receive-feedback","feedback",number(id))};ipc::action_sequence sequence;require(!feedback.error&&ipc::decode_action_sequence(feedback.payload,false,sequence)&&sequence.goal==id&&!sequence.values.empty(),"independently granted real native feedback");
            auto result{poll(a,"action.receive-result","result",number(id))};require(!result.error&&ipc::decode_action_sequence(result.payload,true,sequence)&&sequence.goal==id&&sequence.kind==2&&sequence.values==std::vector<std::int32_t>({0,1,1,2,3,5}),"real generated Action result exact canonical sequence");
            require(a.call("action.receive-result","result",number(id),5500).error==9,"result ID retired after delivery");
            auto long_goal{a.call("action.send-goal","send",number(30,4),5500)};require(!long_goal.error,"long native goal accepted");auto foreign{ipc::get(long_goal.payload.data(),8)};
            require(b.call("action.cancel-own-goal","own",number(foreign),5500).error==13&&native_cancels==0,"another instance cannot cancel foreign goal with own grant");
            require(b.call("action.receive-result","result",number(foreign),5500).error==13,"another instance cannot read foreign result");
            auto canceled{b.call("action.cancel-any-goal","any",number(foreign),5500)};require(!canceled.error&&canceled.payload==std::vector<std::uint8_t>{1}&&native_cancels==1,"explicit independent cancel-any permits known foreign resource goal");
            result=poll(a,"action.receive-result","result",number(foreign));require(!result.error&&ipc::decode_action_sequence(result.payload,true,sequence)&&sequence.kind==4,"actual native cancellation result");
            auto own_goal{a.call("action.send-goal","send",number(30,4),5500)};require(!own_goal.error,"own cancellation native goal");auto own_id{ipc::get(own_goal.payload.data(),8)};require(!a.call("action.cancel-own-goal","own",number(own_id),5500).error&&native_cancels==2,"separate own grant cancels own native goal");result=poll(a,"action.receive-result","result",number(own_id));require(!result.error,"own canceled result delivered");
            auto inspected{a.call("graph.inspect-action","inspect",{},2500)};require(!inspected.error&&inspected.payload.size()>=4,"real ROS2 Action graph inspection");std::string row(reinterpret_cast<char const*>(inspected.payload.data()+4),inspected.payload.size()-4);require(row=="/uwvm_actions/native\taction_tutorials_interfaces/action/Fibonacci\n","exact action graph identity");
            require(!a.call("action.advertise","advertise",{},5500).error,"independent native Action server advertised");
            auto native_client{rclcpp_action::create_client<fibonacci>(node,"/uwvm_actions/gateway")};require(native_client->wait_for_action_server(std::chrono::seconds(3)),"real native client discovers guest-backed Action server");
            rclcpp_action::Client<fibonacci>::SendGoalOptions options;options.feedback_callback=[&](auto,std::shared_ptr<fibonacci::Feedback const> feedback) { if(feedback->partial_sequence==std::vector<std::int32_t>{0,1}) { ++native_feedback; } };
            fibonacci::Goal request;request.order=5;auto future{native_client->async_send_goal(request,options)};require(future.wait_for(std::chrono::seconds(3))==std::future_status::ready,"native incoming goal accepted");auto native_handle{future.get()};require(native_handle!=nullptr,"native incoming generated goal handle");
            auto event{poll(a,"event.poll","advertise",{})};require(!event.error&&event.payload.size()==12&&ipc::get(event.payload.data()+8,4)==5,"incoming goal routed bounded canonical event");auto incoming{ipc::get(event.payload.data(),8)};
            auto reply{ipc::encode_action_sequence({incoming,1,{0,1}},true)};require(b.call("action.respond","respond",reply,5500).error==13,"foreign instance cannot answer incoming native goal");require(!a.call("action.respond","respond",reply,5500).error,"separate response grant publishes native feedback");
            for(unsigned n{};n!=100&&!native_feedback;++n) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }require(native_feedback==1,"native witness received guest server feedback");
            auto native_result{native_client->async_get_result(native_handle)};reply=ipc::encode_action_sequence({incoming,2,{0,1,1,2,3,5}},true);require(!a.call("action.respond","respond",reply,5500).error,"guest responds with generated native success");require(native_result.wait_for(std::chrono::seconds(3))==std::future_status::ready,"native Action result arrived");auto actual{native_result.get()};require(actual.code==rclcpp_action::ResultCode::SUCCEEDED&&actual.result->sequence==std::vector<std::int32_t>({0,1,1,2,3,5}),"actual native server result exact payload");
            require(a.call("action.respond","respond",reply,5500).error==9,"stale server goal cannot respond again");require(!a.call("capability.revoke","send",{},1000).error,"Action send grant revoked");require(a.call("action.send-goal","send",number(5,4),5500).error==125&&native_goals==3,"revoked Action creates no subsequent native goal");
            for(unsigned round{};round!=10;++round)
            {
                ipc::client racing;f.connect(racing);auto canceled_before{native_cancels.load()};
                auto started{racing.call("action.send-goal","send",number(30,4),5500)};require(!started.error&&started.payload.size()==8,"actual teardown race native goal accepted");
                auto live{poll(racing,"action.receive-feedback","feedback",started.payload)};require(!live.error&&!live.payload.empty(),"actual live Action feedback before concurrent teardown");
                auto begin{std::chrono::steady_clock::now()};require(!racing.call(round%2?"capability.revoke":"capability.close","send",{},1500).error,"close/revoke versus live native feedback does not deadlock");require(std::chrono::steady_clock::now()-begin<std::chrono::seconds(2),"Action teardown bounded deadline");
                for(unsigned n{};n!=200&&native_cancels==canceled_before;++n){std::this_thread::sleep_for(std::chrono::milliseconds(5));}require(native_cancels>canceled_before,"actual native goal cancelled during teardown");
                for(unsigned n{};n!=5;++n){auto late{racing.call("action.receive-feedback","feedback",started.payload,1000)};require(late.error==9&&late.payload.empty(),"closed owned goal delivers no late native feedback");std::this_thread::sleep_for(std::chrono::milliseconds(5));}racing.close();
            }
            {
                fixture late{core::version::ros2,argv[1],[&](gateway::configuration& config){config.policy=f.config.policy;config.policy.connection["node-name"]="uwvm_late_action_response";config.policy.capabilities.at("send").limits["timeout-ms"]="300";}};ipc::client delayed;late.connect(delayed);auto goals_before{native_goals.load()},cancels_before{native_cancels.load()};
                auto expired{delayed.call("action.send-goal","send",number(31,4),1500)};require(expired.error==110&&expired.payload.empty(),"delayed native goal response exceeds explicit300ms operation budget");
                for(unsigned n{};n!=300&&(native_goals==goals_before||native_cancels==cancels_before);++n){std::this_thread::sleep_for(std::chrono::milliseconds(5));}require(native_goals==goals_before+1&&native_cancels>cancels_before,"inactive late goal response is actually cancelled without lock inversion");delayed.close();
            }
            std::cout<<"PASS actual ROS2 Action10 live-feedback close/revoke races plus inactive late-response cancellation, bounded teardown and no late Guest delivery\n";
            a.close();b.close();::kill(f.gateway_pid,SIGTERM);::waitpid(f.gateway_pid,nullptr,0);f.gateway_pid=-1;
            fixture restarted{core::version::ros2,argv[1],[&](gateway::configuration& config) { config.policy=f.config.policy; }};ipc::client after_restart;restarted.connect(after_restart);
            auto fresh{after_restart.call("action.send-goal","send",number(5,4),5500)};require(!fresh.error&&fresh.payload.size()==8&&ipc::get(fresh.payload.data(),8)!=id,"Gateway restart uses fresh opaque goal identifier generation");
            require(after_restart.call("action.receive-result","result",number(id),5500).error==9&&after_restart.call("action.cancel-own-goal","own",number(id),5500).error==9,"old process goal ID cannot alias new process goal");
            result=poll(after_restart,"action.receive-result","result",fresh.payload);require(!result.error,"new process generated native goal still completes");after_restart.close();std::cout<<"PASS actual ROS2 Fibonacci goal/feedback/result, own and foreign cancellation, native server advertise/respond, exact Action graph, stale IDs including Gateway restart, denied side effects and revocation\n";
        }
        catch(...) { cleanup();throw; }cleanup();return 0;
    }
    catch(std::exception const& error) { std::cerr<<error.what()<<'\n';return 1; }
}
