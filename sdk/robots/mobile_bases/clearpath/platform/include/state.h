/* SPDX-License-Identifier: Apache-2.0
 * Complete selected Clearpath/legacy Husky/Jackal state, not a projected view.
 * Selection1=Noetic platform,2=Noetic Husky,3=Noetic Jackal,4=Humble,
 * 5=Jazzy,6=main development snapshot. Source/native binding stays explicit.
 */
#ifndef UWVM_CLEARPATH_STATE_H
#define UWVM_CLEARPATH_STATE_H
#include <math.h>
#include <float.h>
#include <roswasm/codec.h>
#include "bounded_records.h"
#if !defined(UWVM_CLEARPATH_SELECTION) || UWVM_CLEARPATH_SELECTION<1 || UWVM_CLEARPATH_SELECTION>6
#error "Select exactly one Clearpath application and its complete original bundle"
#endif
#if UWVM_CLEARPATH_SELECTION==1
#define UWVM_CLEARPATH_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_CLEARPATH_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_clearpath_platform_msgs_Status uwvm_clearpath_state;
#define UWVM_CLEARPATH_STATE_DECODE roswasmc_bounded_1_clearpath_platform_msgs_Status_decode
#define UWVM_CLEARPATH_STATE_ENCODE roswasmc_bounded_1_clearpath_platform_msgs_Status_encode
#define UWVM_CLEARPATH_STATE_CAPACITY ROSWASMC_BOUNDED_1_CLEARPATH_PLATFORM_MSGS_STATUS_MAX_WIRE_BYTES
#elif UWVM_CLEARPATH_SELECTION==2
#define UWVM_CLEARPATH_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_CLEARPATH_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_husky_msgs_HuskyStatus uwvm_clearpath_state;
#define UWVM_CLEARPATH_STATE_DECODE roswasmc_bounded_1_husky_msgs_HuskyStatus_decode
#define UWVM_CLEARPATH_STATE_ENCODE roswasmc_bounded_1_husky_msgs_HuskyStatus_encode
#define UWVM_CLEARPATH_STATE_CAPACITY ROSWASMC_BOUNDED_1_HUSKY_MSGS_HUSKYSTATUS_MAX_WIRE_BYTES
#elif UWVM_CLEARPATH_SELECTION==3
#define UWVM_CLEARPATH_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_CLEARPATH_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_jackal_msgs_Status uwvm_clearpath_state;
#define UWVM_CLEARPATH_STATE_DECODE roswasmc_bounded_1_jackal_msgs_Status_decode
#define UWVM_CLEARPATH_STATE_ENCODE roswasmc_bounded_1_jackal_msgs_Status_encode
#define UWVM_CLEARPATH_STATE_CAPACITY ROSWASMC_BOUNDED_1_JACKAL_MSGS_STATUS_MAX_WIRE_BYTES
#elif UWVM_CLEARPATH_SELECTION==4
#define UWVM_CLEARPATH_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_CLEARPATH_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_clearpath_platform_msgs_msg_Status uwvm_clearpath_state;
#define UWVM_CLEARPATH_STATE_DECODE roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_decode
#define UWVM_CLEARPATH_STATE_ENCODE roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_encode
#define UWVM_CLEARPATH_STATE_CAPACITY ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_MAX_WIRE_BYTES
#elif UWVM_CLEARPATH_SELECTION==5
#define UWVM_CLEARPATH_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_CLEARPATH_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_clearpath_platform_msgs_msg_Status uwvm_clearpath_state;
#define UWVM_CLEARPATH_STATE_DECODE roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_decode
#define UWVM_CLEARPATH_STATE_ENCODE roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_encode
#define UWVM_CLEARPATH_STATE_CAPACITY ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_MAX_WIRE_BYTES
#elif UWVM_CLEARPATH_SELECTION==6
#define UWVM_CLEARPATH_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_CLEARPATH_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_clearpath_platform_msgs_msg_Status uwvm_clearpath_state;
#define UWVM_CLEARPATH_STATE_DECODE roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_decode
#define UWVM_CLEARPATH_STATE_ENCODE roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_encode
#define UWVM_CLEARPATH_STATE_CAPACITY ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_MAX_WIRE_BYTES
#endif
#if UWVM_CLEARPATH_ROS_VERSION==1
typedef roswasmc_bounded_1_clearpath_platform_msgs_Drive uwvm_clearpath_drive;
#else
typedef roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive uwvm_clearpath_drive;
#endif
/** Complete, serialized caller scratch. Keep output and scratch disjoint. */
typedef struct uwvm_clearpath_storage {unsigned char bytes[UWVM_CLEARPATH_STATE_CAPACITY];} uwvm_clearpath_storage;
/** Pure construction of the original platform Drive velocity value.
 * left/right are wheel radians/second. limit is an explicitly reviewed finite
 * model-specific bound; no model/firmware capability is inferred. Check again
 * after float32 conversion to avoid rounded commands exceeding that bound.
 * This builds data only. It is not the legacy Husky Twist control interface,
 * an emergency stop, acknowledgement, publication or permission acquisition.
 * Every refusal preserves out; there is no implicit retry or recovery.
 */
static inline uint32_t uwvm_clearpath_make_wheel_velocity(double left,double right,double limit,uwvm_clearpath_drive* out){
 uwvm_clearpath_drive next={0};
 if(!out||!isfinite(left)||!isfinite(right)||!isfinite(limit)||limit<=0||limit>FLT_MAX||fabs(left)>limit||fabs(right)>limit)return 22;
 next.mode=0;next.drivers[0]=(float)left;next.drivers[1]=(float)right;
 if(!isfinite(next.drivers[0])||!isfinite(next.drivers[1])||fabs((double)next.drivers[0])>limit||fabs((double)next.drivers[1])>limit)return 22;
 *out=next;return 0;
}
/** Borrow a real explicitly opened capability and decode one whole observation.
 * Idle11 does not update the destination. Storage error90 does not retry or
 * allocate. Primary and cleanup outcomes remain independent at the caller.
 */
static inline uint32_t uwvm_clearpath_poll_state(roswasmc_capability const* cap,uwvm_clearpath_state* out,uwvm_clearpath_storage* scratch){
 roswasmc_result r;
 if(!cap||!cap->context||!cap->context->backend||cap->context->backend->version!=UWVM_CLEARPATH_ROS_VERSION||!out||!scratch||uwvm_bounded_overlap(out,sizeof *out,scratch,sizeof *scratch))return 22;
 r=roswasmc_poll(cap,scratch->bytes,sizeof scratch->bytes);
 if(r.error)return r.error;
 if(!r.has_length||r.length>sizeof scratch->bytes)return 22;
 return UWVM_CLEARPATH_STATE_DECODE(scratch->bytes,(size_t)r.length,out);
}
#endif
