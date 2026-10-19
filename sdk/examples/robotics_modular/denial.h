/** @file Verify real denial without constructing a successful fake capability.
 * The exact eight-byte output slot must remain unchanged on DENIED13. The
 * alias is data in this instance's memory; it is never another module's pointer.
 */
#ifndef ROSWASM_ROBOTICS_DENIAL_H
#define ROSWASM_ROBOTICS_DENIAL_H
#include "sdk_backend.h"
static uint32_t robot_denied_open(void) {
    static char const alias[] = "modular-out";
    unsigned char slot[8]; uint32_t error; size_t n;
    for(n = 0; n != 8; ++n) slot[n] = 0xa5;
    error = ROBOT_ROS(capability_open)(0,uwvm_ros_address(alias),sizeof(alias)-1,uwvm_ros_address(slot));
    if(error != UWVM_ROS_DENIED) return UWVM_ROS_INVALID;
    for(n = 0; n != 8; ++n) if(slot[n] != 0xa5) return UWVM_ROS_INVALID;
    return 0;
}
#endif
