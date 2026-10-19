// SPDX-License-Identifier: Apache-2.0
// Original SDK-only API composition contract. The caller owns real prepared
// platform/profile/Context and all original NodeOptions endpoint grants. This
// file changes no upstream default option and supplies no replacement class.
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/node_interfaces/node_graph_interface.hpp>
#include <memory>
#include <stdexcept>
// Invoke only with a genuinely initialized Context and a NodeOptions whose
// context is that same object. The original caller must keep platform/profile
// owners alive until the Context destructor and actual GraphListener join end.
void uwvm_threaded_original_graph_shutdown_contract(
 std::shared_ptr<rclcpp::Context> const&context,rclcpp::NodeOptions const&options){
 if(!context||!context->is_valid()||options.context()!=context)throw std::invalid_argument("actual prepared Context mismatch");
 auto node=std::make_shared<rclcpp::Node>("threaded_original_graph",options);
 auto graph=node->get_node_graph_interface();
 auto event=graph->get_graph_event(); // original GraphListener starts its real worker
 if(!event)throw std::runtime_error("original graph event creation failed");
 // Actual original Context::shutdown calls rcl_shutdown and its registered
 // GraphListener on_shutdown callback, which triggers and physically joins.
 if(!context->shutdown("actual threaded graph contract"))throw std::runtime_error("original Context shutdown refused");
 event.reset();graph.reset();node.reset();
 // Caller must release its real Context, then verify retained diagnostics and
 // platform close. A returned marker is not independent native qualification.
}
