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
/** Complete ROS1 mycobot_communication/MycobotAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotAngles {
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
} roswasmc_bounded_1_mycobot_communication_MycobotAngles;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTANGLES_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTANGLES_CODEC_IDENTITY "UWVMROS1_4cbda93a13d78e9c592327caa737dfecf33adf6db898015afd56ecf7732f9aa6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotAngles_valid(roswasmc_bounded_1_mycobot_communication_MycobotAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotAngles_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotAngles_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_communication_MycobotAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotAngles_encode(roswasmc_bounded_1_mycobot_communication_MycobotAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotAngles* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotCoords {
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
} roswasmc_bounded_1_mycobot_communication_MycobotCoords;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTCOORDS_CODEC_IDENTITY "UWVMROS1_1da0d6342a1f4079244a65ad413d4559047d33b049ce9da04f8ab0bd226410a2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotCoords_valid(roswasmc_bounded_1_mycobot_communication_MycobotCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotCoords_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotCoords_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_communication_MycobotCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotCoords_encode(roswasmc_bounded_1_mycobot_communication_MycobotCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotCoords* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_65edcc278c4e21355c62408ff310598b3dbdf03ed426e3da0f2ee0bc6910e649"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_valid(roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_encode(roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_0c0cb3c0fe97b9f9c65769a09f505ff6c27a60723598d656cd5f16255f36bfbd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_valid(roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_encode(roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotSetAngles {
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
} roswasmc_bounded_1_mycobot_communication_MycobotSetAngles;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETANGLES_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETANGLES_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETANGLES_CODEC_IDENTITY "UWVMROS1_0844a6cf5033b27ad5062741c939a7cfff63edeba34f7e56c8178e1fab85b334"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_valid(roswasmc_bounded_1_mycobot_communication_MycobotSetAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotSetAngles const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_encode(roswasmc_bounded_1_mycobot_communication_MycobotSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotSetAngles* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotSetCoords {
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
} roswasmc_bounded_1_mycobot_communication_MycobotSetCoords;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETCOORDS_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETCOORDS_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETCOORDS_CODEC_IDENTITY "UWVMROS1_39376ad94c0c9b213703a373b6a3b12fd0dad0ec20ff312143d30099842747a9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_valid(roswasmc_bounded_1_mycobot_communication_MycobotSetCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotSetCoords const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotSetCoords const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_encode(roswasmc_bounded_1_mycobot_communication_MycobotSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotSetCoords* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mycobot_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_8915949f81b788e5797f3bc4935e33e9e9eae82c5db3a1211129e7ef03cd2c54"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_valid(roswasmc_bounded_1_mycobot_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_mycobot_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_mycobot_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_encode(roswasmc_bounded_1_mycobot_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_mycobot_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_GetAnglesResponse {
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
} roswasmc_bounded_1_mycobot_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_482e6b845d6913ab7ea12580a395caf0a9815b56f985eb3a0553263fb7fd4ff3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_valid(roswasmc_bounded_1_mycobot_communication_GetAnglesResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_mycobot_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_mycobot_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_encode(roswasmc_bounded_1_mycobot_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_mycobot_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mycobot_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_54acc83a24f2f62d8ba9c14b1cea1703cb6097360720550cc438671282d45876"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_valid(roswasmc_bounded_1_mycobot_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_mycobot_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_mycobot_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_encode(roswasmc_bounded_1_mycobot_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_mycobot_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_GetCoordsResponse {
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
} roswasmc_bounded_1_mycobot_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_275b9bec563bb8578112b3680400523000a08a8e194044164e5334336b73d107"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_valid(roswasmc_bounded_1_mycobot_communication_GetCoordsResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_mycobot_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_mycobot_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_encode(roswasmc_bounded_1_mycobot_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_mycobot_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_d2c94d1906d311981c72642d82bbea634cfed4eac8c0aef41219bf4f382d4585"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_valid(roswasmc_bounded_1_mycobot_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_mycobot_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_mycobot_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_encode(roswasmc_bounded_1_mycobot_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_mycobot_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_3a75c0d75882836971ed4c51e50c7b88778fcfeadf9b30df211d5055eb767664"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_valid(roswasmc_bounded_1_mycobot_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_mycobot_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_mycobot_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_encode(roswasmc_bounded_1_mycobot_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_mycobot_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/PumpStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_PumpStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mycobot_communication_PumpStatusRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_PUMPSTATUSREQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_PUMPSTATUSREQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_PUMPSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_561f4bb1f74832111672bbfed1ef1266a91914fb1034437199f730f55324c65c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_valid(roswasmc_bounded_1_mycobot_communication_PumpStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_wire_size(roswasmc_bounded_1_mycobot_communication_PumpStatusRequest const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_write_fields(roswasmc_bounded_1_mycobot_communication_PumpStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_PumpStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_encode(roswasmc_bounded_1_mycobot_communication_PumpStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_PumpStatusRequest* value) {
  roswasmc_bounded_1_mycobot_communication_PumpStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_PumpStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/PumpStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_PumpStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_PumpStatusResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_PUMPSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_PUMPSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_PUMPSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_b5a1672fe12d2e2131034bb6da8751130b64ef6d48ea48ac0a046c506da452dd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_valid(roswasmc_bounded_1_mycobot_communication_PumpStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_wire_size(roswasmc_bounded_1_mycobot_communication_PumpStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_write_fields(roswasmc_bounded_1_mycobot_communication_PumpStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_PumpStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_encode(roswasmc_bounded_1_mycobot_communication_PumpStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_PumpStatusResponse* value) {
  roswasmc_bounded_1_mycobot_communication_PumpStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_PumpStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetAnglesRequest {
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
} roswasmc_bounded_1_mycobot_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_10bcf04e47b743a985ffefc61dff4522f3718d2ac7170960c8ae6857aaa11cbd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_valid(roswasmc_bounded_1_mycobot_communication_SetAnglesRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_mycobot_communication_SetAnglesRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_mycobot_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_encode(roswasmc_bounded_1_mycobot_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_mycobot_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_d407051eb285975d89ad71f39aede021fb564aecca0e0744126c9a599fe396b2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_valid(roswasmc_bounded_1_mycobot_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_mycobot_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_mycobot_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_encode(roswasmc_bounded_1_mycobot_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_mycobot_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetCoordsRequest {
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
} roswasmc_bounded_1_mycobot_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_7a59a4e31eca60caea9f82142225c890a136c60a9e4c821e9463e89139945f7a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_valid(roswasmc_bounded_1_mycobot_communication_SetCoordsRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_mycobot_communication_SetCoordsRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_mycobot_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_encode(roswasmc_bounded_1_mycobot_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_mycobot_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_1a936a95cb1bfd0a9158342618c40c587af58388abf3b679eb4bad2518aec6af"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_valid(roswasmc_bounded_1_mycobot_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_mycobot_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_mycobot_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_encode(roswasmc_bounded_1_mycobot_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_mycobot_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/MypalAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_MypalAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
} roswasmc_bounded_1_mypalletizer_communication_MypalAngles;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALANGLES_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALANGLES_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALANGLES_CODEC_IDENTITY "UWVMROS1_1d45a33e05e948c1b58988c84afe359138c896119f82da0d737377f0272892d9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalAngles_valid(roswasmc_bounded_1_mypalletizer_communication_MypalAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_MypalAngles_wire_size(roswasmc_bounded_1_mypalletizer_communication_MypalAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_MypalAngles_write_fields(roswasmc_bounded_1_mypalletizer_communication_MypalAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_MypalAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mypalletizer_communication_MypalAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalAngles_encode(roswasmc_bounded_1_mypalletizer_communication_MypalAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_MypalAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_MypalAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_MypalAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_MypalAngles* value) {
  roswasmc_bounded_1_mypalletizer_communication_MypalAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_MypalAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/MypalCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_MypalCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
} roswasmc_bounded_1_mypalletizer_communication_MypalCoords;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALCOORDS_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALCOORDS_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALCOORDS_CODEC_IDENTITY "UWVMROS1_c542d1a1a71224901044bc38ca2934021d811417c7c6937ca194995f67e59b80"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalCoords_valid(roswasmc_bounded_1_mypalletizer_communication_MypalCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_MypalCoords_wire_size(roswasmc_bounded_1_mypalletizer_communication_MypalCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_MypalCoords_write_fields(roswasmc_bounded_1_mypalletizer_communication_MypalCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_MypalCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mypalletizer_communication_MypalCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalCoords_encode(roswasmc_bounded_1_mypalletizer_communication_MypalCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_MypalCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_MypalCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_MypalCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_MypalCoords* value) {
  roswasmc_bounded_1_mypalletizer_communication_MypalCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_MypalCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/MypalGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_e37f608d4499ab2ed49618fc244831d274fadb23bd408d42619e3f8409ad2ee6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_valid(roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_wire_size(roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_write_fields(roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_encode(roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus* value) {
  roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_MypalGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/MypalPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_cdf79e610b5d49b7f73857e02c64688a71517f2fe8e3a96739b16a53ad4d0ebc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_valid(roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_wire_size(roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_write_fields(roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_encode(roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus* value) {
  roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_MypalPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/MypalSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles {
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
} roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALSETANGLES_MAX_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALSETANGLES_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALSETANGLES_CODEC_IDENTITY "UWVMROS1_8d11ba2c705bd577a329a01d039b831162742efe790c93b6c2c2631699d04f98"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_valid(roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_wire_size(roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_write_fields(roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_encode(roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles* value) {
  roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>17u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_MypalSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/MypalSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords {
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
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  int8_t model;
} roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALSETCOORDS_MAX_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALSETCOORDS_MIN_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_MYPALSETCOORDS_CODEC_IDENTITY "UWVMROS1_51a2d485c8d2252f184eb27724e88c1ae06002d54454bd8ebf31962a79f7c448"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_valid(roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_wire_size(roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_write_fields(roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->model,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_encode(roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords* value) {
  roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<18u||length>18u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_MypalSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_d0412d8ccc8f724f2a0660d20e24a39f6445f0cb433e775058b6b092fde65de8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_valid(roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_encode(roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
} roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_cb5a18aec2420c030d8193eb40d25691a2491b98606a7722156984e82f77f745"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_valid(roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_encode(roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_47d45419f14ce576d9e8c2bb288c7b67bb161115e7b99a2c4752d09cbb8087b8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_valid(roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_encode(roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
} roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_7fdad367484b5f89c812c2513e99373844dcd160a115e326fb1c597f221b53fb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_valid(roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_encode(roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_48d661f8fd6adfcf1703123464fcfb9351f8f831c0779ffad103a63bcf7ea709"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_valid(roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_encode(roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_00ab8ceb190b588a9420c2e044da64857cfdb76e709e40e57835ab5732074e2b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_valid(roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_encode(roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/PumpStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_PUMPSTATUSREQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_PUMPSTATUSREQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_PUMPSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_6dd70636864c4450a066c1d75e67643331fac4c3e1625bce1b94c85ce7a35636"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_valid(roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_wire_size(roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_write_fields(roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_encode(roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest* value) {
  roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_PumpStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/PumpStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_PUMPSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_PUMPSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_PUMPSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_c2503f89c84c49db8dab2ac9f847ba9d09434d366fd8916a7d5f30ea161bb4c5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_valid(roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_wire_size(roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_write_fields(roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_encode(roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse* value) {
  roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_PumpStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest {
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
} roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_32072510c6deddb3f372b5f300068ea457916a4e0397b30951e421a1da641d5d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_valid(roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_encode(roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>17u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_1472b47adc3171a9f1938e52bc937af6dd6bb720d33e163b70ae066a89a3211c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_valid(roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_encode(roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest {
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
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  int8_t model;
} roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_2d608cac1a7129ca5cd68ff52170ed40258cb6d199f4842a268a1d7570f9bda1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_valid(roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->model,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_encode(roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<18u||length>18u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mypalletizer_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYPALLETIZER_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_86900f8772e9609e1d2b882b6ab613ce9609be6c589e5b9574a57b8749231308"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_valid(roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_encode(roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mypalletizer_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
