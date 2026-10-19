/** Migration lesson, NOT the unchanged official minimal_client/main.cpp.
 * The Root driver must independently observe actual native service readiness
 * and only then release the first R. No function here claims native discovery.
 * The subsequent request/completion is the REAL additive Guest ABI, never a
 * synchronous service_call or a local ready flag masquerading as a response.
 */
#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/srv/add_two_ints.hpp>
int main(){
    rclcpp::init(0,static_cast<char const*const*>(nullptr));
    auto node=rclcpp::Node::make_shared("minimal_client");
    auto client=node->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");
    auto selection=roswasm_rclcpp::selected_configuration();
    rclcpp::detail::checked(selection->step(selection->step_context)); // Root's explicit readiness gate.
    auto request=std::make_shared<example_interfaces::srv::AddTwoInts::Request>();request->a=41;request->b=1;
    auto future=client->async_send_request(request);
    auto outcome=rclcpp::spin_until_future_complete(node,future);
    if(outcome!=rclcpp::FutureReturnCode::SUCCESS){client->remove_pending_request(future);rclcpp::detail::fail(outcome==rclcpp::FutureReturnCode::TIMEOUT?110:125);}
    auto response=future.get();if(response->sum!=42)rclcpp::detail::fail(22);
    RCLCPP_INFO(node->get_logger(),"result of %" PRId64 " + %" PRId64 " = %" PRId64,request->a,request->b,response->sum);
    auto close=client->close();rclcpp::detail::checked(close);client.reset();node.reset();rclcpp::shutdown();
    rclcpp::detail::checked(roswasm_rclcpp_cleanup_error());return 0;
}
