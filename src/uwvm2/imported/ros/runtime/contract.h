/** @file
 * @brief Portable local-resource operations in the independent uwvm_ros_v2 ABI.
 * @details Descriptors describe import spelling, exact authorization action,
 * fixed request words and output shape; declaring an operation grants nothing.
 * These operations do not alias WASIp1 or either existing ROS-v1 namespace.
 *
 * All request words and result words are unsigned 64-bit little-endian values;
 * signed nanoseconds use their two's-complement bit representation. Memory
 * indices and spans are validated by the host before resource admission.
 * A fixed-output shortage returns the required length without an effect.
 * A failed guard/timer copy must use transactional delivery and consume nothing.
 * Wait output is an array of four-word source/token/events/sequence records.
 *
 * Creation returns one opaque resource handle. guard.trigger/take take one
 * guard handle. clock.create takes the explicit clock kind. clock.read takes
 * one clock handle and returns now/epoch/started/kind. clock.update takes an
 * independently admitted source capability, clock handle, now and started.
 * timer.create takes clock capability/clock handle; reset takes timer/due/period.
 * timer.take returns expected/actual/occurrences/clock_epoch. wait.add takes
 * waitset/source capability/source handle/interests/token. wait.remove takes
 * waitset/source handle. wait.acknowledge adds the source sequence. wait.poll
 * takes waitset/requested duration; infinite duration requires explicit policy.
 * resource.close takes one resource handle; it is distinct from closing a
 * capability handle. APL-2.0.
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>

// C++ symbol, public import name, exact capability operation, request words,
// fixed result bytes. Public names describe semantics, not implementation order.
// A negative result size denotes a bounded variable result.
#define UWVM_ROS_RUNTIME_FUNCTIONS(X) \
 X(guard_create,"guard.create","guard.create",0,8) \
 X(guard_trigger,"guard.trigger","guard.trigger",1,0) \
 X(guard_take,"guard.take","guard.take",1,8) \
 X(clock_create,"clock.create","clock.create",1,8) \
 X(clock_read,"clock.read","clock.read",1,32) \
 X(clock_update,"clock.update","clock.update",4,0) \
 X(timer_create,"timer.create","timer.create",2,8) \
 X(timer_reset,"timer.reset","timer.reset",3,0) \
 X(timer_cancel,"timer.cancel","timer.cancel",1,0) \
 X(timer_take,"timer.take","timer.take",1,32) \
 X(wait_set_create,"wait.create","wait.create",0,8) \
 X(wait_set_add,"wait.add","wait.add",5,0) \
 X(wait_set_remove,"wait.remove","wait.remove",2,0) \
 X(wait_set_acknowledge,"wait.acknowledge","wait.acknowledge",3,0) \
 X(wait_set_wait,"wait.poll","wait.poll",2,-1) \
 X(resource_close,"resource.close","resource.close",1,0)

#ifndef UWVM_SCENE_RUNTIME_CONTRACT_MACROS_ONLY
namespace uwvm2::imported::ros::runtime_contract
{
    /// @brief Internal semantic identifiers; the ABI major appears only in the module name.
    enum class operation : std::uint8_t {
#define UWVM_ROS_OPERATION(name,external,grant,words,bytes) name,
        UWVM_ROS_RUNTIME_FUNCTIONS(UWVM_ROS_OPERATION)
#undef UWVM_ROS_OPERATION
    };
    /// @brief Exact import/action and bounded request/output layout; negative bytes denotes wait records.
    struct descriptor { operation id;std::string_view name,grant;unsigned words;int bytes; };
    inline constexpr std::array operations{
#define UWVM_ROS_DESCRIPTOR(name,external,grant,words,bytes) descriptor{operation::name,external,grant,words,bytes},
        UWVM_ROS_RUNTIME_FUNCTIONS(UWVM_ROS_DESCRIPTOR)
#undef UWVM_ROS_DESCRIPTOR
    };
    /// @brief Whether an exact action is one of the clock operations.
    inline constexpr bool clock(std::string_view value) noexcept
    {return value=="clock.create"||value=="clock.read"||value=="clock.update";}
    /// @brief Discover local operations without granting or aliasing them.
    inline constexpr bool local(std::string_view value) noexcept
    {for(auto const& item:operations)if(item.name==value)return true;return false;}
    inline constexpr std::string_view type="builtin/runtime",identity="builtin:runtime-record";
}
#endif
