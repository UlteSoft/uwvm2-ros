/* SPDX-License-Identifier: Apache-2.0
 * Real independent native publisher source, UNEXECUTED. No raw7 model or Guest
 * rmw_uwvm is linked into this original native ROS SDK process. Root must admit
 * its actual SDK/type/hash, isolated DDS domain/network and finite process owner.
 */
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <memory>
#include <cstdio>
#include <thread>
#include <string>
#include <string_view>
int main(int argc,char**argv){
 if(argc!=3||std::string_view{argv[1]}!="--owned-native-publisher"||!argv[2][0]||argv[2][0]!='/')return 13;
 rclcpp::init(argc,argv);int code=0;{
 auto node=std::make_shared<rclcpp::Node>("uwvm_raw7_native_publisher");
 auto qos=rclcpp::QoS(rclcpp::KeepLast(4)).reliable().durability_volatile();
 auto publisher=node->create_publisher<std_msgs::msg::String>(argv[2],qos);
 auto until=std::chrono::steady_clock::now()+std::chrono::seconds(10);
 // Real discovery is fixture coordination only, never Guest authority or a
 // replacement for same-endpoint native QoS/metadata validation.
 while(!publisher->get_subscription_count()&&std::chrono::steady_clock::now()<until)std::this_thread::sleep_for(std::chrono::milliseconds(1));
 if(!publisher->get_subscription_count())code=110;
 else{std_msgs::msg::String value;value.data=std::string{"wasm\0ro",7};auto gid=publisher->get_gid();
 std::fputs("NATIVE_PUBLISHER_GID ",stdout);for(auto b:gid.data)std::printf("%02x",b);std::fputc('\n',stdout);
 // Finite repeated publications are distinct effects, not a retry of a service
 // admission/unknown ACK. The Guest consumes exactly its one accepted record.
 for(unsigned i=0;i<100;++i){publisher->publish(value);std::this_thread::sleep_for(std::chrono::milliseconds(1));}}
 publisher.reset();node.reset();}
 rclcpp::shutdown();return code;
}
