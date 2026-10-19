#include "../../compat/node.h"
// SPDX-License-Identifier: Apache-2.0
#include "default_control_gateway.h"
#include "default_control_gateway_features.h"
#if !UWVM_DEFAULT_CONTROL_GATEWAY_NATIVE_API
namespace uwvm2::ros_gateway::default_control_gateway {
using cap=::uwvm2::imported::ros::capability_spec;
using binding=::uwvm2::imported::ros::ipc::rmw_endpoint::binding;
using packet=::uwvm2::imported::ros::ipc::rmw_endpoint::packet;
std::uint32_t create_service(std::shared_ptr<profile const>const&,std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type>const&,
 std::shared_ptr<uwvm_rmw_cpp::Registry const>const&,cap const&,binding const&,std::uint64_t,std::size_t,
 std::shared_ptr<native_rcl_service::report>const&,std::shared_ptr<native_rcl_service_registry::endpoint>&,packet&)noexcept{return 95;}
std::uint32_t create_publisher(std::shared_ptr<profile const>const&,std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type>const&,cap const&,binding const&,
 std::uint64_t,std::shared_ptr<native_rmw_publisher::endpoint>&,packet&)noexcept{return 95;}
std::uint32_t create_subscription(std::shared_ptr<profile const>const&,std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type>const&,cap const&,binding const&,
 std::uint64_t,std::shared_ptr<native_rmw_subscription::endpoint>&,packet&)noexcept{return 95;}
}
#endif
