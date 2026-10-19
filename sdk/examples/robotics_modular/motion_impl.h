/** @file Pure motion composition: call an actual separately instantiated Wasm
 * callback. This library cannot drive hardware, perform IO or open a ROS right.
 */
#ifndef ROSWASM_ROBOTICS_MOTION_IMPL_H
#define ROSWASM_ROBOTICS_MOTION_IMPL_H
#include "scalar_abi.h"
ROBOT_EXTERN ROBOT_IMPORT("callbacks","on_motion") int32_t robot_on_motion(uint32_t,uint32_t,int32_t,int32_t);
ROBOT_EXTERN ROBOT_EXPORT("motion_step") int32_t motion_step(uint32_t owner,uint32_t generation,int32_t requested,int32_t clearance) {
    if(requested < -100 || requested > 100 || clearance < 0 || clearance > 5000) return ROBOT_INVALID;
    return robot_on_motion(owner,generation,requested,clearance);
}
#endif
