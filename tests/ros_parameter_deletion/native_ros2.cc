#include "common_ros2.h"
int main(int argc,char** argv)
{
 using namespace deletion_probe;
 try {
  require(argc==2,"usage deletion native probe gateway");int n=1;rclcpp::init(n,argv);
  {
   application app;for(auto const& kind:prior_kinds) { auto name=kind;std::replace(name.begin(),name.end(),'-','_');app.declare(name,kind); }
   fixture session{core::version::ros2,argv[1],[](gateway::configuration& config) {
    config.policy.connection["domain-id"]="192";config.policy.connection["node-name"]="deletion_gateway";config.policy.connection["namespace"]="/parameter_deletion";config.policy.capabilities.clear();
    for(auto const& kind:prior_kinds) { auto name=kind;std::replace(name.begin(),name.end(),'-','_');add(config,capability(kind,name)); }
    auto fields=capability("fields","value");fields.limits["max-event-fields"]="1";add(config,std::move(fields));
    auto bytes=capability("bytes","quota_value");bytes.limits["max-parameter-bytes"]="1";add(config,std::move(bytes));add(config,capability("barrier","value"));
   }};
   ipc::client client;session.connect(client);typed_event_probe::reader_observation readers{app.node,app.publisher,"/parameter_deletion/deletion_gateway"};readers.capture_baseline(false);
   unsigned positives{},rejections{};
   for(auto const& kind:prior_kinds) {
    auto name=kind;std::replace(name.begin(),name.end(),'-','_');require(!client.call("param.subscribe-events",kind,{},2000).error,"dedicated deletion subscription authorized");readers.wait_projection(1);
    app.change(name,kind);app.publisher->publish(message(name,8));receive(client,kind,wire(name,8));
    app.erase(name);receive(client,kind,wire(name));++positives;
    require(!client.call("capability.close",kind,{},1000).error,"deletion subscription last close");readers.wait_projection(0);
   }
   app.declare("value","bool");require(!client.call("param.subscribe-events","barrier",{},2000).error,"deletion rejection projection created");readers.wait_projection(1);
   for(std::string scenario:{"new-not-set","changed-not-set","concrete-delete","wrong-node","wrong-name","stamp"}) {
    app.publisher->publish(rejected(scenario));app.publisher->publish(message());receive(client,"barrier",wire("value",8));++rejections;
   }
   require(!client.call("capability.close","barrier",{},1000).error,"barrier close");readers.wait_projection(0);
   require(!client.call("param.subscribe-events","fields",{},2000).error,"field-limited deletion created");readers.wait_projection(1);app.publisher->publish(rejected("fields"));app.publisher->publish(message());receive(client,"fields",wire("value",8));++rejections;
   require(!client.call("capability.close","fields",{},1000).error,"field projection close");readers.wait_projection(0);
   app.declare("quota_value","bool");require(!client.call("param.subscribe-events","bytes",{},2000).error&&!client.call("param.subscribe-events","barrier",{},2000).error,"independent byte-limited and marker projections authorized");readers.wait_projection(2);
   app.erase("quota_value");app.erase("value");receive(client,"barrier",wire());auto empty=client.call("event.poll","bytes",{},1000);require(empty.error==11&&empty.payload.empty(),"whole deletion envelope byte quota rejects actual native deletion");++rejections;
   require(!client.call("capability.close","bytes",{},1000).error&&!client.call("capability.close","barrier",{},1000).error,"all independent projections close");readers.wait_projection(0);
   require(!app.failed&&rclcpp::ok(),"official SDK deletion publisher remains healthy");fast_io::io::println("PASS deletion-only native official_deletes=",positives+2," prior_kinds=9 rejection_batches=",rejections," tag0_kind3_empty_payload prior_metadata_attested0 retained_framework_GIDs reader_teardown");
  }
  rclcpp::shutdown();return 0;
 } catch(std::exception const& e) { fast_io::io::perrln(fast_io::mnp::os_c_str(e.what()));rclcpp::shutdown();return 1; }
}
