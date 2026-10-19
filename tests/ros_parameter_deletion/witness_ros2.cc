#include "common_ros2.h"
int main(int argc,char** argv)
{
 using namespace deletion_probe;
 try {
  require(argc==4,"usage deletion witness prior-kind scenario gateway-full-name");std::string kind=argv[1],scenario=argv[2];
  int n=1;rclcpp::init(n,argv);
  {
   application app;app.declare("value",kind);if(scenario=="bytes") { app.declare("quota_value",kind); }
   typed_event_probe::reader_observation readers{app.node,app.publisher,argv[3]};readers.capture_baseline(false);
   fast_io::io::println("READY deletion-only prior_type_unknown=1 prior_value_unknown=1 canonical_tag=0");
   if(scenario=="deny-subscription") {
    auto until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
    while(std::chrono::steady_clock::now()<until) { readers.no_projection_after_worker_exit();app.publisher->publish(message());std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
    readers.log_final_absence();
   } else {
    readers.wait_projection(scenario=="bytes"?2:1,8000);
    if(scenario=="prior-delete") {
     app.change("value",kind);std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    } else if(scenario=="bytes") {
     app.erase("quota_value");std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    } else {
     for(unsigned i=0;i<20;++i) { app.publisher->publish(rejected(scenario));std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
     app.publisher->publish(message());std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    app.erase("value");std::this_thread::sleep_for(std::chrono::seconds(1));
   }
   require(!app.failed&&rclcpp::ok(),"native SDK deletion witness remained healthy");
  }
  rclcpp::shutdown();fast_io::io::println("PASS deletion-only ParameterEvent witness");return 0;
 } catch(std::exception const& e) { fast_io::io::perrln(fast_io::mnp::os_c_str(e.what()));rclcpp::shutdown();return 1; }
}
