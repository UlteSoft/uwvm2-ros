#include "common_ros1.h"
#include <actionlib/server/action_server.h>
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib_tutorials/FibonacciAction.h>
using namespace ros_action_probe;
int main(int argc,char** argv)
{
    try
    {
        if(argc>=3&&std::string{argv[1]}=="--deny-native")
        {
            std::string resource{argv[2]};::ros::init(argc,argv,"uwvm_ros1_goal_denial_witness",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();using action=actionlib_tutorials::FibonacciAction;using native_server=actionlib::ActionServer<action>;std::atomic<unsigned> requests{};
            native_server server{node,resource,[&](native_server::GoalHandle goal){++requests;goal.setRejected();},[](native_server::GoalHandle goal){goal.setCanceled();},false};server.start();
            std::cout<<"READY actual ROS1 generated Fibonacci request counter\n"<<std::flush;
            auto deadline{std::chrono::steady_clock::now()+std::chrono::seconds(4)};while(std::chrono::steady_clock::now()<deadline&&!requests){std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            require(!requests,"unauthorized Wasm Action goal emitted no actual generated native request");spinner.stop();::ros::shutdown();std::cout<<"PASS NO_NATIVE_ACTION_REQUESTS native_goal_requests=0\n";return 0;
        }
        if(argc>=3&&std::string{argv[1]}=="--flood-native")
        {
            std::string resource{argv[2]};::ros::init(argc,argv,"uwvm_ros1_action_flood_witness",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();std::atomic<unsigned> goals{};std::atomic<bool> wrong{};
            auto feedback{node.advertise<actionlib_tutorials::FibonacciActionFeedback>(resource+"/feedback",2,false)};auto result{node.advertise<actionlib_tutorials::FibonacciActionResult>(resource+"/result",2,false)};
            boost::function<void(actionlib_tutorials::FibonacciActionGoal::ConstPtr const&)> cb{[&](auto const& goal){++goals;if(goal->goal.order!=5){wrong=true;}}};auto incoming{node.subscribe<actionlib_tutorials::FibonacciActionGoal>(resource+"/goal",2,cb)};
            std::cout<<"READY real ROS1 Action goal quota witness\n"<<std::flush;auto deadline{std::chrono::steady_clock::now()+std::chrono::seconds(5)};
            while(std::chrono::steady_clock::now()<deadline){require(goals<=2&&!wrong,"excess or malformed goal reached actual native Action endpoint");std::this_thread::sleep_for(std::chrono::milliseconds(10));}
            require(goals==2&&!wrong,"exactly two permitted goals reached actual native Action endpoint");spinner.stop();::ros::shutdown();std::cout<<"PASS BOUNDED_NATIVE_GOALS native_goal_requests=2 excess_goal_requests=0\n";return 0;
        }
        if(argc>=3&&std::string{argv[1]}=="--active-native")
        {
            std::string resource{argv[2]};::ros::init(argc,argv,"uwvm_ros1_cancel_denial_witness",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();using action=actionlib_tutorials::FibonacciAction;std::atomic<bool> active{};std::atomic<unsigned> cancelled{};std::unique_ptr<actionlib::SimpleActionServer<action>> server;
            server=std::make_unique<actionlib::SimpleActionServer<action>>(node,resource,[&](actionlib_tutorials::FibonacciGoal::ConstPtr const& goal){require(goal->order==5,"native cancel witness exact goal");active=true;for(unsigned n{};n!=300;++n){if(server->isPreemptRequested()){++cancelled;server->setPreempted();return;}std::this_thread::sleep_for(std::chrono::milliseconds(10));}actionlib_tutorials::FibonacciResult response;response.sequence=expected;server->setSucceeded(response);},false);server->start();actionlib::SimpleActionClient<action> client(node,resource,true);require(client.waitForServer(::ros::Duration{3}),"native self Action server discovery");actionlib_tutorials::FibonacciGoal goal;goal.order=5;client.sendGoal(goal);for(unsigned n{};n!=300&&!active;++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(active,"actual native goal active before malicious Guest");std::cout<<"READY actual ROS1 native goal active\n"<<std::flush;
            require(client.waitForResult(::ros::Duration{5})&&client.getState()==actionlib::SimpleClientGoalState::SUCCEEDED&&client.getResult()->sequence==expected&&!cancelled,"denied own/any cancel leaves actual native goal intact");spinner.stop();::ros::shutdown();std::cout<<"PASS actual ROS1 unauthorized Action cancellation emitted no native cancel\n";return 0;
        }
        if(argc>=3&&(std::string{argv[1]}=="--serve-native"||std::string{argv[1]}=="--call-guest"))
        {
            auto mode{std::string{argv[1]}},resource{std::string{argv[2]}};::ros::init(argc,argv,"uwvm_ros1_action_network_witness",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();
            using action=actionlib_tutorials::FibonacciAction;
            if(mode=="--serve-native")
            {
                std::atomic<bool> done{},wrong{};std::unique_ptr<actionlib::SimpleActionServer<action>> server;
                server=std::make_unique<actionlib::SimpleActionServer<action>>(node,resource,[&](actionlib_tutorials::FibonacciGoal::ConstPtr const& goal)
                {if(goal->order!=5){wrong=true;server->setAborted();return;}actionlib_tutorials::FibonacciFeedback feedback;feedback.sequence=expected;server->publishFeedback(feedback);std::this_thread::sleep_for(std::chrono::milliseconds(100));actionlib_tutorials::FibonacciResult result;result.sequence=expected;server->setSucceeded(result);done=true;},false);server->start();
                for(unsigned n{};n!=1200&&!done&&!wrong;++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(done&&!wrong,"actual native Action receives exact Wasm order");std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            else
            {actionlib::SimpleActionClient<action> client(node,resource,true);require(client.waitForServer(::ros::Duration{12}),"native discovers Wasm Action server");actionlib_tutorials::FibonacciGoal goal;goal.order=5;std::atomic<bool> feedback_seen{},feedback_wrong{};client.sendGoal(goal,{}, {},[&](actionlib_tutorials::FibonacciFeedback::ConstPtr const& feedback){feedback_seen=true;if(feedback->sequence!=expected){feedback_wrong=true;}});require(client.waitForResult(::ros::Duration{8})&&client.getState()==actionlib::SimpleClientGoalState::SUCCEEDED&&client.getResult()->sequence==expected,"native receives exact Wasm Action result");for(unsigned n{};n!=50&&!feedback_seen;++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(feedback_seen&&!feedback_wrong,"native receives exact Wasm Action server feedback");}
            spinner.stop();::ros::shutdown();std::cout<<"PASS actual ROS1 Wasm/native Action exact order/result\n";return 0;
        }
        require(argc==2,"usage Action probe gateway");::ros::init(argc,argv,"uwvm_native_action_probe",::ros::init_options::NoSigintHandler|::ros::init_options::NoRosout);::ros::NodeHandle node;::ros::AsyncSpinner spinner{2};spinner.start();
        using action=actionlib_tutorials::FibonacciAction;using server=actionlib::SimpleActionServer<action>;std::atomic<unsigned> seen{},cancelled{};std::unique_ptr<server> native;
        native=std::make_unique<server>(node,"/uwvm_action/native",[&](actionlib_tutorials::FibonacciGoal::ConstPtr const& goal)
        {
            ++seen;if(goal->order==6){for(unsigned n{};n!=300&&!native->isPreemptRequested();++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}if(native->isPreemptRequested()){++cancelled;native->setPreempted();return;}}
            actionlib_tutorials::FibonacciFeedback feedback;feedback.sequence=expected;native->publishFeedback(feedback);std::this_thread::sleep_for(std::chrono::milliseconds(100));actionlib_tutorials::FibonacciResult result;result.sequence=expected;native->setSucceeded(result);
        },false);native->start();
        fixture f{core::version::ros1,argv[1],configure};ipc::client a,b;f.connect(a);f.connect(b);
        auto before{seen.load()};require(a.call("action.send-goal","goal",order(11),1000).error==22,"order outside explicit bound denied");std::this_thread::sleep_for(std::chrono::milliseconds(50));require(seen==before,"denied Action causes no native goal");
        auto id{send(a,5)};require(b.call("action.receive-result","result",id_bytes(id),1000).error==9,"another instance cannot read owned goal");
        auto feedback{poll(a,"action.receive-feedback","feedback",id_bytes(id))};ipc::action_sequence parsed;require(!feedback.error&&ipc::decode_action_sequence(feedback.payload,false,parsed)&&parsed.goal==id&&parsed.values==expected,"actual generated Action feedback exact");
        auto result{poll(a,"action.receive-result","result",id_bytes(id))};require(!result.error&&ipc::decode_action_sequence(result.payload,true,parsed)&&parsed.goal==id&&parsed.kind==2&&parsed.values==expected,"actual generated Action result exact");require(a.call("action.receive-result","result",id_bytes(id),1000).error==9,"consumed Action ID stale");
        auto foreign_before{seen.load()};auto foreign{send(a,6)};for(unsigned n{};n!=300&&seen==foreign_before;++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(seen>foreign_before,"native foreign goal active");require(b.call("action.cancel-own-goal","own",id_bytes(foreign),1000).error==13,"own-cancel cannot cancel another instance");
        require(!b.call("action.cancel-any-goal","any",id_bytes(foreign),1000).error,"independent any capability cancels exact foreign goal");result=poll(a,"action.receive-result","result",id_bytes(foreign));require(!result.error&&ipc::decode_action_sequence(result.payload,true,parsed)&&parsed.kind==4,"native foreign cancellation result");
        auto owned_before{seen.load()};auto owned{send(a,6)};for(unsigned n{};n!=300&&seen==owned_before;++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(seen>owned_before,"native owned goal active");require(!a.call("action.cancel-own-goal","own",id_bytes(owned),1000).error,"owner cancels own goal");result=poll(a,"action.receive-result","result",id_bytes(owned));require(!result.error&&ipc::decode_action_sequence(result.payload,true,parsed)&&parsed.kind==4,"native owned cancellation result");require(cancelled>=2,"actual native cancellation callbacks");
        actionlib::SimpleActionClient<action> external(node,"/uwvm_action/native",true);require(external.waitForServer(::ros::Duration{3}),"external native Action ready");actionlib_tutorials::FibonacciGoal external_goal;external_goal.order=6;auto external_before{seen.load()};external.sendGoal(external_goal);for(unsigned n{};n!=300&&seen==external_before;++n){std::this_thread::sleep_for(std::chrono::milliseconds(10));}require(seen>external_before,"external native goal active");
        require(a.call("action.cancel-own-goal","own",id_bytes(0),1000).error==22,"own zero ID cannot imply cancel-all");require(!b.call("action.cancel-any-goal","any",id_bytes(0),1500).error&&external.waitForResult(::ros::Duration{3})&&external.getState()==actionlib::SimpleClientGoalState::PREEMPTED,"explicit any zero cancels actual external native goal");
        require(!a.call("action.advertise","server",{},1000).error,"bounded generated Action server creation");
        actionlib::SimpleActionClient<action> caller(node,"/uwvm_action/guest",true);require(caller.waitForServer(::ros::Duration{3}),"native Actionlib discovers Wasm server");actionlib_tutorials::FibonacciGoal goal;goal.order=5;caller.sendGoal(goal);
        auto event{poll(a,"event.poll","server",{})};require(!event.error&&event.payload.size()==12&&ipc::get(event.payload.data()+8,4)==5,"actual incoming Action goal canonical");auto server_id{ipc::get(event.payload.data(),8)};
        auto bytes{ipc::encode_action_sequence({server_id,2,expected},true)};auto wrong{bytes};wrong[0]^=0x80;require(a.call("action.respond","responder",wrong,1000).error==9,"unknown server goal rejected");require(a.call("action.respond","missing",bytes,1000).error==13,"ungranted response rejected");
        require(a.call("action.respond","responder",ipc::encode_action_sequence({server_id,4,expected},true),1000).error==13,"unsolicited cancellation response rejected");require(!caller.waitForResult(::ros::Duration{.1}),"denied cancellation leaves actual native goal active");
        require(!a.call("action.respond","responder",bytes,1000).error,"authorized generated Action response");require(caller.waitForResult(::ros::Duration{3})&&caller.getState()==actionlib::SimpleClientGoalState::SUCCEEDED&&caller.getResult()->sequence==expected,"native Actionlib receives exact server result");
        require(!a.call("capability.revoke","server",{},1000).error&&a.call("event.poll","server",{},1000).error==125,"Action server revocation closes authority");
        a.close();b.close();::kill(f.gateway_pid,SIGKILL);::waitpid(f.gateway_pid,nullptr,0);f.gateway_pid=-1;
        fixture restarted{core::version::ros1,argv[1],configure};ipc::client fresh;restarted.connect(fresh);auto fresh_id{send(fresh,5)};require(fresh_id!=id,"opaque GoalID is fresh across Gateway restart");require(fresh.call("action.receive-result","result",id_bytes(id),1000).error==9,"old GoalID after Gateway restart cannot alias a new goal");auto fresh_result{poll(fresh,"action.receive-result","result",id_bytes(fresh_id))};require(!fresh_result.error&&ipc::decode_action_sequence(fresh_result.payload,true,parsed)&&parsed.values==expected,"new Gateway independent actual goal result");fresh.close();
        spinner.stop();::ros::shutdown();std::cout<<"PASS ROS1 real generated Action client/server, exact feedback/result, own/foreign cancel ownership, denied no-goal, stale IDs across restart, native cancel-all and revoke\n";return 0;
    }
    catch(std::exception const& e){std::cerr<<e.what()<<'\n';return 1;}
}
