// Independent native ROS2 witness for Go/Zig/Kotlin/F#/VB Wasm applications.
// Require exact graph/QoS/writer/value identity, two seconds quiet and retirement.
// Native test process only; never linked into a Guest or used as a Host import.
#include <cstring>
#include <rclcpp/rclcpp.hpp>
#include <rmw/rmw.h>
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <array>
#include <chrono>
#include <fast_io.h>
#include <fast_io_dsal/string.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <poll.h>

namespace {
using clock_type=std::chrono::steady_clock;
using gid_type=std::array<unsigned char,RMW_GID_STORAGE_SIZE>;
fast_io::string hex(gid_type const& gid) {
  fast_io::string text;text.reserve(gid.size()*2);
  fast_io::ostring_ref_fast_io output{std::addressof(text)};
  for(auto byte:gid)fast_io::print(output,fast_io::mnp::right(fast_io::mnp::hex(static_cast<unsigned>(byte)),2,'0'));
  return text;
}
void require(bool value,char const* message) {if(!value)throw std::runtime_error(message);}
void marker(char const* phase,std::string const& ns,gid_type const& gid,unsigned warmup,unsigned delivered) {
  // The owner needs live markers even when stdout is a regular evidence file.
  // Select descriptor output explicitly instead of inheriting C stdio buffering.
  fast_io::io::println(fast_io::posix_stdout(),fast_io::mnp::os_c_str(phase)," namespace=",ns," gateway=roswasm_language_gateway gid=",hex(gid)," warmup=",warmup," delivered=",delivered);
}
}
int main(int argc,char** argv) {
  try {
    require(argc==2||(argc==3&&std::strcmp(argv[2],"absent")==0),"usage: telemetry-peer <explicit-namespace> [absent]");std::string ns{argv[1]};
    bool const absent{argc==3};bool absence_credit{};
    require(ns.size()>1&&ns.front()=='/'&&ns.find_first_not_of("/abcdefghijklmnopqrstuvwxyz0123456789_")==std::string::npos,"explicit fixture namespace");
    rclcpp::init(0,nullptr);require(std::string{rmw_get_implementation_identifier()}=="rmw_fastrtps_cpp","actual selected RMW");
    rclcpp::NodeOptions options;options.enable_rosout(false);options.start_parameter_services(false);options.start_parameter_event_publisher(false);
    auto node=std::make_shared<rclcpp::Node>("roswasm_language_native_peer",ns,options);
    rclcpp::QoS qos{rclcpp::KeepLast(4)};qos.reliable();qos.durability_volatile();
    unsigned warmup{},delivered{};bool bad{},matched{};gid_type expected{},received{};
    auto subscription=node->create_subscription<std_msgs::msg::String>(ns+"/telemetry",qos,
        [&](std_msgs::msg::String::ConstSharedPtr msg,rclcpp::MessageInfo const& info) {
          auto const& gid=info.get_rmw_message_info().publisher_gid;
          if(!gid.implementation_identifier||std::string{gid.implementation_identifier}!="rmw_fastrtps_cpp") {bad=true;return;}
          if(msg->data=="wasmros-telemetry-ready") {++warmup;if(warmup>1)bad=true;}
          else if(msg->data=="bounded WasmROS telemetry") {
            ++delivered;std::copy(gid.data,gid.data+RMW_GID_STORAGE_SIZE,received.begin());
            if(!matched||delivered!=1||received!=expected)bad=true;
          } else bad=true;
        });
    marker("READY_NATIVE_TELEMETRY",ns,expected,0,0);
    auto deadline=clock_type::now()+std::chrono::seconds(25);auto quiet=deadline;bool announced{};
    while(clock_type::now()<deadline) {
      rclcpp::spin_some(node);require(!bad,"unexpected/duplicate native message or writer identity");
      if(absent) {
        require(!warmup&&!delivered&&subscription->get_publisher_count()==0&&node->get_publishers_info_by_topic(ns+"/telemetry").empty(),"unexpected publisher in zero-resource fixture");
        if(!absence_credit) {
          pollfd input{0,POLLIN,0};auto ready=::poll(&input,1,0);
          require(ready>=0&&!(input.revents&(POLLERR|POLLNVAL)),"absence input poll");
          if(ready) {
            require(input.revents&POLLIN,"absence input EOF");char credit{};
            require(fast_io::operations::read_some(fast_io::posix_stdin(),&credit,&credit+1)==&credit+1&&credit=='Q',"absence completion credit");
            absence_credit=true;quiet=clock_type::now()+std::chrono::seconds(2);
          }
        }
        if(absence_credit&&clock_type::now()>=quiet) {
          marker("ABSENT_NATIVE_TELEMETRY",ns,expected,0,0);subscription.reset();node.reset();rclcpp::shutdown();return 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));continue;
      }
      if(!matched&&subscription->get_publisher_count()==1) {
        auto endpoints=node->get_publishers_info_by_topic(ns+"/telemetry");
        // A matched DDS endpoint can be visible before the ROS graph metadata
        // is ready. Keep the same finite deadline and admit no P gate until
        // every required identity field is present and exactly matches.
        if(endpoints.size()==1&&endpoints.front().node_name()=="roswasm_language_gateway"&&
            endpoints.front().node_namespace()==ns&&endpoints.front().topic_type()=="std_msgs/msg/String") {
          auto const& endpoint=endpoints.front();
          require(endpoint.node_name()=="roswasm_language_gateway"&&endpoint.node_namespace()==ns&&endpoint.topic_type()=="std_msgs/msg/String","native graph identity differs");
          expected=endpoint.endpoint_gid();require(std::any_of(expected.begin(),expected.end(),[](auto b){return b!=0;}),"actual nonzero publisher GID");
          auto const& actual=endpoint.qos_profile().get_rmw_qos_profile();
          require(actual.reliability==RMW_QOS_POLICY_RELIABILITY_RELIABLE&&actual.durability==RMW_QOS_POLICY_DURABILITY_VOLATILE,"actual selected native QoS");
          matched=true;marker("MATCHED_NATIVE_TELEMETRY",ns,expected,warmup,0);
        }
      }
      if(delivered==1&&!announced) {announced=true;marker("DELIVERED_NATIVE_TELEMETRY",ns,expected,warmup,delivered);}
      // Require a contiguous quiet interval after publisher disappearance;
      // time spent waiting for unregister does not count toward retirement.
      if(delivered==1&&subscription->get_publisher_count()==0) {
        if(quiet==deadline)quiet=clock_type::now()+std::chrono::seconds(2);
        if(clock_type::now()>=quiet) {
          marker("PASS_NATIVE_TELEMETRY",ns,expected,warmup,delivered);subscription.reset();node.reset();rclcpp::shutdown();return 0;
        }
      } else quiet=deadline;
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    throw std::runtime_error("actual match/delivery/quiet/endpoint-retirement deadline");
  } catch(std::exception const& error) {fast_io::io::println(fast_io::posix_stderr(),"FAIL_NATIVE_TELEMETRY ",fast_io::mnp::os_c_str(error.what()));return 1;}
}
