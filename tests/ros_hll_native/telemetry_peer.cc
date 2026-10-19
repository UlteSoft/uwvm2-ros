// Independent native ROS2 witness for the language SDK telemetry application.
// Native test process only; never linked into a Guest or used as a Host import.
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <thread>

namespace {
using clock_type=std::chrono::steady_clock;
using gid_type=std::array<unsigned char,RMW_GID_STORAGE_SIZE>;
std::string hex(gid_type const& gid) {
  char const alphabet[]="0123456789abcdef";std::string out;
  for(auto b:gid) {out+=alphabet[b>>4];out+=alphabet[b&15];}return out;
}
void require(bool value,char const* message) {if(!value)throw std::runtime_error(message);}
void marker(char const* phase,std::string const& ns,gid_type const& gid,unsigned warmup,unsigned delivered) {
  std::printf("%s namespace=%s gateway=moonbit_gateway gid=%s warmup=%u delivered=%u\n",
      phase,ns.c_str(),hex(gid).c_str(),warmup,delivered);std::fflush(stdout);
}
}
int main(int argc,char** argv) {
  try {
    require(argc==2,"usage: telemetry-peer <explicit-namespace>");std::string ns{argv[1]};
    require(ns.size()>1&&ns.front()=='/'&&ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_")==std::string::npos,"explicit fixture namespace");
    rclcpp::init(0,nullptr);require(std::string{rmw_get_implementation_identifier()}=="rmw_fastrtps_cpp","actual selected RMW");
    rclcpp::NodeOptions options;options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
    auto node=std::make_shared<rclcpp::Node>("moonbit_native_peer",ns,options);
    rclcpp::QoS qos{rclcpp::KeepLast(4)};qos.reliable();qos.durability_volatile();
    unsigned warmup{},delivered{};bool bad{},matched{};gid_type expected{},received{};
    auto subscription=node->create_subscription<std_msgs::msg::String>(ns+"/telemetry",qos,
        [&](std_msgs::msg::String::ConstSharedPtr msg,rclcpp::MessageInfo const& info) {
          auto const& gid=info.get_rmw_message_info().publisher_gid;
          if(!gid.implementation_identifier||std::string{gid.implementation_identifier}!="rmw_fastrtps_cpp") {bad=true;return;}
          if(msg->data=="moonbit-telemetry-ready") {++warmup;if(warmup>1)bad=true;}
          else if(msg->data=="bounded MoonBit telemetry") {
            ++delivered;std::copy(gid.data,gid.data+RMW_GID_STORAGE_SIZE,received.begin());
            if(!matched||delivered!=1||received!=expected)bad=true;
          } else bad=true;
        });
    marker("READY_NATIVE_TELEMETRY",ns,expected,0,0);
    auto deadline=clock_type::now()+std::chrono::seconds(25);auto quiet=deadline;
    while(clock_type::now()<deadline) {
      rclcpp::spin_some(node);require(!bad,"unexpected/duplicate native message or writer identity");
      if(!matched&&subscription->get_publisher_count()==1) {
        auto endpoints=node->get_publishers_info_by_topic(ns+"/telemetry");
        // A matched DDS endpoint can be visible before the ROS graph metadata
        // is ready. Keep the same finite deadline and admit no P gate until
        // every required identity field is present and exactly matches.
        if(endpoints.size()==1&&endpoints.front().node_name()=="moonbit_gateway"&&
            endpoints.front().node_namespace()==ns&&endpoints.front().topic_type()=="std_msgs/msg/String") {
          auto const& endpoint=endpoints.front();
          require(endpoint.node_name()=="moonbit_gateway"&&endpoint.node_namespace()==ns&&endpoint.topic_type()=="std_msgs/msg/String","native graph identity differs");
          expected=endpoint.endpoint_gid();require(std::any_of(expected.begin(),expected.end(),[](auto b){return b!=0;}),"actual nonzero publisher GID");
          auto const& actual=endpoint.qos_profile().get_rmw_qos_profile();
          require(actual.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&actual.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE,"actual selected native QoS");
          matched=true;marker("MATCHED_NATIVE_TELEMETRY",ns,expected,warmup,0);
        }
      }
      if(delivered==1&&quiet==deadline) {quiet=clock_type::now()+std::chrono::seconds(2);marker("DELIVERED_NATIVE_TELEMETRY",ns,expected,warmup,delivered);}
      if(delivered==1&&clock_type::now()>=quiet&&subscription->get_publisher_count()==0) {
        marker("PASS_NATIVE_TELEMETRY",ns,expected,warmup,delivered);subscription.reset();node.reset();rclcpp::shutdown();return 0;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    throw std::runtime_error("actual match/delivery/quiet/endpoint-retirement deadline");
  } catch(std::exception const& error) {std::fprintf(stderr,"FAIL_NATIVE_TELEMETRY %s\n",error.what());return 1;}
}
