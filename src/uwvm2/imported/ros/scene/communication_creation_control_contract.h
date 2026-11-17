/** @file
 * @brief Guest operations for separately publishing or cancelling an admitted creation.
 * @details Both operations take one little-endian u64 request identity. A take
 * returns the canonical 64-byte publication receipt; cancel returns the 48-byte
 * creation receipt. Shared request.describe/request.release use the same retained
 * owner. Default exposure and grant authority are zero. Names carry semantics;
 * the enclosing uwvm_ros_v2 module owns the ABI generation.
 */
#pragma once
#define UWVM_ROS_COMMUNICATION_CREATION_FUNCTIONS(X) \
    X(communication_open_take, take, "communication.open.take") \
    X(communication_open_cancel, cancel, "communication.open.cancel")
#ifndef UWVM_SCENE_COMMUNICATION_CREATION_CONTRACT_MACROS_ONLY
#include "communication_open_dispatch.h"
namespace uwvm2::imported::ros::communication_creation_control_contract
{
    using operation=scene::communication_open_dispatch::operation;
    struct descriptor{operation id;std::string_view name;};
    inline constexpr std::array operations{
#define UWVM_COMMUNICATION_CREATION_DESCRIPTOR(symbol,action,name) descriptor{operation::action,name},
        UWVM_ROS_COMMUNICATION_CREATION_FUNCTIONS(UWVM_COMMUNICATION_CREATION_DESCRIPTOR)
#undef UWVM_COMMUNICATION_CREATION_DESCRIPTOR
    };
}
#endif
