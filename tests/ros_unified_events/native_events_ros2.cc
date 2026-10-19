/** @file native_events_ros2.cc
 * @brief Independent DDS Topic/Service/Action effects for unified events.
 *
 * Uses a separate process/context, domain206 and rmw_fastrtps_cpp. The actual
 * generated Action feedback writer waits for a matched DDS reader; the
 * official Action server owns Goal acceptance and Result service delivery.
 * See native_events_ros1.cc for the finite I/T/S/F/E/Q control protocol. No IPC
 * client or Guest authorization exists in this peer, and no QoS override is
 * installed in production. The executor thread is joined on every exit.
 */
#include "native_events_common.h"
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <action_tutorials_interfaces/action/fibonacci.hpp>
#include <mutex>

int main(int argc,char** argv)
{
    using namespace unified_native;
    using action_type=action_tutorials_interfaces::action::Fibonacci;
    using goal_type=rclcpp_action::ServerGoalHandle<action_type>;
    try
    {
        require(argc==1&&std::getenv("ROS_DOMAIN_ID")&&std::string_view{std::getenv("ROS_DOMAIN_ID")}=="206"&&
                std::getenv("RMW_IMPLEMENTATION")&&std::string_view{std::getenv("RMW_IMPLEMENTATION")}=="rmw_fastrtps_cpp");
        gate('I');rclcpp::init(argc,argv);counters counts;
        auto node{std::make_shared<rclcpp::Node>("events_peer","/uwvm_events")};
        std::size_t domain{};require(rcl_context_get_domain_id(node->get_node_base_interface()->get_context()->get_rcl_context().get(),&domain)==RCL_RET_OK&&
            domain==206&&std::string_view{node->get_fully_qualified_name()}=="/uwvm_events/events_peer"&&
            std::string_view{rmw_get_implementation_identifier()}=="rmw_fastrtps_cpp");
        auto qos{rclcpp::QoS{rclcpp::KeepLast{16}}.reliable().durability_volatile()};
        auto pub{node->create_publisher<std_msgs::msg::String>("topic",qos)};
        auto service{node->create_service<std_srvs::srv::SetBool>("native",[&](std::shared_ptr<std_srvs::srv::SetBool::Request> request,
            std::shared_ptr<std_srvs::srv::SetBool::Response> response)
        {if(++counts.service_calls!=1||!request->data) {counts.failed=true;}response->success=true;response->message="native-response";})};
        std::mutex mutex;std::shared_ptr<goal_type> accepted;
        auto server{rclcpp_action::create_server<action_type>(node,"/uwvm_events/action",
            [&](auto const&,auto goal) {if(++counts.goals!=1||goal->order!=5) {counts.failed=true;return rclcpp_action::GoalResponse::REJECT;}
                return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;},
            [&](auto){counts.failed=true;return rclcpp_action::CancelResponse::REJECT;},
            [&](auto goal){std::lock_guard<std::mutex> lock{mutex};accepted=goal;})};
        using feedback_type=action_type::Impl::FeedbackMessage;
        auto feedback_pub{node->create_publisher<feedback_type>("/uwvm_events/action/_action/feedback",qos)};
        auto client{node->create_client<std_srvs::srv::SetBool>("guest")};
        rclcpp::executors::SingleThreadedExecutor executor;executor.add_node(node);std::thread spinner;
        auto cleanup=[&]{executor.cancel();if(spinner.joinable()) {spinner.join();}rclcpp::shutdown();};
        try
        {
            spinner=std::thread{[&]{try {executor.spin();}catch(...) {counts.failed=true;}}};
            emit("READY_NATIVE_SEVEN_EVENTS",2);gate('T');until([&]{return pub->get_subscription_count()==1;});
            std_msgs::msg::String text;text.data="native-topic";pub->publish(text);emit("TOPIC_NATIVE_SENT",2);
            gate('S');require(client->wait_for_service(std::chrono::seconds(8)));
            auto request{std::make_shared<std_srvs::srv::SetBool::Request>()};request->data=true;
            auto future{client->async_send_request(request,[&](rclcpp::Client<std_srvs::srv::SetBool>::SharedFuture response) {try {auto value{response.get()};
                if(!value->success||value->message!="guest-response"||++counts.guest_responses!=1) {counts.failed=true;}
            }catch(...) {counts.failed=true;}})};(void)future;emit("SERVICE_NATIVE_STARTED",2);
            gate('F');until([&]{std::lock_guard<std::mutex> lock{mutex};return bool(accepted);});
            until([&]{return feedback_pub->get_subscription_count()==1;});require(!counts.failed);
            std::shared_ptr<goal_type> goal;{std::lock_guard<std::mutex> lock{mutex};goal=accepted;}
            feedback_type feedback;feedback.goal_id.uuid=goal->get_goal_id();feedback.feedback.partial_sequence={0,1,1,2,3,5};
            feedback_pub->publish(feedback);++counts.feedback;
            // DDS feedback and result use independent channels. Retain the
            // official client goal until the Guest confirms actual feedback.
            emit("FEEDBACK_NATIVE_SENT",2);gate('E');require(!counts.failed);
            auto result{std::make_shared<action_type::Result>()};result->sequence=feedback.feedback.partial_sequence;goal->succeed(result);++counts.results;
            emit("ACTION_NATIVE_SENT",2);gate('Q');until([&]{return counts.exact();});quiet(counts,2);cleanup();return 0;
        }
        catch(...) {cleanup();throw;}
    }
    catch(...) {io::diagnostic("FAIL_NATIVE_SEVEN_EVENTS","ROS2 fixed contract");return 1;}
}
