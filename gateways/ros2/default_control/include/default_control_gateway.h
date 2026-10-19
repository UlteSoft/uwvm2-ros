// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "../../compat/node.h"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string_view>
namespace rclcpp {class Node;}
namespace uwvm_rmw_cpp {class Registry;}
namespace uwvm2::imported::ros {struct capability_spec;}
namespace uwvm2::imported::ros::ipc::rmw_endpoint {struct binding;struct packet;}
namespace uwvm2::ros_gateway::native_rcl_service {class report;}
namespace uwvm2::ros_gateway::native_rcl_service_registry {class endpoint;}
namespace uwvm2::ros_gateway::native_rmw_publisher {class endpoint;}
namespace uwvm2::ros_gateway::native_rmw_subscription {class endpoint;}
namespace uwvm2::ros_gateway::default_control_gateway {
// An explicit independently exported input, never an ambient getter used as its
// own expected hash. The generated name-only manifest fixes the complete list.
struct identity {char const* native_type;std::uint8_t rihs01[32];};
class profile;
struct canonical_limits {std::size_t request{},response{};std::uint64_t separately_owned_native_depth_limit{};};
// Cold metadata admission only. All original C++ generic/introspection graphs,
// source/defaults/typed layouts/Event handles pass before publishing the owner.
// No Node, endpoint, permission, alias, global cache or Host singleton is made.
std::uint32_t prepare(identity const*,std::size_t,std::shared_ptr<profile const>&) noexcept;
bool known(std::shared_ptr<profile const> const&,::uwvm2::imported::ros::capability_spec const&) noexcept;
std::uint32_t canonical_bounds(std::shared_ptr<profile const> const&,
 ::uwvm2::imported::ros::capability_spec const&,canonical_limits&) noexcept;
std::uint32_t validate_body(std::shared_ptr<profile const> const&,
 ::uwvm2::imported::ros::capability_spec const&,bool response,std::uint8_t const*,std::size_t) noexcept;
std::uint32_t create_service(std::shared_ptr<profile const> const&,std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const&,
 std::shared_ptr<uwvm_rmw_cpp::Registry const> const&,::uwvm2::imported::ros::capability_spec const&,
 ::uwvm2::imported::ros::ipc::rmw_endpoint::binding const&,std::uint64_t,std::size_t,
 std::shared_ptr<native_rcl_service::report> const&,std::shared_ptr<native_rcl_service_registry::endpoint>&,
 ::uwvm2::imported::ros::ipc::rmw_endpoint::packet&) noexcept;
std::uint32_t create_publisher(std::shared_ptr<profile const> const&,std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const&,
 ::uwvm2::imported::ros::capability_spec const&,::uwvm2::imported::ros::ipc::rmw_endpoint::binding const&,
 std::uint64_t,std::shared_ptr<native_rmw_publisher::endpoint>&,::uwvm2::imported::ros::ipc::rmw_endpoint::packet&) noexcept;
std::uint32_t create_subscription(std::shared_ptr<profile const> const&,std::shared_ptr<::uwvm2::ros_gateway::ros2_compat::node_type> const&,
 ::uwvm2::imported::ros::capability_spec const&,::uwvm2::imported::ros::ipc::rmw_endpoint::binding const&,
 std::uint64_t,std::shared_ptr<native_rmw_subscription::endpoint>&,::uwvm2::imported::ros::ipc::rmw_endpoint::packet&) noexcept;
// These are selected factories, not wildcard fallback. A Root-owned composite
// may route unrelated explicitly qualified families through their old selector.
// Every returned endpoint retains the SAME immutable profile through real fini.
}
