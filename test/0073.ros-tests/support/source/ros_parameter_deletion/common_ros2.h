#pragma once
#include <rclcpp/rclcpp.hpp>
#include <fast_io.h>
#include "../ros_resources/common.h"
using namespace ros_resources_probe;
#include "../ros_resources/typed_event_witness_ros2.h"
#include "../../../../../sdk/origin/common/codec.hpp"
#include <rclcpp/parameter_client.hpp>
#include <rosgraph_msgs/msg/clock.hpp>
#include <atomic>
#include <array>
#include <future>
#include <thread>
namespace deletion_probe
{
 using namespace ros_resources_probe;
 inline constexpr char node_name[]{"/parameter_deletion/native"};
 inline std::array<std::string,9> const prior_kinds{"bool","int64","float64","string","byte-array","bool-array","int64-array","float64-array","string-array"};
 inline rclcpp::ParameterValue prior(std::string const& kind,bool changed=false)
 {
  if(kind=="bool") { return rclcpp::ParameterValue{changed}; }
  if(kind=="int64") { return rclcpp::ParameterValue{std::int64_t{changed?9007199254740993LL:-7LL}}; }
  if(kind=="float64") { return rclcpp::ParameterValue{changed?1.25:-0.5}; }
  if(kind=="string") { return rclcpp::ParameterValue{std::string{changed?"changed":"before"}}; }
  if(kind=="byte-array") { return rclcpp::ParameterValue{std::vector<std::uint8_t>{0,127,128,255}}; }
  if(kind=="bool-array") { return rclcpp::ParameterValue{std::vector<bool>{changed,false,true}}; }
  if(kind=="int64-array") { return rclcpp::ParameterValue{std::vector<std::int64_t>{-7,9007199254740993LL}}; }
  if(kind=="float64-array") { return rclcpp::ParameterValue{std::vector<double>{-0.0,changed?1.25:-0.5}}; }
  require(kind=="string-array","known prior kind");return rclcpp::ParameterValue{std::vector<std::string>{"","中",changed?"changed":"before"}};
 }
 inline core::capability_spec capability(std::string alias="cap",std::string name="value")
 {
  auto c=resource(core::version::ros2,std::move(alias),"param.subscribe-events",std::string{node_name}+"/"+name);
  c.type="builtin/param-event-deletion-v1";c.type_identity="builtin:param-event-deletion-v1";
  c.limits["parameter-node"]=node_name;c.limits["parameter-name"]=name;c.limits["max-parameter-bytes"]="4096";
  c.limits["burst"]="8";c.limits["max-subscribers"]="1";c.limits["max-event-fields"]="8";
  c.limits["max-event-queue-depth"]="8";c.limits["max-event-queue-bytes"]="32768";c.limits["overflow-policy"]="reject";
  c.limits["qos.history"]="keep-last";c.limits["qos.depth"]="32";c.limits["qos.reliability"]="reliable";c.limits["qos.durability"]="volatile";
  for(auto key:{"qos.deadline","qos.lifespan","qos.liveliness-lease-duration"}) { c.limits[key]="infinite"; }
  c.limits["qos.liveliness"]="automatic";gateway::validate(core::version::ros2,c);return c;
 }
 inline std::vector<std::uint8_t> wire(std::string const& name="value",unsigned stamp=9)
 {
  uwvm_ros::typed_parameter_event e;e.seconds=123;e.nanoseconds=stamp;e.kind=3;e.node=node_name;e.name=name;e.tag=uwvm_ros::parameter_event_value_tag::deletion;
  return uwvm_ros::encode(e);
 }
 struct application
 {
  rclcpp::Node::SharedPtr node;
  rclcpp::Publisher<rcl_interfaces::msg::ParameterEvent>::SharedPtr publisher;
  rclcpp::Publisher<rosgraph_msgs::msg::Clock>::SharedPtr clock;
  std::shared_ptr<rclcpp::AsyncParametersClient> client;
  rclcpp::executors::SingleThreadedExecutor executor;
  std::atomic<bool> stopped{},failed{};std::thread spinner;
  application()
  {
   rclcpp::NodeOptions options;options.enable_rosout(false).parameter_overrides({rclcpp::Parameter("use_sim_time",true),rclcpp::Parameter("start_type_description_service",false)});
   node=std::make_shared<rclcpp::Node>("native","/parameter_deletion",options);
   publisher=node->create_publisher<rcl_interfaces::msg::ParameterEvent>("/parameter_events",rclcpp::QoS(32).reliable().durability_volatile());
   clock=node->create_publisher<rosgraph_msgs::msg::Clock>("/clock",rclcpp::QoS(10).reliable());
   executor.add_node(node);spinner=std::thread{[&] { try { while(!stopped&&rclcpp::ok()) { executor.spin_once(std::chrono::milliseconds(10)); } } catch(...) { failed=true; } }};
   try {
    rosgraph_msgs::msg::Clock now;now.clock.sec=123;now.clock.nanosec=9;
    auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(node->get_clock()->now().nanoseconds()!=123000000009LL) { clock->publish(now);require(!failed&&std::chrono::steady_clock::now()<deadline,"fixed native ROS clock reached official SDK publisher");std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
    client=std::make_shared<rclcpp::AsyncParametersClient>(node,node_name);
    while(!client->service_is_ready()) { require(!failed&&std::chrono::steady_clock::now()<deadline,"official native parameter services discovered");std::this_thread::sleep_for(std::chrono::milliseconds(5)); }
   } catch(...) { finish();throw; }
  }
  void finish() { stopped=true;executor.cancel();if(spinner.joinable()) { spinner.join(); } }
  ~application() { finish(); }
  void declare(std::string const& name,std::string const& kind)
  { rcl_interfaces::msg::ParameterDescriptor descriptor;descriptor.dynamic_typing=true;node->declare_parameter(name,prior(kind),descriptor); }
  void change(std::string const& name,std::string const& kind)
  { require(node->set_parameter(rclcpp::Parameter{name,prior(kind,true)}).successful,"official SDK parameter change succeeded"); }
  void erase(std::string const& name)
  {
   auto pending=client->delete_parameters({name});require(pending.wait_for(std::chrono::seconds(5))==std::future_status::ready,"official SDK delete completed");
   auto results=pending.get();require(results.size()==1&&results[0].successful&&!node->has_parameter(name),"official SDK delete removes actual dynamic parameter");
   fast_io::io::println("OFFICIAL_DELETE name=",name," successful=1 native_has_parameter=0 prior_metadata_attested=0");
  }
 };
 inline rcl_interfaces::msg::ParameterEvent message(std::string const& name="value",unsigned stamp=8)
 {
  rcl_interfaces::msg::ParameterEvent e;e.node=node_name;e.stamp.sec=123;e.stamp.nanosec=stamp;
  rcl_interfaces::msg::Parameter p;p.name=name;p.value.type=rcl_interfaces::msg::ParameterType::PARAMETER_NOT_SET;e.deleted_parameters.push_back(p);return e;
 }
 inline rcl_interfaces::msg::ParameterEvent rejected(std::string const& scenario)
 {
  auto e=message("value",80);
  if(scenario=="new-not-set") { e.new_parameters=std::move(e.deleted_parameters);e.deleted_parameters.clear(); }
  else if(scenario=="changed-not-set") { e.changed_parameters=std::move(e.deleted_parameters);e.deleted_parameters.clear(); }
  else if(scenario=="concrete-delete") { e.deleted_parameters[0].value=prior("bool",true).to_value_msg(); }
  else if(scenario=="wrong-node") { e.node="/parameter_deletion/other"; }
  else if(scenario=="wrong-name") { e.deleted_parameters[0].name="other"; }
  else if(scenario=="stamp") { e.stamp.nanosec=1000000000U; }
  else if(scenario=="fields") { e.deleted_parameters.push_back(e.deleted_parameters[0]); }
  else { require(false,"known rejected deletion scenario"); }
  return e;
 }
 inline void receive(ipc::client& client,std::string const& alias,std::vector<std::uint8_t> const& expected)
 {
  auto until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
  while(std::chrono::steady_clock::now()<until) {
   auto r=client.call("event.poll",alias,{},1000);if(!r.error) { require(r.payload==expected,"whole deletion envelope equals independent SDK oracle");return; }
   require(r.error==11&&r.payload.empty(),"waiting deletion has EAGAIN and empty payload");std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }require(false,"actual deletion event receipt deadline");
 }
}
