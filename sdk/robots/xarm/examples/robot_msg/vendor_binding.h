/** Complete generated RobotMsg binding for the C/C++ lesson.
 * Include the ACTUAL generated-official-standard bundle; no projected struct,
 * guessed wire bound, native identity, field offset or codec implementation.
 * C and C++ generated records intentionally share this inline C value layout.
 */
#ifndef UWVM_XARM_ROBOT_MSG_LESSON_BINDING_H
#define UWVM_XARM_ROBOT_MSG_LESSON_BINDING_H
#include <string.h>
#include "../../../../custom-types/generated-official-standard/bounded_records.h"
#if UWVM_XARM_ROBOT_MSG_ROS_VERSION == 1
#define UWVM_VENDOR_ROS_VERSION 1
#define ROBOT_RECORD roswasmc_bounded_1_xarm_msgs_RobotMsg
#define ROBOT_DECODE roswasmc_bounded_1_xarm_msgs_RobotMsg_decode
#define VENDOR_MAX_WIRE_BYTES ROSWASMC_BOUNDED_1_XARM_MSGS_ROBOTMSG_MAX_WIRE_BYTES
#elif UWVM_XARM_ROBOT_MSG_ROS_VERSION == 2
#define UWVM_VENDOR_ROS_VERSION 2
#define ROBOT_RECORD roswasmc_bounded_2_xarm_msgs_msg_RobotMsg
#define ROBOT_DECODE roswasmc_bounded_2_xarm_msgs_msg_RobotMsg_decode
#define VENDOR_MAX_WIRE_BYTES ROSWASMC_BOUNDED_2_XARM_MSGS_MSG_ROBOTMSG_MAX_WIRE_BYTES
#else
#error "Select exactly one actual RobotMsg ROS version (1 or 2)"
#endif
typedef ROBOT_RECORD vendor_record;
/** Full independent fixture observation, after the ACTUAL complete decoder.
 * No topic/model/native hash is derived from these values. Fixed native arrays
 * stay six elements; the dynamic seven-angle fixture is checked by exact length.
 */
static inline int xarm_robot_msg_full_value(vendor_record const* v) {
    static float const angle[7]={0.0f,0.25f,-0.5f,1.0f,-1.25f,1.5f,-1.75f};
    static float const pose[6]={125.5f,-250.25f,375.0f,0.25f,-0.5f,0.75f};
    static float const offset[6]={1.0f,2.0f,3.0f,-0.125f,0.25f,-0.5f};
    size_t i;
    if(!v)return 0;
#if UWVM_XARM_ROBOT_MSG_ROS_VERSION == 1
    if(v->header.seq!=19u||v->header.stamp.sec!=42u||v->header.stamp.nsec!=300000000u)return 0;
#else
    if(v->header.stamp.sec!=-2||v->header.stamp.nanosec!=300000000u)return 0;
#endif
    if(v->header.frame_id.size!=9u||memcmp(v->header.frame_id.data,"xarm-base",9u)||
       v->state!=4||v->mode!=0||v->cmdnum!=3||v->mt_brake!=85||v->mt_able!=42||v->err!=7||v->warn!=9||v->angle.size!=7u)return 0;
    for(i=0;i<7u;++i)if(v->angle.data[i]!=angle[i])return 0;
    for(i=0;i<6u;++i)if(v->pose[i]!=pose[i]||v->offset[i]!=offset[i])return 0;
    return 1;
}
static inline uint32_t vendor_decode(void const* bytes,size_t length,vendor_record* out) {
    return ROBOT_DECODE(bytes,length,out);
}
static inline uint32_t vendor_observe_record(vendor_record const* value) {
    return xarm_robot_msg_full_value(value)?0u:22u;
}
#endif
