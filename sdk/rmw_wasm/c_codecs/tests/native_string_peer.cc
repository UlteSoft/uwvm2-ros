/* SPDX-License-Identifier: Apache-2.0
 * Independent ORIGINAL native peer; does not link this Registry, rmw_uwvm or
 * any Guest raw transport. C++ here is solely the external native counterpart,
 * NEVER a companion used to encode/decode a Guest C object. UNEXECUTED.
 */
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include <string_view>
#include <cstdio>
#include <exception>
int main(int argc,char**argv){
 if(argc!=4||std::string_view(argv[1])!="--owned-native-string"||(std::string_view(argv[2])!="publish"&&std::string_view(argv[2])!="subscribe")||argv[3][0]!='/')return 22;
 bool publish=std::string_view(argv[2])=="publish";rclcpp::init(argc,argv);int code=110;
 try{auto node=std::make_shared<rclcpp::Node>("uwvm_pure_c_external_string_peer");auto qos=rclcpp::QoS(rclcpp::KeepLast(4)).reliable().durability_volatile();
 auto until=std::chrono::steady_clock::now()+std::chrono::seconds(10);
 if(publish){auto endpoint=node->create_publisher<std_msgs::msg::String>(argv[3],qos);
 while(!endpoint->get_subscription_count()&&std::chrono::steady_clock::now()<until)std::this_thread::sleep_for(std::chrono::milliseconds(1));
 if(endpoint->get_subscription_count()){std_msgs::msg::String value;value.data=std::string("wasm\0ro",7);auto gid=endpoint->get_gid();std::fputs("NATIVE_STRING_GID ",stdout);for(auto b:gid.data)std::printf("%02x",b);std::putchar('\n');
 /* Bounded distinct topic effects, no service replay/unknown-ACK retry. */
 for(unsigned n=0;n<100&&std::chrono::steady_clock::now()<until;++n){endpoint->publish(value);std::this_thread::sleep_for(std::chrono::milliseconds(1));}code=0;}
 endpoint.reset();
 }else{bool received=false,valid=false;auto endpoint=node->create_subscription<std_msgs::msg::String>(argv[3],qos,[&](std_msgs::msg::String::ConstSharedPtr value){received=true;valid=value->data==std::string("wasm\0ro",7);});
 while(!received&&std::chrono::steady_clock::now()<until){rclcpp::spin_some(node);std::this_thread::sleep_for(std::chrono::milliseconds(1));}if(received)code=valid?0:22;endpoint.reset();}
 node.reset();}catch(std::exception const&e){std::fprintf(stderr,"Native String peer exception: %s\n",e.what());code=5;}catch(...){code=5;}
 rclcpp::shutdown();if(!code)std::puts("PASS_ORIGINAL_NATIVE_STRING_PEER_NOT_GUEST_ACCEPTANCE");return code;
}
