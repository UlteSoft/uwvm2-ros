/** Trusted static binding contract for the finite rclcpp source adapter.
 * Merely selecting a backend or table grants nothing. The Host must separately
 * grant the exact aliases/resources/types/QoS/time/log operations. The caller
 * must qualify that actual policy/source association before real ROS tests.
 */
#pragma once
#include <roswasm/roswasmcpp.hpp>
#include <roswasm/service_client.h>
#include <cstdint>
#include <span>
#include <string_view>
namespace roswasm_rclcpp {
enum class Role { publish,subscribe };
struct TopicBinding {
    std::string_view node,topic,alias;
    Role role;
    std::size_t keep_last_depth,maximum_wire_bytes;
};
struct NodeBinding {std::string_view node,steady_clock_alias,info_logger_alias;};
/** Separate business grants, never inferred from a topic or Node row. */
enum class ServiceRole {client,server};
struct ServiceBinding {
    std::string_view node,service,type,qos_profile;
    ServiceRole role;
    std::string_view call_alias,observer_alias,advertise_alias,respond_alias;
    std::size_t maximum_pending{};
    // Optional independent observation owner; empty keeps legacy query95.
    std::string_view availability_alias{};
};
struct ServiceProfile {
    roswasmc_event_backend const* events{};
    std::span<ServiceBinding const> bindings;
    // Separate optional numeric namespace. Old tables need no new imports.
    roswasmc_service_client_backend const* clients{};
};
/** step returns0 to dispatch one pass or125 for explicit local shutdown.
 * Every other code is a real failure. It must not create ROS authority or send
 * update/debug/policy requests. A basic stdio Host pacing gate is one possible
 * separately authorized implementation; there is no implicit sleep/clock I/O.
 */
struct Selection {
    roswasmc_backend const* backend{};
    std::span<TopicBinding const> topics;
    std::span<NodeBinding const> nodes;
    std::uint32_t(*step)(void*){};void* step_context{};
    std::size_t maximum_spin_steps{};
    ServiceProfile const* services{}; // Absent in the original topic profile.
};
}
// A source-locked selected header defines exactly this function. It is an
// explicit build input, never a Guest environment/plugin/ROS-discovery lookup.
#if defined(UWVM_RCLCPP_COMPAT_SELECTION_HEADER)
#include UWVM_RCLCPP_COMPAT_SELECTION_HEADER
#else
namespace roswasm_rclcpp {
inline Selection const* selected_configuration() noexcept {return nullptr;}
}
#endif
