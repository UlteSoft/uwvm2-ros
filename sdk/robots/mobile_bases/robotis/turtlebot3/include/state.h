/* SPDX-License-Identifier: Apache-2.0
 * Whole original TurtleBot3 observations and pure sound-value construction.
 * Select an exact original profile and corresponding complete bundle.
 */
#ifndef UWVM_TURTLEBOT3_STATE_H
#define UWVM_TURTLEBOT3_STATE_H
#include <roswasm/codec.h>
#include "bounded_records.h"
#if !defined(UWVM_TURTLEBOT3_PROFILE) || UWVM_TURTLEBOT3_PROFILE<1 || UWVM_TURTLEBOT3_PROFILE>4
#error "Select profile 1=Noetic, 2=Humble, 3=Jazzy, 4=main with matching exact source"
#endif
#if UWVM_TURTLEBOT3_PROFILE==1
#define UWVM_TURTLEBOT3_ROS_VERSION 1
#else
#define UWVM_TURTLEBOT3_ROS_VERSION 2
#endif
#if UWVM_TURTLEBOT3_ROS_VERSION==1
#include <roswasm/ros1.h>
#define UWVM_TURTLEBOT3_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_turtlebot3_msgs_SensorState uwvm_turtlebot3_state;
typedef roswasmc_bounded_1_turtlebot3_msgs_Sound uwvm_turtlebot3_sound;
#define UWVM_TURTLEBOT3_STATE_DECODE roswasmc_bounded_1_turtlebot3_msgs_SensorState_decode
#define UWVM_TURTLEBOT3_STATE_ENCODE roswasmc_bounded_1_turtlebot3_msgs_SensorState_encode
#define UWVM_TURTLEBOT3_STATE_CAPACITY ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_MAX_WIRE_BYTES
#else
#include <roswasm/ros2.h>
#define UWVM_TURTLEBOT3_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState uwvm_turtlebot3_state;
typedef roswasmc_bounded_2_turtlebot3_msgs_msg_Sound uwvm_turtlebot3_sound;
#define UWVM_TURTLEBOT3_STATE_DECODE roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_decode
#define UWVM_TURTLEBOT3_STATE_ENCODE roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_encode
#define UWVM_TURTLEBOT3_STATE_CAPACITY ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_MAX_WIRE_BYTES
#endif
/** Caller-owned scratch; includes every field of the complete observation.
 * Keep it disjoint from the destination and serialize access to a capability.
 */
typedef struct uwvm_turtlebot3_storage {unsigned char bytes[UWVM_TURTLEBOT3_STATE_CAPACITY];} uwvm_turtlebot3_storage;
/** Build a complete original Sound message without publishing or opening.
 * Values 0..5 are the original named codes. Invalid input preserves out.
 * ROS1 uses a topic; ROS2 also defines a distinct Sound service request.
 * This helper does not convert a topic value into a service or perform motion.
 */
static inline uint32_t uwvm_turtlebot3_make_sound(uint32_t code,uwvm_turtlebot3_sound* out) {
 uwvm_turtlebot3_sound next={0};if(!out||code>5)return 22;
 next.value=(uint8_t)code;*out=next;return 0;
}
/** One full transactional decode over the actual controlled ROS ABI.
 * Error11 is idle, never a new observation. Error90 does not retry or allocate.
 * Closing a subscription is resource cleanup, not a physical emergency stop.
 */
static inline uint32_t uwvm_turtlebot3_poll_state(roswasmc_capability const* cap,
 uwvm_turtlebot3_state* out,uwvm_turtlebot3_storage* scratch) {
 roswasmc_result r;
 if(!cap||!cap->context||!cap->context->backend||
 cap->context->backend->version!=UWVM_TURTLEBOT3_ROS_VERSION||!out||!scratch||
 uwvm_bounded_overlap(out,sizeof *out,scratch,sizeof *scratch))return 22;
 r=roswasmc_poll(cap,scratch->bytes,sizeof scratch->bytes);
 if(r.error)return r.error;
 if(!r.has_length||r.length>sizeof scratch->bytes)return 22;
 return UWVM_TURTLEBOT3_STATE_DECODE(scratch->bytes,(size_t)r.length,out);
}
#endif
