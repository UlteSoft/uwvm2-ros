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
/** @brief Fixed value for ROS1 example_interfaces_custom/Packet; arrays have exact lengths. */
typedef struct roswasmc_custom_1_example_interfaces_custom_Packet {
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
} roswasmc_custom_1_example_interfaces_custom_Packet;
#define ROSWASMC_CUSTOM_1_EXAMPLE_INTERFACES_CUSTOM_PACKET_WIRE_BYTES 23u
#define ROSWASMC_CUSTOM_1_EXAMPLE_INTERFACES_CUSTOM_PACKET_CODEC_IDENTITY "UWVMROS1_e68d0934f545cd81105ac13682379dc84f023c860c070c046e69c7f70e94d407"
/** @brief Validate all booleans and finite floats without writing or allocating. */
static inline int roswasmc_custom_1_example_interfaces_custom_Packet_valid(roswasmc_custom_1_example_interfaces_custom_Packet const* value) {
    if(!value) return 0;
    if(value->active>1) return 0;
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->measurement,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    return 1;
}
/** @brief Encode one exact record; length and output are preserved on validation failure.
 * Supply separate output storage with capacity >= WIRE_BYTES. */
static inline uint32_t roswasmc_custom_1_example_interfaces_custom_Packet_encode(roswasmc_custom_1_example_interfaces_custom_Packet const* value,void* output,size_t capacity,size_t* length) {
    unsigned char* p=(unsigned char*)output;if(!length||!output||capacity<23u||!roswasmc_custom_1_example_interfaces_custom_Packet_valid(value))return UWVM_CUSTOM_INVALID;
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
static inline uint32_t roswasmc_custom_1_example_interfaces_custom_Packet_decode(void const* input,size_t length,roswasmc_custom_1_example_interfaces_custom_Packet* value) {
    unsigned char const* p=(unsigned char const*)input;roswasmc_custom_1_example_interfaces_custom_Packet next;if(!value||!input||length!=23u)return UWVM_CUSTOM_INVALID;
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
    if(!roswasmc_custom_1_example_interfaces_custom_Packet_valid(&next))return UWVM_CUSTOM_INVALID;*value=next;return 0;
}
/** @brief Fixed value for ROS1 example_interfaces_custom/ScalarRecord; arrays have exact lengths. */
typedef struct roswasmc_custom_1_example_interfaces_custom_ScalarRecord {
    /** @brief Native field enabled; one fixed value. */
    uint8_t enabled;
    /** @brief Native field unsigned_small; one fixed value. */
    uint8_t unsigned_small;
    /** @brief Native field signed_small; one fixed value. */
    int8_t signed_small;
    /** @brief Native field unsigned_medium; one fixed value. */
    uint16_t unsigned_medium;
    /** @brief Native field signed_medium; one fixed value. */
    int16_t signed_medium;
    /** @brief Native field unsigned_word; one fixed value. */
    uint32_t unsigned_word;
    /** @brief Native field signed_word; one fixed value. */
    int32_t signed_word;
    /** @brief Native field unsigned_large; one fixed value. */
    uint64_t unsigned_large;
    /** @brief Native field signed_large; one fixed value. */
    int64_t signed_large;
    /** @brief Native field single_value; one fixed value. */
    float single_value;
    /** @brief Native field double_value; one fixed value. */
    double double_value;
} roswasmc_custom_1_example_interfaces_custom_ScalarRecord;
#define ROSWASMC_CUSTOM_1_EXAMPLE_INTERFACES_CUSTOM_SCALARRECORD_WIRE_BYTES 43u
#define ROSWASMC_CUSTOM_1_EXAMPLE_INTERFACES_CUSTOM_SCALARRECORD_CODEC_IDENTITY "UWVMROS1_dfab9988d24ce356a6a914d4ae6f6a7c8c44dc8118cfc768e41fa8bf47f386c6"
/** @brief Validate all booleans and finite floats without writing or allocating. */
static inline int roswasmc_custom_1_example_interfaces_custom_ScalarRecord_valid(roswasmc_custom_1_example_interfaces_custom_ScalarRecord const* value) {
    if(!value) return 0;
    if(value->enabled>1) return 0;
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->single_value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->double_value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    return 1;
}
/** @brief Encode one exact record; length and output are preserved on validation failure.
 * Supply separate output storage with capacity >= WIRE_BYTES. */
static inline uint32_t roswasmc_custom_1_example_interfaces_custom_ScalarRecord_encode(roswasmc_custom_1_example_interfaces_custom_ScalarRecord const* value,void* output,size_t capacity,size_t* length) {
    unsigned char* p=(unsigned char*)output;if(!length||!output||capacity<43u||!roswasmc_custom_1_example_interfaces_custom_ScalarRecord_valid(value))return UWVM_CUSTOM_INVALID;
    uwvm_custom_put(p+0,(uint64_t)value->enabled,1);
    uwvm_custom_put(p+1,(uint64_t)value->unsigned_small,1);
    uwvm_custom_put(p+2,(uint64_t)value->signed_small,1);
    uwvm_custom_put(p+3,(uint64_t)value->unsigned_medium,2);
    uwvm_custom_put(p+5,(uint64_t)value->signed_medium,2);
    uwvm_custom_put(p+7,(uint64_t)value->unsigned_word,4);
    uwvm_custom_put(p+11,(uint64_t)value->signed_word,4);
    uwvm_custom_put(p+15,(uint64_t)value->unsigned_large,8);
    uwvm_custom_put(p+23,(uint64_t)value->signed_large,8);
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->single_value,4);uwvm_custom_put(p+31,bits,4);}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->double_value,8);uwvm_custom_put(p+35,bits,8);}
    *length=43u;return 0;
}
/** @brief Decode exact canonical bytes transactionally; trailing bytes are refused. */
static inline uint32_t roswasmc_custom_1_example_interfaces_custom_ScalarRecord_decode(void const* input,size_t length,roswasmc_custom_1_example_interfaces_custom_ScalarRecord* value) {
    unsigned char const* p=(unsigned char const*)input;roswasmc_custom_1_example_interfaces_custom_ScalarRecord next;if(!value||!input||length!=43u)return UWVM_CUSTOM_INVALID;
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+0,1);uwvm_custom_copy(&next.enabled,&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+1,1);uwvm_custom_copy(&next.unsigned_small,&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+2,1);uwvm_custom_copy(&next.signed_small,&bits,1);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+3,2);uwvm_custom_copy(&next.unsigned_medium,&bits,2);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+5,2);uwvm_custom_copy(&next.signed_medium,&bits,2);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+7,4);uwvm_custom_copy(&next.unsigned_word,&bits,4);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+11,4);uwvm_custom_copy(&next.signed_word,&bits,4);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+15,8);uwvm_custom_copy(&next.unsigned_large,&bits,8);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+23,8);uwvm_custom_copy(&next.signed_large,&bits,8);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+31,4);uwvm_custom_copy(&next.single_value,&bits,4);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+35,8);uwvm_custom_copy(&next.double_value,&bits,8);}
    if(!roswasmc_custom_1_example_interfaces_custom_ScalarRecord_valid(&next))return UWVM_CUSTOM_INVALID;*value=next;return 0;
}
/** @brief Fixed value for ROS1 example_interfaces_custom/NestedRecord; arrays have exact lengths. */
typedef struct roswasmc_custom_1_example_interfaces_custom_NestedRecord {
    /** @brief Native field readings; exactly 2 elements. */
    roswasmc_custom_1_example_interfaces_custom_ScalarRecord readings[2];
    /** @brief Native field positions; exactly 3 elements. */
    int32_t positions[3];
} roswasmc_custom_1_example_interfaces_custom_NestedRecord;
#define ROSWASMC_CUSTOM_1_EXAMPLE_INTERFACES_CUSTOM_NESTEDRECORD_WIRE_BYTES 98u
#define ROSWASMC_CUSTOM_1_EXAMPLE_INTERFACES_CUSTOM_NESTEDRECORD_CODEC_IDENTITY "UWVMROS1_b0856889a47eb923055e44730a21c04d498472c373a1b9539a8f213a7c09ae5c"
/** @brief Validate all booleans and finite floats without writing or allocating. */
static inline int roswasmc_custom_1_example_interfaces_custom_NestedRecord_valid(roswasmc_custom_1_example_interfaces_custom_NestedRecord const* value) {
    if(!value) return 0;
    if(value->readings[0].enabled>1) return 0;
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->readings[0].single_value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->readings[0].double_value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    if(value->readings[1].enabled>1) return 0;
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->readings[1].single_value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->readings[1].double_value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
    return 1;
}
/** @brief Encode one exact record; length and output are preserved on validation failure.
 * Supply separate output storage with capacity >= WIRE_BYTES. */
static inline uint32_t roswasmc_custom_1_example_interfaces_custom_NestedRecord_encode(roswasmc_custom_1_example_interfaces_custom_NestedRecord const* value,void* output,size_t capacity,size_t* length) {
    unsigned char* p=(unsigned char*)output;if(!length||!output||capacity<98u||!roswasmc_custom_1_example_interfaces_custom_NestedRecord_valid(value))return UWVM_CUSTOM_INVALID;
    uwvm_custom_put(p+0,(uint64_t)value->readings[0].enabled,1);
    uwvm_custom_put(p+1,(uint64_t)value->readings[0].unsigned_small,1);
    uwvm_custom_put(p+2,(uint64_t)value->readings[0].signed_small,1);
    uwvm_custom_put(p+3,(uint64_t)value->readings[0].unsigned_medium,2);
    uwvm_custom_put(p+5,(uint64_t)value->readings[0].signed_medium,2);
    uwvm_custom_put(p+7,(uint64_t)value->readings[0].unsigned_word,4);
    uwvm_custom_put(p+11,(uint64_t)value->readings[0].signed_word,4);
    uwvm_custom_put(p+15,(uint64_t)value->readings[0].unsigned_large,8);
    uwvm_custom_put(p+23,(uint64_t)value->readings[0].signed_large,8);
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->readings[0].single_value,4);uwvm_custom_put(p+31,bits,4);}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->readings[0].double_value,8);uwvm_custom_put(p+35,bits,8);}
    uwvm_custom_put(p+43,(uint64_t)value->readings[1].enabled,1);
    uwvm_custom_put(p+44,(uint64_t)value->readings[1].unsigned_small,1);
    uwvm_custom_put(p+45,(uint64_t)value->readings[1].signed_small,1);
    uwvm_custom_put(p+46,(uint64_t)value->readings[1].unsigned_medium,2);
    uwvm_custom_put(p+48,(uint64_t)value->readings[1].signed_medium,2);
    uwvm_custom_put(p+50,(uint64_t)value->readings[1].unsigned_word,4);
    uwvm_custom_put(p+54,(uint64_t)value->readings[1].signed_word,4);
    uwvm_custom_put(p+58,(uint64_t)value->readings[1].unsigned_large,8);
    uwvm_custom_put(p+66,(uint64_t)value->readings[1].signed_large,8);
    {uint32_t bits=0;uwvm_custom_copy(&bits,&value->readings[1].single_value,4);uwvm_custom_put(p+74,bits,4);}
    {uint64_t bits=0;uwvm_custom_copy(&bits,&value->readings[1].double_value,8);uwvm_custom_put(p+78,bits,8);}
    uwvm_custom_put(p+86,(uint64_t)value->positions[0],4);
    uwvm_custom_put(p+90,(uint64_t)value->positions[1],4);
    uwvm_custom_put(p+94,(uint64_t)value->positions[2],4);
    *length=98u;return 0;
}
/** @brief Decode exact canonical bytes transactionally; trailing bytes are refused. */
static inline uint32_t roswasmc_custom_1_example_interfaces_custom_NestedRecord_decode(void const* input,size_t length,roswasmc_custom_1_example_interfaces_custom_NestedRecord* value) {
    unsigned char const* p=(unsigned char const*)input;roswasmc_custom_1_example_interfaces_custom_NestedRecord next;if(!value||!input||length!=98u)return UWVM_CUSTOM_INVALID;
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+0,1);uwvm_custom_copy(&next.readings[0].enabled,&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+1,1);uwvm_custom_copy(&next.readings[0].unsigned_small,&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+2,1);uwvm_custom_copy(&next.readings[0].signed_small,&bits,1);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+3,2);uwvm_custom_copy(&next.readings[0].unsigned_medium,&bits,2);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+5,2);uwvm_custom_copy(&next.readings[0].signed_medium,&bits,2);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+7,4);uwvm_custom_copy(&next.readings[0].unsigned_word,&bits,4);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+11,4);uwvm_custom_copy(&next.readings[0].signed_word,&bits,4);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+15,8);uwvm_custom_copy(&next.readings[0].unsigned_large,&bits,8);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+23,8);uwvm_custom_copy(&next.readings[0].signed_large,&bits,8);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+31,4);uwvm_custom_copy(&next.readings[0].single_value,&bits,4);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+35,8);uwvm_custom_copy(&next.readings[0].double_value,&bits,8);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+43,1);uwvm_custom_copy(&next.readings[1].enabled,&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+44,1);uwvm_custom_copy(&next.readings[1].unsigned_small,&bits,1);}
    {uint8_t bits=(uint8_t)uwvm_custom_get(p+45,1);uwvm_custom_copy(&next.readings[1].signed_small,&bits,1);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+46,2);uwvm_custom_copy(&next.readings[1].unsigned_medium,&bits,2);}
    {uint16_t bits=(uint16_t)uwvm_custom_get(p+48,2);uwvm_custom_copy(&next.readings[1].signed_medium,&bits,2);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+50,4);uwvm_custom_copy(&next.readings[1].unsigned_word,&bits,4);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+54,4);uwvm_custom_copy(&next.readings[1].signed_word,&bits,4);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+58,8);uwvm_custom_copy(&next.readings[1].unsigned_large,&bits,8);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+66,8);uwvm_custom_copy(&next.readings[1].signed_large,&bits,8);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+74,4);uwvm_custom_copy(&next.readings[1].single_value,&bits,4);}
    {uint64_t bits=(uint64_t)uwvm_custom_get(p+78,8);uwvm_custom_copy(&next.readings[1].double_value,&bits,8);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+86,4);uwvm_custom_copy(&next.positions[0],&bits,4);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+90,4);uwvm_custom_copy(&next.positions[1],&bits,4);}
    {uint32_t bits=(uint32_t)uwvm_custom_get(p+94,4);uwvm_custom_copy(&next.positions[2],&bits,4);}
    if(!roswasmc_custom_1_example_interfaces_custom_NestedRecord_valid(&next))return UWVM_CUSTOM_INVALID;*value=next;return 0;
}
/** @brief Fixed value for ROS2 example_interfaces_custom/msg/FixedSample; arrays have exact lengths. */
typedef struct roswasmc_custom_2_example_interfaces_custom_msg_FixedSample {
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
} roswasmc_custom_2_example_interfaces_custom_msg_FixedSample;
#define ROSWASMC_CUSTOM_2_EXAMPLE_INTERFACES_CUSTOM_MSG_FIXEDSAMPLE_WIRE_BYTES 45u
#define ROSWASMC_CUSTOM_2_EXAMPLE_INTERFACES_CUSTOM_MSG_FIXEDSAMPLE_CODEC_IDENTITY "UWVMROS1_b794a6eb1863626cf279af1fbad1f071fce945d6585f3fdb22979c8f24e32663"
/** @brief Validate all booleans and finite floats without writing or allocating. */
static inline int roswasmc_custom_2_example_interfaces_custom_msg_FixedSample_valid(roswasmc_custom_2_example_interfaces_custom_msg_FixedSample const* value) {
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
static inline uint32_t roswasmc_custom_2_example_interfaces_custom_msg_FixedSample_encode(roswasmc_custom_2_example_interfaces_custom_msg_FixedSample const* value,void* output,size_t capacity,size_t* length) {
    unsigned char* p=(unsigned char*)output;if(!length||!output||capacity<45u||!roswasmc_custom_2_example_interfaces_custom_msg_FixedSample_valid(value))return UWVM_CUSTOM_INVALID;
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
static inline uint32_t roswasmc_custom_2_example_interfaces_custom_msg_FixedSample_decode(void const* input,size_t length,roswasmc_custom_2_example_interfaces_custom_msg_FixedSample* value) {
    unsigned char const* p=(unsigned char const*)input;roswasmc_custom_2_example_interfaces_custom_msg_FixedSample next;if(!value||!input||length!=45u)return UWVM_CUSTOM_INVALID;
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
    if(!roswasmc_custom_2_example_interfaces_custom_msg_FixedSample_valid(&next))return UWVM_CUSTOM_INVALID;*value=next;return 0;
}
#undef UWVM_CUSTOM_ASSERT
#endif
