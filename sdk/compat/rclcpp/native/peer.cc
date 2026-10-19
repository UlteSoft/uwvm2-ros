/** Actual ROS2 native one-shot complete String peer, not a generated stub.
 * The owner must qualify actual installed SDK/compiler/fullMD/loaded mappings,
 * hold the initial SIGSTOP, then release I/M/P/F gates independently. This
 * executable never talks to a robot command topic and never changes policy.
 */
#include "tests/ros_custom_types/bounded-v2/native_protocol.h"
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <csignal>
#include <thread>
int main(int argc,char**argv){
 try{
  if(argc!=2||(std::string_view{argv[1]}!="publish"&&std::string_view{argv[1]}!="subscribe"))return 22;
  bool publish=std::string_view{argv[1]}=="publish";if(::raise(SIGSTOP))return 22;bounded_native_io::gate('I');
  rclcpp::init(argc,argv);struct Shutdown{~Shutdown(){if(rclcpp::ok())rclcpp::shutdown();}}cleanup;auto node=std::make_shared<rclcpp::Node>("uwvm_compat_native_peer");
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub; rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub;
  unsigned deliveries{},mismatches{};
  if(publish)pub=node->create_publisher<std_msgs::msg::String>("/uwvm_rclcpp_compat/native_to_guest",10);
  else sub=node->create_subscription<std_msgs::msg::String>("/uwvm_rclcpp_compat/guest_to_native",10,[&](std_msgs::msg::String const& value){++deliveries;if(value.data!="Hello, world! 0")++mismatches;});
  bounded_native_io::gate('M');auto end=std::chrono::steady_clock::now()+std::chrono::seconds(20);
  while((publish?pub->get_subscription_count():sub->get_publisher_count())==0){if(std::chrono::steady_clock::now()>=end)return 110;rclcpp::spin_some(node);std::this_thread::sleep_for(std::chrono::milliseconds(10));}
  bounded_native_io::print(STDOUT_FILENO,"NATIVE_COMPAT_MATCHED\n");bounded_native_io::gate('P');
  if(publish){std_msgs::msg::String value;value.data="Hello, world! 0";pub->publish(value);}
  else while(deliveries==0){if(std::chrono::steady_clock::now()>=end)return 110;rclcpp::spin_some(node);std::this_thread::sleep_for(std::chrono::milliseconds(10));}
  bounded_native_io::gate('F');pub.reset();sub.reset();node.reset();rclcpp::shutdown();
  if(!publish&&(deliveries!=1||mismatches))return 22;
  bounded_native_io::print(STDOUT_FILENO,"PASS_NATIVE_RCLCPP_STRING_PEER deliveries=",deliveries," mismatches=",mismatches,'\n');return 0;
 }catch(std::exception const&){if(rclcpp::ok())rclcpp::shutdown();return 22;}
}
