/** @file Generated fixed ROS topic records; no I/O, native serialization or authority.
 * Encode validates the complete value before modifying output. Decode preserves
 * the destination on refusal. Caller buffers and records must not overlap. */
#ifndef UWVM_ROS_CUSTOM_FIXED_RECORDS_H
#define UWVM_ROS_CUSTOM_FIXED_RECORDS_H
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include <limits.h>
#define UWVM_CUSTOM_INVALID 22u
#ifdef __cplusplus
#define UWVM_CUSTOM_ASSERT(x) static_assert(x,"canonical scalar representation required")
#else
#define UWVM_CUSTOM_ASSERT(x) _Static_assert(x,"canonical scalar representation required")
#endif
UWVM_CUSTOM_ASSERT(CHAR_BIT==8 && sizeof(float)==4 && sizeof(double)==8 && FLT_RADIX==2 && FLT_MANT_DIG==24 && DBL_MANT_DIG==53 && FLT_MAX_EXP==128 && DBL_MAX_EXP==1024);
/** @brief Copy fixed scalar object bits; compilers optimize this intrinsic, with a portable freestanding fallback. */
static inline void uwvm_custom_copy(void* out,void const* in,size_t n) {
#if defined(__GNUC__) || defined(__clang__)
__builtin_memcpy(out,in,n);
#else
size_t i;for(i=0;i<n;++i)((unsigned char*)out)[i]=((unsigned char const*)in)[i];
#endif
}
/** @brief Read one already range-checked LE scalar; no allocation occurs. */
static inline uint64_t uwvm_custom_get(unsigned char const* p,unsigned n) {uint64_t v=0;unsigned i;for(i=0;i<n;++i)v|=(uint64_t)p[i]<<(8*i);return v;}
/** @brief Store the low n bytes of one scalar into prevalidated storage. */
static inline void uwvm_custom_put(unsigned char* p,uint64_t v,unsigned n) {unsigned i;for(i=0;i<n;++i)p[i]=(unsigned char)(v>>(8*i));}
/** @brief Fixed value for ROS1 uwvm_custom_ros1_fixture/Packet; arrays have exact lengths. */
typedef struct roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet {
    /** @brief Native field sequence; one fixed value. */
    int32_t sequence;
    /** @brief Native field active; one fixed value. */
    uint8_t active;
    /** @brief Native field measurement; one fixed value. */
    double measurement;
    /** @brief Native field tag; exactly 4 elements. */
    uint8_t tag[4];
    /** @brief Native field samples; exactly 3 elements. */
    int16_t samples[3];
} roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet;
#define ROSWASMC_CUSTOM_1_UWVM_CUSTOM_ROS1_FIXTURE_PACKET_WIRE_BYTES 23u
#define ROSWASMC_CUSTOM_1_UWVM_CUSTOM_ROS1_FIXTURE_PACKET_CODEC_IDENTITY "UWVMROS1_eb0eef4515ec546678bae4030eb858c151af98afa1bfa3ca08b3a0e02bd48544"
/** @brief Validate all booleans and finite floats without writing or allocating. */
static inline int roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_valid(roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet const* value) {
    if(!value) return 0;
    if(value->active>1) return 0;
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->measurement,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    return 1;
}
/** @brief Encode one exact record; length and output are preserved on validation failure.
 * Supply separate output storage with capacity >= WIRE_BYTES. */
static inline uint32_t roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_encode(roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet const* value,void* output,size_t capacity,size_t* length) {
    unsigned char* p=(unsigned char*)output;if(!length||!output||capacity<23u||!roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_valid(value))return UWVM_CUSTOM_INVALID;
    uwvm_custom_put(p+0,(uint64_t)value->sequence,4);
    uwvm_custom_put(p+4,(uint64_t)value->active,1);
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->measurement,8);uwvm_custom_put(p+5,bits,8);}
    uwvm_custom_put(p+13,(uint64_t)value->tag[0],1);
    uwvm_custom_put(p+14,(uint64_t)value->tag[1],1);
    uwvm_custom_put(p+15,(uint64_t)value->tag[2],1);
    uwvm_custom_put(p+16,(uint64_t)value->tag[3],1);
    uwvm_custom_put(p+17,(uint64_t)value->samples[0],2);
    uwvm_custom_put(p+19,(uint64_t)value->samples[1],2);
    uwvm_custom_put(p+21,(uint64_t)value->samples[2],2);
    *length=23u;return 0;
}
/** @brief Decode exact canonical bytes transactionally; trailing bytes are refused. */
static inline uint32_t roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_decode(void const* input,size_t length,roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet* value) {
    unsigned char const* p=(unsigned char const*)input;roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet next;if(!value||!input||length!=23u)return UWVM_CUSTOM_INVALID;
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+0,4);uwvm_custom_copy(&next.sequence,&bits,4);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+4,1);uwvm_custom_copy(&next.active,&bits,1);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+5,8);uwvm_custom_copy(&next.measurement,&bits,8);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+13,1);uwvm_custom_copy(&next.tag[0],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+14,1);uwvm_custom_copy(&next.tag[1],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+15,1);uwvm_custom_copy(&next.tag[2],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+16,1);uwvm_custom_copy(&next.tag[3],&bits,1);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+17,2);uwvm_custom_copy(&next.samples[0],&bits,2);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+19,2);uwvm_custom_copy(&next.samples[1],&bits,2);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+21,2);uwvm_custom_copy(&next.samples[2],&bits,2);}
    if(!roswasmc_custom_1_uwvm_custom_ros1_fixture_Packet_valid(&next))return UWVM_CUSTOM_INVALID;*value=next;return 0;
}
/** @brief Fixed value for ROS2 uwvm_ros_custom_test_msgs/msg/FixedSample; arrays have exact lengths. */
typedef struct roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample {
    /** @brief Native field sequence; one fixed value. */
    uint32_t sequence;
    /** @brief Native field enabled; one fixed value. */
    uint8_t enabled;
    /** @brief Native field temperature; one fixed value. */
    float temperature;
    /** @brief Native field acceleration; exactly 3 elements. */
    double acceleration[3];
    /** @brief Native field tag; exactly 8 elements. */
    uint8_t tag[8];
    /** @brief Native field thresholds; exactly 2 elements. */
    int16_t thresholds[2];
} roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample;
#define ROSWASMC_CUSTOM_2_UWVM_ROS_CUSTOM_TEST_MSGS_MSG_FIXEDSAMPLE_WIRE_BYTES 45u
#define ROSWASMC_CUSTOM_2_UWVM_ROS_CUSTOM_TEST_MSGS_MSG_FIXEDSAMPLE_CODEC_IDENTITY "UWVMROS1_3f390efa2dccaf91a53c14a50fcead8790bb1bbc103c6ac6b29f176d045e2516"
/** @brief Validate all booleans and finite floats without writing or allocating. */
static inline int roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_valid(roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample const* value) {
    if(!value) return 0;
    if(value->enabled>1) return 0;
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->acceleration[0],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->acceleration[1],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->acceleration[2],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    return 1;
}
/** @brief Encode one exact record; length and output are preserved on validation failure.
 * Supply separate output storage with capacity >= WIRE_BYTES. */
static inline uint32_t roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_encode(roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample const* value,void* output,size_t capacity,size_t* length) {
    unsigned char* p=(unsigned char*)output;if(!length||!output||capacity<45u||!roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_valid(value))return UWVM_CUSTOM_INVALID;
    uwvm_custom_put(p+0,(uint64_t)value->sequence,4);
    uwvm_custom_put(p+4,(uint64_t)value->enabled,1);
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->temperature,4);uwvm_custom_put(p+5,bits,4);}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->acceleration[0],8);uwvm_custom_put(p+9,bits,8);}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->acceleration[1],8);uwvm_custom_put(p+17,bits,8);}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->acceleration[2],8);uwvm_custom_put(p+25,bits,8);}
    uwvm_custom_put(p+33,(uint64_t)value->tag[0],1);
    uwvm_custom_put(p+34,(uint64_t)value->tag[1],1);
    uwvm_custom_put(p+35,(uint64_t)value->tag[2],1);
    uwvm_custom_put(p+36,(uint64_t)value->tag[3],1);
    uwvm_custom_put(p+37,(uint64_t)value->tag[4],1);
    uwvm_custom_put(p+38,(uint64_t)value->tag[5],1);
    uwvm_custom_put(p+39,(uint64_t)value->tag[6],1);
    uwvm_custom_put(p+40,(uint64_t)value->tag[7],1);
    uwvm_custom_put(p+41,(uint64_t)value->thresholds[0],2);
    uwvm_custom_put(p+43,(uint64_t)value->thresholds[1],2);
    *length=45u;return 0;
}
/** @brief Decode exact canonical bytes transactionally; trailing bytes are refused. */
static inline uint32_t roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_decode(void const* input,size_t length,roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample* value) {
    unsigned char const* p=(unsigned char const*)input;roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample next;if(!value||!input||length!=45u)return UWVM_CUSTOM_INVALID;
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+0,4);uwvm_custom_copy(&next.sequence,&bits,4);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+4,1);uwvm_custom_copy(&next.enabled,&bits,1);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+5,4);uwvm_custom_copy(&next.temperature,&bits,4);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+9,8);uwvm_custom_copy(&next.acceleration[0],&bits,8);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+17,8);uwvm_custom_copy(&next.acceleration[1],&bits,8);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+25,8);uwvm_custom_copy(&next.acceleration[2],&bits,8);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+33,1);uwvm_custom_copy(&next.tag[0],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+34,1);uwvm_custom_copy(&next.tag[1],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+35,1);uwvm_custom_copy(&next.tag[2],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+36,1);uwvm_custom_copy(&next.tag[3],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+37,1);uwvm_custom_copy(&next.tag[4],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+38,1);uwvm_custom_copy(&next.tag[5],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+39,1);uwvm_custom_copy(&next.tag[6],&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+40,1);uwvm_custom_copy(&next.tag[7],&bits,1);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+41,2);uwvm_custom_copy(&next.thresholds[0],&bits,2);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+43,2);uwvm_custom_copy(&next.thresholds[1],&bits,2);}
    if(!roswasmc_custom_2_uwvm_ros_custom_test_msgs_msg_FixedSample_valid(&next))return UWVM_CUSTOM_INVALID;*value=next;return 0;
}
#undef UWVM_CUSTOM_ASSERT
#endif
