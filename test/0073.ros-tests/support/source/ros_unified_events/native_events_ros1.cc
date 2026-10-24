/** @file native_events_ros1.cc
 * @brief Independent TCPROS Topic/Service/Action effects for unified events.
 *
 * Start with the controller's owned Master11386 and ROS_IP=127.0.0.1.
 * I initializes /uwvm_events/events_peer. T sends exactly one matched String;
 * S starts one real SetBool call to the Guest server. F emits one feedback;
 * E permits one result only after the Guest proves feedback receipt. Q verifies
 * all exact native effects remain unchanged for at least two seconds.
 * All worker threads join before native objects are destroyed.
 */
#include "native_events_common.h"
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_srvs/SetBool.h>
#include <actionlib/server/simple_action_server.h>
#include <actionlib_tutorials/FibonacciAction.h>
#include <mutex>

int main(int argc,char** argv)
{
    using namespace unified_native;
    try
    {
        require(argc==1&&std::getenv("ROS_MASTER_URI")&&std::string_view{std::getenv("ROS_MASTER_URI")}=="http://127.0.0.1:11386"&&
                std::getenv("ROS_IP")&&std::string_view{std::getenv("ROS_IP")}=="127.0.0.1");gate('I');
        ros::M_string remappings{{"__ns","/uwvm_events"}};
        ros::init(remappings,"events_peer",ros::init_options::NoSigintHandler);
        require(ros::this_node::getName()=="/uwvm_events/events_peer");(void)argv;
        ros::NodeHandle node{"/uwvm_events"};counters counts;
        auto pub{node.advertise<std_msgs::String>("topic",1,false)};
        auto callback=[&](std_srvs::SetBool::Request& request,std_srvs::SetBool::Response& response)
        {if(++counts.service_calls!=1||!request.data) {counts.failed=true;return false;}response.success=true;response.message="native-response";return true;};
        auto service{node.advertiseService<std_srvs::SetBool::Request,std_srvs::SetBool::Response>("native",callback)};
        actionlib::SimpleActionServer<actionlib_tutorials::FibonacciAction> action{node,"action",false};
        std::mutex mutex;bool accepted{};
        action.registerGoalCallback([&]{auto goal{action.acceptNewGoal()};std::lock_guard<std::mutex> lock{mutex};
            if(++counts.goals!=1||!goal||goal->order!=5) {counts.failed=true;return;}accepted=true;});
        action.registerPreemptCallback([&]{counts.failed=true;});action.start();
        ros::AsyncSpinner spinner{1};spinner.start();std::thread caller;
        auto cleanup=[&]{if(caller.joinable()) {caller.join();}spinner.stop();ros::shutdown();};
        try
        {
            emit("READY_NATIVE_SEVEN_EVENTS",1);gate('T');until([&]{return pub.getNumSubscribers()==1;});
            std_msgs::String text;text.data="native-topic";pub.publish(text);emit("TOPIC_NATIVE_SENT",1);
            gate('S');caller=std::thread{[&]{try {auto client{node.serviceClient<std_srvs::SetBool>("guest",false)};
                require(client.waitForExistence(ros::Duration(8)));std_srvs::SetBool request;request.request.data=true;
                require(client.call(request)&&request.response.success&&request.response.message=="guest-response");++counts.guest_responses;
            }catch(...) {counts.failed=true;}}};emit("SERVICE_NATIVE_STARTED",1);
            gate('F');until([&]{std::lock_guard<std::mutex> lock{mutex};return accepted;});require(!counts.failed);
            actionlib_tutorials::FibonacciFeedback feedback;feedback.sequence={0,1,1,2,3,5};action.publishFeedback(feedback);++counts.feedback;
            emit("FEEDBACK_NATIVE_SENT",1);gate('E');require(!counts.failed);
            actionlib_tutorials::FibonacciResult result;result.sequence=feedback.sequence;action.setSucceeded(result);++counts.results;
            emit("ACTION_NATIVE_SENT",1);gate('Q');until([&]{return counts.exact();});quiet(counts,1);cleanup();return 0;
        }
        catch(...) {cleanup();throw;}
    }
    catch(...) {io::diagnostic("FAIL_NATIVE_SEVEN_EVENTS","ROS1 fixed contract");return 1;}
}
