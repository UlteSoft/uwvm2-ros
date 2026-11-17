/** @file
 * @brief Checked fixed-record imports for the local managed lifecycle provider.
 * @details All request/result words are little-endian u64 under the existing
 * seven-argument carrier. No host pointer, node name or native ROS endpoint is
 * accepted. Explicit local-lifecycle deployment binds one logical node/alias.
 * Open accepts exactly role client=1 or server=2; role never replaces exact
 * import/action admission. Deadlines are absolute steady nanoseconds and zero
 * is invalid. All result spans must fit before any state effect or callback
 * consumption. Delivery failure of submit/respond is recovered by exact retry.
 *
 * Requests: open[role], state/transitions/take[handle],
 * submit[handle,key,transition,expected_generation,deadline],
 * respond[handle,request,expected_generation,outcome]. Outcomes retain the ROS
 * callback numbers 97/98/99. Results: open[handle], transitions[allowed_mask],
 * state[state,generation,request,transition,deadline,status,delivered],
 * submit/respond[key,request,generation,state,status],
 * take[request,transition,generation,phase,deadline].
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#define UWVM_ROS_MANAGED_LIFECYCLE_FUNCTIONS(X) \
 X(lifecycle_open,open,"lifecycle.open",1,8) \
 X(lifecycle_state,state,"lifecycle.state",1,56) \
 X(lifecycle_transitions,transitions,"lifecycle.transitions",1,8) \
 X(lifecycle_submit,submit,"lifecycle.submit",5,40) \
 X(lifecycle_take,take,"lifecycle.take",1,40) \
 X(lifecycle_respond,respond,"lifecycle.respond",4,40)
#ifndef UWVM_SCENE_MANAGED_LIFECYCLE_CONTRACT_MACROS_ONLY
namespace uwvm2::imported::ros::managed_lifecycle_contract
{
    enum class operation : std::uint8_t {open,state,transitions,submit,take,respond};
    struct descriptor {operation id;std::string_view name;std::uint64_t words,bytes;};
    inline constexpr std::array operations{
#define UWVM_MANAGED_LIFECYCLE_DESCRIPTOR(symbol,action,name,words,result) descriptor{operation::action,name,words,result},
        UWVM_ROS_MANAGED_LIFECYCLE_FUNCTIONS(UWVM_MANAGED_LIFECYCLE_DESCRIPTOR)
#undef UWVM_MANAGED_LIFECYCLE_DESCRIPTOR
    };
}
#endif
