/* SPDX-License-Identifier: Apache-2.0
 * Complete selected Franka state and pure gripper-goal construction.
 * Select one ROS major explicitly. No constructor opens a native resource.
 */
#ifndef UWVM_FRANKA_STATE_H
#define UWVM_FRANKA_STATE_H
#include <math.h>
#include <roswasm/codec.h>
#include "bounded_records.h"
#if !defined(UWVM_FRANKA_ROS_VERSION) || (UWVM_FRANKA_ROS_VERSION!=1 && UWVM_FRANKA_ROS_VERSION!=2)
#error "Select UWVM_FRANKA_ROS_VERSION=1 or 2 with its exact complete bundle"
#endif
#if UWVM_FRANKA_ROS_VERSION==1
#include <roswasm/ros1.h>
#define UWVM_FRANKA_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_franka_msgs_FrankaState uwvm_franka_state;
typedef roswasmc_bounded_1_franka_gripper_GraspGoal uwvm_franka_grasp_goal;
#define UWVM_FRANKA_STATE_DECODE roswasmc_bounded_1_franka_msgs_FrankaState_decode
#define UWVM_FRANKA_STATE_ENCODE roswasmc_bounded_1_franka_msgs_FrankaState_encode
#define UWVM_FRANKA_STATE_CAPACITY ROSWASMC_BOUNDED_1_FRANKA_MSGS_FRANKASTATE_MAX_WIRE_BYTES
#elif UWVM_FRANKA_ROS_VERSION==2
#include <roswasm/ros2.h>
#define UWVM_FRANKA_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState uwvm_franka_state;
typedef roswasmc_bounded_2_franka_msgs_action_Grasp_Goal uwvm_franka_grasp_goal;
#define UWVM_FRANKA_STATE_DECODE roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_decode
#define UWVM_FRANKA_STATE_ENCODE roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_encode
#define UWVM_FRANKA_STATE_CAPACITY ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_MAX_WIRE_BYTES
#endif
/** Serialized caller-owned storage for the exact complete selected state.
 * Capacity includes every nested field, never a projected prefix.
 */
typedef struct uwvm_franka_storage {unsigned char bytes[UWVM_FRANKA_STATE_CAPACITY];} uwvm_franka_storage;
/** Construct one whole original Grasp goal without sending it.
 * Units are metres, metres/second and newtons. max_width is an explicit,
 * independently reviewed model limit, not discovered or inferred here.
 * Positive speed/force and finite nonnegative tolerances are necessary input
 * checks; they do not qualify the model's force/speed limits or physical safety.
 * All refusal paths preserve the destination. No recovery/resume is implicit.
 */
static inline uint32_t uwvm_franka_make_grasp(double width,double speed,double force,
 double inner,double outer,double max_width,uwvm_franka_grasp_goal* out) {
 uwvm_franka_grasp_goal next={0};
 if(!out||!isfinite(width)||!isfinite(speed)||!isfinite(force)||!isfinite(inner)||
    !isfinite(outer)||!isfinite(max_width)||max_width<=0||width<0||width>max_width||
    speed<=0||force<=0||inner<0||outer<0)return 22;
 next.width=width;next.speed=speed;next.force=force;next.epsilon.inner=inner;next.epsilon.outer=outer;
 *out=next;return 0;
}
/** Poll once through the real controlled ROS ABI. Idle11 is not a state.
 * Decode commits a complete value transactionally; scratch and output must be
 * disjoint. Error90 never enlarges buffers or replays a consumed event.
 */
static inline uint32_t uwvm_franka_poll_state(roswasmc_capability const* cap,
 uwvm_franka_state* out,uwvm_franka_storage* scratch) {
 roswasmc_result r;
 if(!cap||!cap->context||!cap->context->backend||
    cap->context->backend->version!=UWVM_FRANKA_ROS_VERSION||!out||!scratch||
    uwvm_bounded_overlap(out,sizeof *out,scratch,sizeof *scratch))return 22;
 r=roswasmc_poll(cap,scratch->bytes,sizeof scratch->bytes);
 if(r.error)return r.error;
 if(!r.has_length||r.length>sizeof scratch->bytes)return 22;
 return UWVM_FRANKA_STATE_DECODE(scratch->bytes,(size_t)r.length,out);
}
#endif
