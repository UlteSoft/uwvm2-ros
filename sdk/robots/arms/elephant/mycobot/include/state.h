/* SPDX-License-Identifier: Apache-2.0
 * Complete original model-specific angular observations over controlled imports.
 * Select exactly one source-locked distribution/model combination. The complete
 * source domain retains all original message/service fields for every package.
 */
#ifndef UWVM_ELEPHANT_STATE_H
#define UWVM_ELEPHANT_STATE_H
#include <math.h>
#include <float.h>
#include <roswasm/codec.h>
#include "bounded_records.h"
#if !defined(UWVM_ELEPHANT_SELECTION) || UWVM_ELEPHANT_SELECTION<1 || UWVM_ELEPHANT_SELECTION>30
#error "Select exactly one original Elephant model and matching complete source domain"
#endif
#if UWVM_ELEPHANT_SELECTION==1
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mycobot_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mycobot_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mycobot_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==2
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mypalletizer_communication_MypalAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mypalletizer_communication_MypalAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mypalletizer_communication_MypalAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==3
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mecharm_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mecharm_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mecharm_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==4
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_myarm_communication_MyArmAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_myarm_communication_MyArmAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_myarm_communication_MyArmAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 7
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
case 6:return (double)value->joint_7;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
case 6:if(fabs(x)>FLT_MAX)return 22;value->joint_7=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==5
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mybuddy_communication_MybuddyAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 13
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_0;
case 1:return (double)value->joint_1;
case 2:return (double)value->joint_2;
case 3:return (double)value->joint_3;
case 4:return (double)value->joint_4;
case 5:return (double)value->joint_5;
case 6:return (double)value->joint_6;
case 7:return (double)value->joint_7;
case 8:return (double)value->joint_8;
case 9:return (double)value->joint_9;
case 10:return (double)value->joint_10;
case 11:return (double)value->joint_11;
case 12:return (double)value->joint_12;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_0=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 6:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
case 7:if(fabs(x)>FLT_MAX)return 22;value->joint_7=(float)x;return 0;
case 8:if(fabs(x)>FLT_MAX)return 22;value->joint_8=(float)x;return 0;
case 9:if(fabs(x)>FLT_MAX)return 22;value->joint_9=(float)x;return 0;
case 10:if(fabs(x)>FLT_MAX)return 22;value->joint_10=(float)x;return 0;
case 11:if(fabs(x)>FLT_MAX)return 22;value->joint_11=(float)x;return 0;
case 12:if(fabs(x)>FLT_MAX)return 22;value->joint_12=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==6
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mycobot_320_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==7
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mycobot_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mycobot_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mycobot_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==8
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mypalletizer_communication_MypalAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mypalletizer_communication_MypalAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mypalletizer_communication_MypalAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==9
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_ultraarm_communication_ultraArmAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_ultraarm_communication_ultraArmAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_ultraarm_communication_ultraArmAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_ULTRAARMANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 3
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==10
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mecharm_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mecharm_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mecharm_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==11
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_myarm_communication_MyArmAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_myarm_communication_MyArmAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_myarm_communication_MyArmAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 7
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
case 6:return (double)value->joint_7;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
case 6:if(fabs(x)>FLT_MAX)return 22;value->joint_7=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==12
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mybuddy_communication_MybuddyAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 13
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_0;
case 1:return (double)value->joint_1;
case 2:return (double)value->joint_2;
case 3:return (double)value->joint_3;
case 4:return (double)value->joint_4;
case 5:return (double)value->joint_5;
case 6:return (double)value->joint_6;
case 7:return (double)value->joint_7;
case 8:return (double)value->joint_8;
case 9:return (double)value->joint_9;
case 10:return (double)value->joint_10;
case 11:return (double)value->joint_11;
case 12:return (double)value->joint_12;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_0=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 6:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
case 7:if(fabs(x)>FLT_MAX)return 22;value->joint_7=(float)x;return 0;
case 8:if(fabs(x)>FLT_MAX)return 22;value->joint_8=(float)x;return 0;
case 9:if(fabs(x)>FLT_MAX)return 22;value->joint_9=(float)x;return 0;
case 10:if(fabs(x)>FLT_MAX)return 22;value->joint_10=(float)x;return 0;
case 11:if(fabs(x)>FLT_MAX)return 22;value->joint_11=(float)x;return 0;
case 12:if(fabs(x)>FLT_MAX)return 22;value->joint_12=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==13
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mycobot_320_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==14
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mycobot_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mycobot_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mycobot_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==15
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==16
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_mypalletizer_communication_MypalAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_mypalletizer_communication_MypalAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_mypalletizer_communication_MypalAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==17
#define UWVM_ELEPHANT_ROS_VERSION 1
#include <roswasm/ros1.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros1_backend
typedef roswasmc_bounded_1_ultraarm_communication_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_1_ultraarm_communication_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_1_ultraarm_communication_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==18
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==19
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==20
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==21
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==22
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==23
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==24
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==25
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==26
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==27
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==28
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 6
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
case 4:return (double)value->joint_5;
case 5:return (double)value->joint_6;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
case 4:if(fabs(x)>FLT_MAX)return 22;value->joint_5=(float)x;return 0;
case 5:if(fabs(x)>FLT_MAX)return 22;value->joint_6=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==29
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#elif UWVM_ELEPHANT_SELECTION==30
#define UWVM_ELEPHANT_ROS_VERSION 2
#include <roswasm/ros2.h>
#define UWVM_ELEPHANT_BACKEND roswasmc_ros2_backend
typedef roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotAngles uwvm_elephant_state;
#define UWVM_ELEPHANT_STATE_DECODE roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotAngles_decode
#define UWVM_ELEPHANT_STATE_ENCODE roswasmc_bounded_2_ultraarm_p1_interfaces_msg_MycobotAngles_encode
#define UWVM_ELEPHANT_STATE_CAPACITY ROSWASMC_BOUNDED_2_ULTRAARM_P1_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES
#define UWVM_ELEPHANT_COMPONENTS 4
/** Exact original declaration order; no joint or model projection. */
static inline double uwvm_elephant_component(uwvm_elephant_state const* value,size_t index){switch(index){
case 0:return (double)value->joint_1;
case 1:return (double)value->joint_2;
case 2:return (double)value->joint_3;
case 3:return (double)value->joint_4;
default:return NAN;}}
/** Private checked scalar conversion; complete commit occurs below. */
static inline uint32_t uwvm_elephant_assign(uwvm_elephant_state* value,size_t index,double x){switch(index){
case 0:if(fabs(x)>FLT_MAX)return 22;value->joint_1=(float)x;return 0;
case 1:if(fabs(x)>FLT_MAX)return 22;value->joint_2=(float)x;return 0;
case 2:if(fabs(x)>FLT_MAX)return 22;value->joint_3=(float)x;return 0;
case 3:if(fabs(x)>FLT_MAX)return 22;value->joint_4=(float)x;return 0;
default:return 22;}}
#endif
/** Caller-owned whole-value storage. Serialize use of a borrowed capability. */
typedef struct uwvm_elephant_storage {unsigned char bytes[UWVM_ELEPHANT_STATE_CAPACITY];} uwvm_elephant_storage;
/** Pure construction of one complete original observation.
 * Arrays contain exactly UWVM_ELEPHANT_COMPONENTS entries in original field
 * declaration order. Values and reviewed min/max use the selected driver's
 * original units; no degrees/radians, coordinate frame or model conversion is
 * inferred. Every bound is explicit and finite. Recheck rounded float32 values.
 * This constructs data only, not a motion command, publication, acknowledgement
 * or emergency stop. It grants no rights and preserves out on every refusal.
 */
static inline uint32_t uwvm_elephant_make_observation(double const* values,
 double const* minimum,double const* maximum,uwvm_elephant_state* out){
 uwvm_elephant_state next={0};
 if(!values||!minimum||!maximum||!out)return 22;
 for(size_t i=0;i<UWVM_ELEPHANT_COMPONENTS;++i){
  if(!isfinite(values[i])||!isfinite(minimum[i])||!isfinite(maximum[i])||
     minimum[i]>maximum[i]||values[i]<minimum[i]||values[i]>maximum[i])return 22;
  if(uwvm_elephant_assign(&next,i,values[i]))return 22;
  double rounded=uwvm_elephant_component(&next,i);
  if(!isfinite(rounded)||rounded<minimum[i]||rounded>maximum[i])return 22;
 }
 *out=next;return 0;
}
/** One full checked decode over an explicitly acquired actual ROS capability.
 * Idle11 never publishes a new observation. Error90 never allocates or retries.
 * Keep out/scratch disjoint; close remains cleanup rather than physical stop.
 */
static inline uint32_t uwvm_elephant_poll_state(roswasmc_capability const* cap,
 uwvm_elephant_state* out,uwvm_elephant_storage* scratch){
 roswasmc_result r;
 if(!cap||!cap->context||!cap->context->backend||
 cap->context->backend->version!=UWVM_ELEPHANT_ROS_VERSION||!out||!scratch||
 uwvm_bounded_overlap(out,sizeof *out,scratch,sizeof *scratch))return 22;
 r=roswasmc_poll(cap,scratch->bytes,sizeof scratch->bytes);
 if(r.error)return r.error;
 if(!r.has_length||r.length>sizeof scratch->bytes)return 22;
 return UWVM_ELEPHANT_STATE_DECODE(scratch->bytes,(size_t)r.length,out);
}
#endif
