/** @file Generated bounded-v2 canonical values; no native ROS serialization or authority.
 * Every dynamic field has inline caller-owned capacity. Encode validates and measures
 * the full value before writes; decode commits one temporary only after full validation.
 * Supply distinct record, byte and size-output storage; overlap is explicitly refused.
 * Empty Service/Action components use a C layout dummy that is never a wire field. */
#ifndef UWVM_ROS_BOUNDED_RECORDS_H
#define UWVM_ROS_BOUNDED_RECORDS_H
#include <stdint.h>
#include <stddef.h>
#include <float.h>
#include <limits.h>
#define UWVM_BOUNDED_INVALID 22u
#define UWVM_BOUNDED_TOO_LARGE 90u
#if defined(__cplusplus)
#define UWVM_BOUNDED_ASSERT(x) static_assert(x,"canonical representation required")
#else
#define UWVM_BOUNDED_ASSERT(x) _Static_assert(x,"canonical representation required")
#endif
UWVM_BOUNDED_ASSERT(CHAR_BIT==8&&sizeof(float)==4&&sizeof(double)==8&&FLT_RADIX==2&&FLT_MANT_DIG==24&&DBL_MANT_DIG==53&&FLT_MAX_EXP==128&&DBL_MAX_EXP==1024);
/** Copy exact primitive/string bytes; compiler intrinsics avoid byte-at-a-time copies. */
static inline void uwvm_bounded_copy(void* out,void const* in,size_t n){
#if defined(__GNUC__) || defined(__clang__)
__builtin_memcpy(out,in,n);
#else
size_t i;for(i=0;i<n;++i)((unsigned char*)out)[i]=((unsigned char const*)in)[i];
#endif
}
static inline uint64_t uwvm_bounded_get(unsigned char const* p,unsigned n){uint64_t v=0;unsigned i;for(i=0;i<n;++i)v|=(uint64_t)p[i]<<(8*i);return v;}
static inline void uwvm_bounded_put(unsigned char* p,uint64_t v,unsigned n){unsigned i;for(i=0;i<n;++i)p[i]=(unsigned char)(v>>(8*i));}
/** Strict Unicode scalar UTF-8, rejecting embedded NUL as required by this profile. */
static inline int uwvm_bounded_utf8(unsigned char const* p,size_t n){size_t i=0;while(i<n){unsigned b=p[i++],k,code,minimum;if(!b)return 0;if(b<128)continue;if(b>=194&&b<=223){k=1;code=b&31;minimum=128;}else if(b>=224&&b<=239){k=2;code=b&15;minimum=2048;}else if(b>=240&&b<=244){k=3;code=b&7;minimum=65536;}else return 0;if(k>n-i)return 0;while(k--){b=p[i++];if((b&192)!=128)return 0;code=(code<<6)|(b&63);}if(code<minimum||code>1114111||(code>=55296&&code<=57343))return 0;}return 1;}
static inline int uwvm_bounded_overlap(void const* a,size_t an,void const* b,size_t bn){uintptr_t x=(uintptr_t)a,y=(uintptr_t)b;if((an&&!a)||(bn&&!b)||an>UINTPTR_MAX-x||bn>UINTPTR_MAX-y)return 1;return an&&bn&&x<y+bn&&y<x+an;}
/** Complete ROS2 mecharm_interfaces/msg/MecharmAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
} roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMANGLES_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMANGLES_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMANGLES_CODEC_IDENTITY "UWVMROS1_37d12e0fc3b7bbb50bdcee0ca624a5c637f9554f3c64d640f538aeda57417331"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_valid(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_wire_size(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_write_fields(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_encode(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles* value) {
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/msg/MecharmCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
} roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMCOORDS_CODEC_IDENTITY "UWVMROS1_e1bb96b1908c796e2a89eb4dd2402a6cc02bcc5077c52268bb378a52dd3029ed"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_valid(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_wire_size(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_write_fields(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_encode(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords* value) {
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/msg/MecharmGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_2123b61d7b9ec218ee074473c26e5ec82fde818f40796047fc369cb84516e046"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_valid(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_wire_size(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_write_fields(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_encode(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus* value) {
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/msg/MecharmPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
  /** Declared field pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin1;
  /** Declared field pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin2;
} roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_4d44c84c1fdd3f4db92d9975545348bfd727320e63c9fbfde5adf0b81ca04add"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_valid(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_wire_size(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_write_fields(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_encode(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus* value) {
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/msg/MecharmSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMSETANGLES_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMSETANGLES_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMSETANGLES_CODEC_IDENTITY "UWVMROS1_54b634e4c5d0bc9fdf04a98d31816e24d3a9c5eb497bb38fb16e2de593cf81f5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_valid(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_wire_size(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_write_fields(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_encode(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles* value) {
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/msg/MecharmSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  int8_t model;
} roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMSETCOORDS_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMSETCOORDS_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_MSG_MECHARMSETCOORDS_CODEC_IDENTITY "UWVMROS1_44944ee22cd4254167083cfdcd7377ee2f59dbe27414932c980670a471a6e7c2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_valid(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_wire_size(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_write_fields(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->model,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_encode(roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords* value) {
  roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_msg_MecharmSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/GetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETANGLES_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETANGLES_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_95b1d7786c03c6fec98fec0706088481fe9c7aa48c9f33f4325031d0437d97df"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_valid(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request* value) {
  return roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_encode(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/GetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
} roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETANGLES_RESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETANGLES_RESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_b1a035847ac7a888053ba8de7494abb6d2cdc01f61e4d0a51cb3ef6f479adf61"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_valid(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_encode(roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/GetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETCOORDS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETCOORDS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_353827e7125cfa6c829f1138cc33901099a26cf73a8d045cf806b427994ccef1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_valid(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request* value) {
  return roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_encode(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/GetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
} roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETCOORDS_RESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETCOORDS_RESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_f7fcb8c4fdb47c06e93a8f67613490f7ab0ef2af7efb02092874722670b36c75"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_valid(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_encode(roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_GetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/GripperStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_044bf47303f5da0d0e17e795b8567b714d55680e8144c8e2e71ff7032859dfbb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_valid(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_encode(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/GripperStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_5aeee3c79bb6ebe4a4cb77f54552e97dcc155c65151a5965761a360d41acbea3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_valid(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_encode(roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_GripperStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/PumpStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
  /** Declared field pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin1;
  /** Declared field pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin2;
} roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_PUMPSTATUS_REQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_PUMPSTATUS_REQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_PUMPSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_f80774682b7b1d447ff2f8dd7f6ac2f917dd4f71ccd3b1685a57b579fa41d6b9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_valid(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_encode(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/PumpStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_PUMPSTATUS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_PUMPSTATUS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_PUMPSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_935c0e2aaca3f1e8aad8e4487484292b3d30ccdff180a9aaa50f454f75ecff1b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_valid(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_encode(roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_PumpStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/SetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETANGLES_REQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETANGLES_REQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_86897875ef9a609098bc1eabba63aec7d960de380db98370e7a67626b2253eeb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_valid(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_encode(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/SetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETANGLES_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETANGLES_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_4782fe6a137dee35bf39d863be0aea118f61dcef092c11b013c44c6f4c129620"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_valid(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_encode(roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/SetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  int8_t model;
} roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETCOORDS_REQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETCOORDS_REQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_a0daefe14076b73f1a64924ffc15a6b9647abe1b1887377e642f41b1a7920916"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_valid(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->model,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_encode(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mecharm_interfaces/srv/SetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response;
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETCOORDS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETCOORDS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MECHARM_INTERFACES_SRV_SETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_43d02cfe61b6fb08f035e1f8ea78496cdcb488ed4c916f528415dce4ab810b7f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_valid(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_wire_size(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_write_fields(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_encode(roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response* value) {
  roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mecharm_interfaces_srv_SetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/msg/MycobotAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
} roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTANGLES_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTANGLES_CODEC_IDENTITY "UWVMROS1_38f099339ed5ee9fe56a5b44c3f0849e2ff7ff721b2a95b5cf49e3b92f6a9d82"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_valid(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_wire_size(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_write_fields(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_encode(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles* value) {
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/msg/MycobotCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
} roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTCOORDS_CODEC_IDENTITY "UWVMROS1_8d9606c561db7680e816f5a19fd148c374f271acee8b261f7d060f370e0399d7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_valid(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_wire_size(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_write_fields(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_encode(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords* value) {
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/msg/MycobotGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_4dc3ab413af3da01e26a3289ef9e5ef39488d00483631d701b4d617d9c9ba33d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_valid(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_wire_size(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_write_fields(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_encode(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus* value) {
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/msg/MycobotPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
  /** Declared field pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin1;
  /** Declared field pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin2;
} roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_860004dc3975ab44b5a1b0a77a9ff01ab58bfc71d7142d842d3bf9c636365e58"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_valid(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_wire_size(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_write_fields(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_encode(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus* value) {
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/msg/MycobotSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTSETANGLES_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTSETANGLES_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTSETANGLES_CODEC_IDENTITY "UWVMROS1_798576c5f0b03e46e5cc5d2edf81bc7042042482ef574643ca136621346d731e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_valid(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_wire_size(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_write_fields(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_encode(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles* value) {
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/msg/MycobotSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  int8_t model;
} roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTSETCOORDS_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTSETCOORDS_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_MSG_MYCOBOTSETCOORDS_CODEC_IDENTITY "UWVMROS1_462b7629172037a6959d08984896980c8c07ae5b6364c6d848b172dea23cab49"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_valid(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_wire_size(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_write_fields(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->model,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_encode(roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords* value) {
  roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_msg_MycobotSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/GetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETANGLES_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETANGLES_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_ea433bdb6e3cb77923720a56bec24aaec62c88480248a4ed425370092474d38e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_valid(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request* value) {
  return roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_encode(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/GetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
} roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETANGLES_RESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETANGLES_RESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_14d6e2d8a6f7234719fae06d8807c651965387e252160df555ede0a070123b8a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_valid(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_encode(roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/GetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETCOORDS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETCOORDS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_31ef03eefc143e9d040572e378a5803f44f68bac6d03fbb44ca27f87fb5fb076"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_valid(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request* value) {
  return roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_encode(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/GetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
} roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETCOORDS_RESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETCOORDS_RESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_c9886b5cc5f3c48116a1c884f75ab4af49be46e2c0476ffc3aa8167ff96f7787"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_valid(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_encode(roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_GetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/GripperStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_6c5ae66e66f12834b7d623e07a5b3ef2b8127cd9cd83cb36d6793a68d921fcd5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_valid(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_encode(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/GripperStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_530f1e1acaa8cda6b43f8a8c0a38f5376eaebce3e99f7fb194a28143bed2d1ab"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_valid(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_encode(roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_GripperStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/PumpStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
  /** Declared field pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin1;
  /** Declared field pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin2;
} roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_PUMPSTATUS_REQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_PUMPSTATUS_REQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_PUMPSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_50b7fa285b93dd9e6b3b0eafda0afeb5911ab6e3b79d58f03f1cf1f76e732cd0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_valid(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_encode(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/PumpStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_PUMPSTATUS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_PUMPSTATUS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_PUMPSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_b347313e1d4430f4e78a72fb8c35d53a39d1d391290373f84f1f8b2b44375eb0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_valid(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_encode(roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_PumpStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/SetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETANGLES_REQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETANGLES_REQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_a06efd8b6af878d55c26fda8a7177a0b6c8b687f7e1b9f2e17e2ecd19c440310"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_valid(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_encode(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/SetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETANGLES_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETANGLES_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_af4daae72e9ec28c8848b76b556c9387e65ba9698d8f0f6e5ba17522472ab665"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_valid(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_encode(roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/SetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  int8_t model;
} roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETCOORDS_REQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETCOORDS_REQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_76a98d5db6f98255046f13855dc7468049894805d83e3d3a7fbdaace40292213"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_valid(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->model,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_encode(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_interfaces/srv/SetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETCOORDS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETCOORDS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_INTERFACES_SRV_SETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_75e3ae0e6db60a275b08e450c420969aa1ee05d0f34864e8fb6535d2d13613b9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_valid(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_wire_size(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_write_fields(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_encode(roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response* value) {
  roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_interfaces_srv_SetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
} roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTANGLES_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTANGLES_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTANGLES_CODEC_IDENTITY "UWVMROS1_2351398be27155ac63b8a3d15ffd0d3088dd7cf284182a9b494791b7780aa6da"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
} roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTCOORDS_CODEC_IDENTITY "UWVMROS1_b8513545b0975757c6b7f756a5cedfe8aa0c5d28d0e573adfde001b452e923de"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_1d097932f2e6b9b4e73ff838b5223b585b230e4edc5c229b310eb00404f6e784"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTSETANGLES_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTSETANGLES_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTSETANGLES_CODEC_IDENTITY "UWVMROS1_23d208d7fa8f7b7488be7c7db487698db4b227ef65e99c55d996901d27c3e591"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/msg/MycobotSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTSETCOORDS_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTSETCOORDS_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_MSG_MYCOBOTSETCOORDS_CODEC_IDENTITY "UWVMROS1_60f40a2f6a577e5785fd69e0cbe9128b301aeaec6b70fd678c71e7f9a5426cea"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_msg_MycobotSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETANGLES_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETANGLES_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_d2d8197675b9dadd50ae1a22b165a84e2e76b293b28cc70bec211225c60626c4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request* value) {
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETANGLES_RESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETANGLES_RESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_13056bbe3bdef4feb8488783e1c5cf725d1fb94006f36f2221f61081084227f8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETCOORDS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETCOORDS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_bda408ece90e671ce58ae428f79f01c1429c3b3aa3c1ef2efe9dc045faa9e3c2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request* value) {
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETCOORDS_RESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETCOORDS_RESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_01e8a3ef4c048100af32d92cd027769e32eacd82dba36dc3bde30c70cb44afdf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetGripperValue_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETGRIPPERVALUE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETGRIPPERVALUE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETGRIPPERVALUE_REQUEST_CODEC_IDENTITY "UWVMROS1_17cb5e3826409b661c2ad4bad8fde2b4c826dde95d8600e1a17bcf4908eca125"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request* value) {
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GetGripperValue_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response {
  /** Declared field gripper_angle; canonical capacity is part of CODEC_IDENTITY. */
  int8_t gripper_angle;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETGRIPPERVALUE_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETGRIPPERVALUE_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GETGRIPPERVALUE_RESPONSE_CODEC_IDENTITY "UWVMROS1_6b210e18d0696735196fb2e166b7168165dba6f03b97759a437fc8dc08ab2676"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->gripper_angle,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->gripper_angle,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GetGripperValue_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GripperStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_edc3352d37effe3923c55a024d6acac85a3cb49350974ea35934efa41555ccac"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/GripperStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_65166c2943fd319555cc1a2dcdaf4475b1e7779960907fe5ab6d1b5dc5536fc2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_GripperStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field joint_5; canonical capacity is part of CODEC_IDENTITY. */
  float joint_5;
  /** Declared field joint_6; canonical capacity is part of CODEC_IDENTITY. */
  float joint_6;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETANGLES_REQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETANGLES_REQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_ec2659577304938ca4f78c1b4add50190766d8d1b551397668ef7ad03e2ccb09"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETANGLES_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETANGLES_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_28dc59c06324913c1ccbe286ca05b9f47585653a8f086fc5d99a01f6e9ee08b4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field rz; canonical capacity is part of CODEC_IDENTITY. */
  float rz;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETCOORDS_REQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETCOORDS_REQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_e0c550200472f77aa582a46a5149c05c95648615a306f254599f820992890c81"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mycobot_pro450_interfaces/srv/SetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response;
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETCOORDS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETCOORDS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYCOBOT_PRO450_INTERFACES_SRV_SETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_9a5ee934233519095e1b28568490f8d958c319e2544976fae0b7de53a54a3f51"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_valid(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_wire_size(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_write_fields(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_encode(roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response* value) {
  roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mycobot_pro450_interfaces_srv_SetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
} roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALANGLES_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALANGLES_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALANGLES_CODEC_IDENTITY "UWVMROS1_0a2dc324515448310b0f026fb26c6c5c6da452e5e7067096e315abe3ab86a35d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_valid(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_encode(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
} roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALCOORDS_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALCOORDS_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALCOORDS_CODEC_IDENTITY "UWVMROS1_839491548a07528380403fa31c89889ce82f28b2f82e6fd9474835659714b4f7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_valid(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_encode(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_f6e4ad6193272d6401bbbfcc20c0f38b6a147a81d9de1ad956d7c154f29daeb0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_valid(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_encode(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
  /** Declared field pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin1;
  /** Declared field pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin2;
} roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_008dfd13cad42029330909fb7dc68392ba7f6678ad1baa14a41aad1e759f523c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_valid(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_encode(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALSETANGLES_MAX_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALSETANGLES_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALSETANGLES_CODEC_IDENTITY "UWVMROS1_b9fdd6ac956fdeb4934ff41982e616f7c0c45b11169b6d60ba74fbcc61458bb5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_valid(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_encode(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>17u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/msg/MypalSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALSETCOORDS_MAX_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALSETCOORDS_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_MSG_MYPALSETCOORDS_CODEC_IDENTITY "UWVMROS1_0c1b65f543cdd239a5a56a7839ac607c5310cd4145ef6a6da5639a9af8ba4c79"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_valid(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_encode(roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>17u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_msg_MypalSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/GetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETANGLES_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETANGLES_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_c8124a1063d47b24dae2e279f51d4e48ebf9936a5e305b81b53387a4be344f78"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request* value) {
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/GetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETANGLES_RESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETANGLES_RESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_33812a320c14891a5bc76a86ccbb0a5507cfbcc3706e01bc304263edabc6f75d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/GetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETCOORDS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETCOORDS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_b3db3a0e7d2aed097027798a897caf833f9cc1b8bb027f98c0db016968fa4440"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request* value) {
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/GetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETCOORDS_RESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETCOORDS_RESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_9d237cfa03c4b3c17d99fe0a9c08d1cafa3e8d0e404191e2169eea8683c54c84"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/GripperStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GRIPPERSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_b6d4c5991599ec4ac56e5f65dacd5a1ab89f073e9f07dc2b03692f4289fdcc9d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/GripperStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_GRIPPERSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_bd0c427dc564ff7201019c6d33c7d0c743a65d263c67eb420b5e653d48e4daf4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_GripperStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/PumpStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
  /** Declared field pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin1;
  /** Declared field pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin2;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_PUMPSTATUS_REQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_PUMPSTATUS_REQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_PUMPSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_6b14af554cb5c65c9411b2c6a76d49cb7cbe06dedcd26d47804d00d12ccf2e2f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request const* value) {
  if(!value)return 0;
  if(value->status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/PumpStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_PUMPSTATUS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_PUMPSTATUS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_PUMPSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_187a703ce648bc82a276961d82ed8abd5365d8db7af3bb211e590620e1bb2b45"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_PumpStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/SetAngles_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETANGLES_REQUEST_MAX_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETANGLES_REQUEST_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETANGLES_REQUEST_CODEC_IDENTITY "UWVMROS1_3997bb19c183b59fbca1675695ea5ee548a247a34f9682dae3fb68fdf1a155ec"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>17u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/SetAngles_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETANGLES_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETANGLES_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETANGLES_RESPONSE_CODEC_IDENTITY "UWVMROS1_a7952f549b35aa140adac4f18bbe8538c0f6230aa884459091cd74720ab757c9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetAngles_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/SetCoords_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETCOORDS_REQUEST_MAX_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETCOORDS_REQUEST_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETCOORDS_REQUEST_CODEC_IDENTITY "UWVMROS1_c72f4f1cb71f1ad1076bcc89499abbcfc5eab173fadd19c9a266baf1c32d1541"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>17u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 mypalletizer_interfaces/srv/SetCoords_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response {
  /** Declared field flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flag;
} roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response;
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETCOORDS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETCOORDS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_MYPALLETIZER_INTERFACES_SRV_SETCOORDS_RESPONSE_CODEC_IDENTITY "UWVMROS1_0cedaf9087ba0ae3726b9dd0d304665a226fc6df2ee068f6ac2b1c12ce88cb5b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_valid(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response const* value) {
  if(!value)return 0;
  if(value->flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_wire_size(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_write_fields(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_encode(roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_decode(void const* input,size_t length,roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response* value) {
  roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_mypalletizer_interfaces_srv_SetCoords_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
