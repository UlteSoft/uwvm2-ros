// SPDX-License-Identifier: Apache-2.0
// Real original rclcpp APIs. Root must provide the complete genuine B/C/platform
// profile beforehand, including EVERY original default endpoint's real codec.
#include <uwvm_rmw/wasi_signal_bootstrap.hpp>
#include <rclcpp/rclcpp.hpp>
#ifndef UWVM_SIGNAL_READ_FD
#error "Root must bind an actual private read-only signal descriptor"
#endif
#ifndef UWVM_SIGNAL_SOURCE_COOKIE
#error "Root must bind the real immutable Source/channel association"
#endif
// Called only after the enclosing application's actual capability/platform
// preparation. No quiet NodeOptions, synthetic entity or permission fallback.
int uwvm_original_context_entry(int argc,char**argv){
 rclcpp::init(argc,argv); // original default SignalHandlerOptions::All
 {
  auto node=std::make_shared<rclcpp::Node>("original_wasi_signal_context");
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  executor.spin(); // genuine infinite RMW/GraphListener waits must be available
  executor.remove_node(node);
 }
 rclcpp::shutdown(); // original Context + signal worker shutdown, no fake errno
 return 0;
}
int uwvm_run_prepared_original_context(int argc,char**argv,
 uwvm_rmw::wasi_signal::bootstrap_report&actual){
 using namespace uwvm_rmw::wasi_signal;
 profile admitted{profile_version,UWVM_SIGNAL_READ_FD,UWVM_SIGNAL_SOURCE_COOKIE,
  maximum_frame_count,maximum_slice_ns};
 return run_original(admitted,&uwvm_original_context_entry,argc,argv,actual);
}
