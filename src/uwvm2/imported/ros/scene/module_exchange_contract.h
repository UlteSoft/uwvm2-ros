/** @file
 * @brief Exact registered-message operations shared by policy and dispatch.
 * @details Indices are internal compile-time identifiers, never guest-selected
 * permission masks. Public spellings are the sole source for deployed grants.
 */
#pragma once
#include <array>
#include <cstdint>
#include <string_view>
#define UWVM_ROS_MODULE_CALL_FUNCTIONS(X) \
 X(module_call_submit,submit,"module.call.submit") \
 X(module_call_take,take,"module.call.take") \
 X(module_call_respond,respond,"module.call.respond") \
 X(module_call_result,result,"module.call.result") \
 X(module_call_acknowledge,acknowledge,"module.call.acknowledge") \
 X(module_call_cancel,cancel,"module.call.cancel")
#ifndef UWVM_SCENE_MODULE_EXCHANGE_MACROS_ONLY
namespace uwvm2::imported::ros::scene::module_exchange
{
    enum class operation : std::uint8_t {
#define UWVM_MODULE_CALL_ENUM(symbol,action,name) action,
        UWVM_ROS_MODULE_CALL_FUNCTIONS(UWVM_MODULE_CALL_ENUM)
#undef UWVM_MODULE_CALL_ENUM
    };
    inline constexpr std::array names{
#define UWVM_MODULE_CALL_NAME(symbol,action,name) std::string_view{name},
        UWVM_ROS_MODULE_CALL_FUNCTIONS(UWVM_MODULE_CALL_NAME)
#undef UWVM_MODULE_CALL_NAME
    };
}
#endif
