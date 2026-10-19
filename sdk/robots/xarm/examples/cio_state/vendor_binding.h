/** Complete generated CIOState binding for the C/C++ lesson.
 * Include the ACTUAL generated-official-standard bundle; no projected struct,
 * guessed wire bound, native identity, field offset or codec implementation.
 * C and C++ generated records intentionally share this inline C value layout.
 */
#ifndef UWVM_XARM_CIO_STATE_LESSON_BINDING_H
#define UWVM_XARM_CIO_STATE_LESSON_BINDING_H
#include <string.h>
#include "../../../../custom-types/generated-official-standard/bounded_records.h"
#if UWVM_XARM_CIO_STATE_ROS_VERSION == 1
#define UWVM_VENDOR_ROS_VERSION 1
#define CIO_RECORD roswasmc_bounded_1_xarm_msgs_CIOState
#define CIO_DECODE roswasmc_bounded_1_xarm_msgs_CIOState_decode
#define VENDOR_MAX_WIRE_BYTES ROSWASMC_BOUNDED_1_XARM_MSGS_CIOSTATE_MAX_WIRE_BYTES
#elif UWVM_XARM_CIO_STATE_ROS_VERSION == 2
#define UWVM_VENDOR_ROS_VERSION 2
#define CIO_RECORD roswasmc_bounded_2_xarm_msgs_msg_CIOState
#define CIO_DECODE roswasmc_bounded_2_xarm_msgs_msg_CIOState_decode
#define VENDOR_MAX_WIRE_BYTES ROSWASMC_BOUNDED_2_XARM_MSGS_MSG_CIOSTATE_MAX_WIRE_BYTES
#else
#error "Select exactly one actual CIOState ROS version (1 or 2)"
#endif
typedef CIO_RECORD vendor_record;
/** Observe every native-declared field only AFTER complete generated decode.
 * Digital bit patterns remain uint16; all sixteen configuration bytes remain
 * uint8. No field is projected to booleans or used as a hardware command.
 */
static inline int xarm_cio_state_full_value(vendor_record const* v) {
    static uint16_t const input_digitals[2]={165u,32769u},output_digitals[2]={90u,16386u};
    static float const input_analogs[2]={1.25f,2.5f},output_analogs[2]={3.75f,0.5f};
    size_t i;if(!v)return 0;
#if UWVM_XARM_CIO_STATE_ROS_VERSION == 1
    if(v->header.seq!=19u||v->header.stamp.sec!=42u||v->header.stamp.nsec!=300000000u)return 0;
#else
    if(v->header.stamp.sec!=-2||v->header.stamp.nanosec!=300000000u)return 0;
#endif
    if(v->header.frame_id.size!=9u||memcmp(v->header.frame_id.data,"xarm-gpio",9u)||v->state!=1||v->code!=0)return 0;
    for(i=0;i<2u;++i)if(v->input_digitals[i]!=input_digitals[i]||v->output_digitals[i]!=output_digitals[i]||v->input_analogs[i]!=input_analogs[i]||v->output_analogs[i]!=output_analogs[i])return 0;
    for(i=0;i<16u;++i)if(v->input_conf[i]!=i||v->output_conf[i]!=15u-i)return 0;
    return 1;
}
static inline uint32_t vendor_decode(void const* bytes,size_t length,vendor_record* out) {
    return CIO_DECODE(bytes,length,out);
}
static inline uint32_t vendor_observe_record(vendor_record const* value) {
    return xarm_cio_state_full_value(value)?0u:22u;
}
#endif
