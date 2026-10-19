/** Complete generated JointState binding for the C/C++ lesson.
 * Include the ACTUAL generated-official-standard bundle; no projected struct,
 * guessed wire bound, native identity, field offset or codec implementation.
 * C and C++ generated records intentionally share this inline C value layout.
 */
#ifndef UWVM_JOINT_STATE_LESSON_BINDING_H
#define UWVM_JOINT_STATE_LESSON_BINDING_H
#include <string.h>
#include "../../../../custom-types/generated-official-standard/bounded_records.h"
#if UWVM_JOINT_STATE_ROS_VERSION == 1
#define UWVM_VENDOR_ROS_VERSION 1
#define JOINT_RECORD roswasmc_bounded_1_sensor_msgs_JointState
#define JOINT_DECODE roswasmc_bounded_1_sensor_msgs_JointState_decode
#define VENDOR_MAX_WIRE_BYTES ROSWASMC_BOUNDED_1_SENSOR_MSGS_JOINTSTATE_MAX_WIRE_BYTES
#elif UWVM_JOINT_STATE_ROS_VERSION == 2
#define UWVM_VENDOR_ROS_VERSION 2
#define JOINT_RECORD roswasmc_bounded_2_sensor_msgs_msg_JointState
#define JOINT_DECODE roswasmc_bounded_2_sensor_msgs_msg_JointState_decode
#define VENDOR_MAX_WIRE_BYTES ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MAX_WIRE_BYTES
#else
#error "Select exactly one actual JointState ROS version (1 or 2)"
#endif
typedef JOINT_RECORD vendor_record;
/** Every active field is compared, including the deliberately EMPTY velocity.
 * Binary fractions below have exact f64 representations; no tolerance hides a
 * conversion or projection. Bounds/Unicode/nonfinite checks belong to the real
 * complete decoder before this fixture-specific independent value check.
 */
static inline int joint_state_full_value(vendor_record const* v) {
    if(!v)return 0;
#if UWVM_JOINT_STATE_ROS_VERSION == 1
    if(v->header.seq!=19u||v->header.stamp.sec!=42u||v->header.stamp.nsec!=300000000u)return 0;
#else
    if(v->header.stamp.sec!=-2||v->header.stamp.nanosec!=300000000u)return 0;
#endif
    return v->header.frame_id.size==4u&&!memcmp(v->header.frame_id.data,"base",4u)&&
        v->name.size==2u&&v->name.data[0].size==4u&&!memcmp(v->name.data[0].data,"left",4u)&&
        v->name.data[1].size==5u&&!memcmp(v->name.data[1].data,"right",5u)&&
        v->position.size==2u&&v->position.data[0]==1.25&&v->position.data[1]==-2.5&&
        v->velocity.size==0u&&v->effort.size==2u&&v->effort.data[0]==0.5&&v->effort.data[1]==-0.25;
}
static inline uint32_t vendor_decode(void const* bytes,size_t length,vendor_record* out) {
    return JOINT_DECODE(bytes,length,out);
}
static inline uint32_t vendor_observe_record(vendor_record const* value) {
    return joint_state_full_value(value)?0u:22u;
}
#endif
