/** @file Allocation-free, finite registration owned by this callback instance.
 * The owner tag is a protocol label, not authenticated caller identity. Any
 * module importing these exports can call them; this is not a security ACL.
 * The generation never wraps. Its exhaustion refuses a new registration.
 */
#ifndef ROSWASM_ROBOTICS_CALLBACK_IMPL_H
#define ROSWASM_ROBOTICS_CALLBACK_IMPL_H
#include "scalar_abi.h"
#include "robotics_math.h"
static uint32_t robot_generation;
static uint32_t robot_active;
static uint32_t robot_busy;
static uint32_t robot_calls;
static uint32_t robot_denials;

/* The separately selected negative fixture defines this hook to call the
 * actual ROS import during a cross-module stack. Normal providers have no
 * host imports. No native function pointer ever enters this registry. */
#ifndef ROBOT_CALLBACK_CHAIN_CHECK
#define ROBOT_CALLBACK_CHAIN_CHECK() 0
#endif

ROBOT_EXTERN ROBOT_EXPORT("callback_begin") uint32_t callback_begin(uint32_t owner) {
    if(owner != ROBOT_OWNER || robot_active || robot_busy || robot_generation == UINT32_MAX) return 0;
    ++robot_generation; robot_active = 1; robot_calls = 0;
    return robot_generation;
}
ROBOT_EXTERN ROBOT_EXPORT("callback_end") uint32_t callback_end(uint32_t owner,uint32_t generation) {
    if(robot_busy) return 17;
    if(!robot_active || owner != ROBOT_OWNER || generation != robot_generation) return 9;
    robot_active = 0; return 0;
}
ROBOT_EXTERN ROBOT_EXPORT("callback_count") uint32_t callback_count(uint32_t owner,uint32_t generation) {
    if(!robot_active || owner != ROBOT_OWNER || generation != robot_generation) return UINT32_MAX;
    return robot_calls;
}
ROBOT_EXTERN ROBOT_EXPORT("callback_denials") uint32_t callback_denials(void) {return robot_denials;}
ROBOT_EXTERN ROBOT_EXPORT("on_motion") int32_t on_motion(uint32_t owner,uint32_t generation,int32_t requested,int32_t clearance) {
    if(robot_busy) return ROBOT_BUSY;
    if(!robot_active || owner != ROBOT_OWNER || generation != robot_generation) return ROBOT_STALE;
    if(requested < -100 || requested > 100 || clearance < 0 || clearance > 5000 || robot_calls == UINT32_MAX) return ROBOT_INVALID;
    robot_busy = 1;
    if(ROBOT_CALLBACK_CHAIN_CHECK()) {robot_busy = 0; return ROBOT_INVALID;}
#ifdef ROBOT_CALLBACK_COUNTS_DENIALS
    ++robot_denials; /* At most three accepted calls in this executable lesson. */
#endif
    ++robot_calls; robot_busy = 0;
    return robot_math_command(requested,clearance);
}
#endif
