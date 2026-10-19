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
/** Complete ROS1 mybuddy_communication/MybuddyAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_MybuddyAngles {
  /** Declared field joint_0; canonical capacity is part of CODEC_IDENTITY. */
  float joint_0;
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
  /** Declared field joint_8; canonical capacity is part of CODEC_IDENTITY. */
  float joint_8;
  /** Declared field joint_9; canonical capacity is part of CODEC_IDENTITY. */
  float joint_9;
  /** Declared field joint_10; canonical capacity is part of CODEC_IDENTITY. */
  float joint_10;
  /** Declared field joint_11; canonical capacity is part of CODEC_IDENTITY. */
  float joint_11;
  /** Declared field joint_12; canonical capacity is part of CODEC_IDENTITY. */
  float joint_12;
} roswasmc_bounded_1_mybuddy_communication_MybuddyAngles;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYANGLES_MAX_WIRE_BYTES 52u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYANGLES_MIN_WIRE_BYTES 52u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYANGLES_CODEC_IDENTITY "UWVMROS1_f3c510869207e6d72e868ae35ef95d49a1eea5164c7ef1056cb3d335029563e9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_valid(roswasmc_bounded_1_mybuddy_communication_MybuddyAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_wire_size(roswasmc_bounded_1_mybuddy_communication_MybuddyAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_write_fields(roswasmc_bounded_1_mybuddy_communication_MybuddyAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_MybuddyAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_0,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_8,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_9,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_10,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_11,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_12,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_encode(roswasmc_bounded_1_mybuddy_communication_MybuddyAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_MybuddyAngles* value) {
  roswasmc_bounded_1_mybuddy_communication_MybuddyAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<52u||length>52u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_MybuddyAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/MybuddyCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_MybuddyCoords {
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
} roswasmc_bounded_1_mybuddy_communication_MybuddyCoords;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYCOORDS_CODEC_IDENTITY "UWVMROS1_a1df376733f2673e25fed814a038e00f556b82c2b8bce3755644904e50c95384"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_valid(roswasmc_bounded_1_mybuddy_communication_MybuddyCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_wire_size(roswasmc_bounded_1_mybuddy_communication_MybuddyCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_write_fields(roswasmc_bounded_1_mybuddy_communication_MybuddyCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_MybuddyCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_encode(roswasmc_bounded_1_mybuddy_communication_MybuddyCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_MybuddyCoords* value) {
  roswasmc_bounded_1_mybuddy_communication_MybuddyCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_MybuddyCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/MybuddyGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_86fdaf7d3397b26155c891e15d2ebdd903d5899a0895128e720276eb35a63854"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_valid(roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_wire_size(roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_write_fields(roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_encode(roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus* value) {
  roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_MybuddyGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/MybuddyPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_33b6419700f9baf41b584bb464d0234426a04e1bd86d198fe3d66c06845cd80a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_valid(roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_wire_size(roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_write_fields(roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_encode(roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus* value) {
  roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_MybuddyPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/MybuddySetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles {
  /** Declared field joint_0; canonical capacity is part of CODEC_IDENTITY. */
  float joint_0;
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
  /** Declared field joint_8; canonical capacity is part of CODEC_IDENTITY. */
  float joint_8;
  /** Declared field joint_9; canonical capacity is part of CODEC_IDENTITY. */
  float joint_9;
  /** Declared field joint_10; canonical capacity is part of CODEC_IDENTITY. */
  float joint_10;
  /** Declared field joint_11; canonical capacity is part of CODEC_IDENTITY. */
  float joint_11;
  /** Declared field joint_12; canonical capacity is part of CODEC_IDENTITY. */
  float joint_12;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYSETANGLES_MAX_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYSETANGLES_MIN_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYSETANGLES_CODEC_IDENTITY "UWVMROS1_8f41fddfb715bfaa95c90974cf257181a9ae3e2f36969f6fdfdf3ab03b7d4486"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_valid(roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_wire_size(roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
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
static inline void roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_write_fields(roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_0,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_8,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_9,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_10,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_11,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_12,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_encode(roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles* value) {
  roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<53u||length>53u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_MybuddySetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/MybuddySetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords {
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
} roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYSETCOORDS_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYSETCOORDS_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_MYBUDDYSETCOORDS_CODEC_IDENTITY "UWVMROS1_bf9f2ee073928aafabbd532644eb57eec70d4b864a3097e2ab1a37840abd3669"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_valid(roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_wire_size(roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_write_fields(roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_encode(roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords* value) {
  roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_MybuddySetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_7ba90c12eb91dbcc296c0d61069d0dc12c3ac576c970cd2fe9a0fed29f058e38"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_valid(roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_encode(roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse {
  /** Declared field joint_0; canonical capacity is part of CODEC_IDENTITY. */
  float joint_0;
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
  /** Declared field joint_8; canonical capacity is part of CODEC_IDENTITY. */
  float joint_8;
  /** Declared field joint_9; canonical capacity is part of CODEC_IDENTITY. */
  float joint_9;
  /** Declared field joint_10; canonical capacity is part of CODEC_IDENTITY. */
  float joint_10;
  /** Declared field joint_11; canonical capacity is part of CODEC_IDENTITY. */
  float joint_11;
  /** Declared field joint_12; canonical capacity is part of CODEC_IDENTITY. */
  float joint_12;
} roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 52u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 52u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_98085b01a21ae26a74a286d790aab310530fff2a88d7a3c35e071df09abd709d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_valid(roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_0,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_8,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_9,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_10,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_11,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_12,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_encode(roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<52u||length>52u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_e5b4bd6f4d17f79bf3477c2577e73355b871523291aace267fea201f5a2b43ef"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_valid(roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_encode(roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse {
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
} roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_604a9a58b11d6e51faee74226c4820db7668b2054b2e48949cf9deb5958f5873"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_valid(roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_encode(roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_7816f3fa4c8b7068eeb16e74940d2e26401df47b7bc6881d123f94b554523173"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_valid(roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_encode(roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_b329fd687ef106ed366d80c5651fc536ad266d2b16dee8f985f1bad32738330b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_valid(roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_encode(roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/PumpStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_PUMPSTATUSREQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_PUMPSTATUSREQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_PUMPSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_4261e7e11f0c6065de868444a0be45d363465b0913246b06b4a2c413714d77e9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_valid(roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_wire_size(roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_write_fields(roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_encode(roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest* value) {
  roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_PumpStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/PumpStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_PUMPSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_PUMPSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_PUMPSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_975f537d88c880f8daa3459f6762dc6ec7d7eee1f580c0e79f3cb581b77cabd4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_valid(roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_wire_size(roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_write_fields(roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_encode(roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse* value) {
  roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_PumpStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest {
  /** Declared field joint_0; canonical capacity is part of CODEC_IDENTITY. */
  float joint_0;
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
  /** Declared field joint_8; canonical capacity is part of CODEC_IDENTITY. */
  float joint_8;
  /** Declared field joint_9; canonical capacity is part of CODEC_IDENTITY. */
  float joint_9;
  /** Declared field joint_10; canonical capacity is part of CODEC_IDENTITY. */
  float joint_10;
  /** Declared field joint_11; canonical capacity is part of CODEC_IDENTITY. */
  float joint_11;
  /** Declared field joint_12; canonical capacity is part of CODEC_IDENTITY. */
  float joint_12;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_47d19578189892ca10e073d8d250839f1e2b39ebbb3525ed9b03a15af18eb240"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_valid(roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
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
static inline void roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_0,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_8,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_9,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_10,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_11,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_12,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_0,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_8,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_9,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_10,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_11,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_12,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_encode(roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<53u||length>53u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_7371fa369708af552fd68522d7effdd00bf82bf7e2fe21469ceb210f51582f81"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_valid(roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_encode(roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest {
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
} roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_81f7315bebd612a9406a2805b393415b8a1956e49f3d1b8efb21f9e703b0bfe0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_valid(roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_encode(roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mybuddy_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYBUDDY_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_e5c41ea9420e43a5406ddee23cee961347bfad4f83baf9ebac93d16cdb5d0b21"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_valid(roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_encode(roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mybuddy_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/MycobotAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_MycobotAngles {
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
} roswasmc_bounded_1_mecharm_communication_MycobotAngles;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTANGLES_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTANGLES_CODEC_IDENTITY "UWVMROS1_4afa3dc66ce6ffe02170c76da688514a04aa7b47008aeb8f79c44f9ff867d189"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotAngles_valid(roswasmc_bounded_1_mecharm_communication_MycobotAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_MycobotAngles_wire_size(roswasmc_bounded_1_mecharm_communication_MycobotAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_MycobotAngles_write_fields(roswasmc_bounded_1_mecharm_communication_MycobotAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_MycobotAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mecharm_communication_MycobotAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotAngles_encode(roswasmc_bounded_1_mecharm_communication_MycobotAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_MycobotAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_MycobotAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_MycobotAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_MycobotAngles* value) {
  roswasmc_bounded_1_mecharm_communication_MycobotAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_MycobotAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/MycobotCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_MycobotCoords {
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
} roswasmc_bounded_1_mecharm_communication_MycobotCoords;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTCOORDS_CODEC_IDENTITY "UWVMROS1_a6a82e22cd0a2f24657c0a253ed00bf31c0a266e6ebaab071cfd369780818d35"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotCoords_valid(roswasmc_bounded_1_mecharm_communication_MycobotCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_MycobotCoords_wire_size(roswasmc_bounded_1_mecharm_communication_MycobotCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_MycobotCoords_write_fields(roswasmc_bounded_1_mecharm_communication_MycobotCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_MycobotCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mecharm_communication_MycobotCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotCoords_encode(roswasmc_bounded_1_mecharm_communication_MycobotCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_MycobotCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_MycobotCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_MycobotCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_MycobotCoords* value) {
  roswasmc_bounded_1_mecharm_communication_MycobotCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_MycobotCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/MycobotGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_07e04dad52b423c371dc5432b68a2f4272cf55328d6f0a10f091dc389d57a4bf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_valid(roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_wire_size(roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_write_fields(roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_encode(roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus* value) {
  roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_MycobotGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/MycobotPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_f559fbf7d5d2c872b3660132eb8c17c79ca3c2d27e975f8fb8fde1d86072a6fa"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_valid(roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_wire_size(roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_write_fields(roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_encode(roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus* value) {
  roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_MycobotPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/MycobotSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_MycobotSetAngles {
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
} roswasmc_bounded_1_mecharm_communication_MycobotSetAngles;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTSETANGLES_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTSETANGLES_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTSETANGLES_CODEC_IDENTITY "UWVMROS1_bce09b709e806417595c05e0e795d689b8c53008f9916633d934f935f9389de9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_valid(roswasmc_bounded_1_mecharm_communication_MycobotSetAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_wire_size(roswasmc_bounded_1_mecharm_communication_MycobotSetAngles const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_write_fields(roswasmc_bounded_1_mecharm_communication_MycobotSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_MycobotSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_encode(roswasmc_bounded_1_mecharm_communication_MycobotSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_MycobotSetAngles* value) {
  roswasmc_bounded_1_mecharm_communication_MycobotSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_MycobotSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/MycobotSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_MycobotSetCoords {
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
} roswasmc_bounded_1_mecharm_communication_MycobotSetCoords;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTSETCOORDS_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTSETCOORDS_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_MYCOBOTSETCOORDS_CODEC_IDENTITY "UWVMROS1_68ffa6950e0934b845b2b45140612997c8276a3ac7ed4303e43cd70417491913"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_valid(roswasmc_bounded_1_mecharm_communication_MycobotSetCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_wire_size(roswasmc_bounded_1_mecharm_communication_MycobotSetCoords const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_write_fields(roswasmc_bounded_1_mecharm_communication_MycobotSetCoords const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_MycobotSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_encode(roswasmc_bounded_1_mecharm_communication_MycobotSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_MycobotSetCoords* value) {
  roswasmc_bounded_1_mecharm_communication_MycobotSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_MycobotSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mecharm_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_6a8f60e71495c9944e1e97c308d5c992674bd46060b6f6abf2df3f9748c13123"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_valid(roswasmc_bounded_1_mecharm_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_mecharm_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_mecharm_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_encode(roswasmc_bounded_1_mecharm_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_mecharm_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_GetAnglesResponse {
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
} roswasmc_bounded_1_mecharm_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_e792a206903dc49fab725e3b3507af3cf4712f10740e9dcceeb90dedfe6ea6e8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_valid(roswasmc_bounded_1_mecharm_communication_GetAnglesResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_mecharm_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_mecharm_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_encode(roswasmc_bounded_1_mecharm_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_mecharm_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mecharm_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_55d4d1b8965ea12f9b15d4bd5a22c43793d04af452fefb84bd98bc3d7d8da8bf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_valid(roswasmc_bounded_1_mecharm_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_mecharm_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_mecharm_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_encode(roswasmc_bounded_1_mecharm_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_mecharm_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_GetCoordsResponse {
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
} roswasmc_bounded_1_mecharm_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_2230e517685ea625507334067ccceb8424ecf782fcd4163c551d09048e900ca8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_valid(roswasmc_bounded_1_mecharm_communication_GetCoordsResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_mecharm_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_mecharm_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_encode(roswasmc_bounded_1_mecharm_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_mecharm_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mecharm_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_98284fbac3bdb61857b9175b1f82adc7265d0ed418088123d79bf75cb04c9a2c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_valid(roswasmc_bounded_1_mecharm_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_mecharm_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_mecharm_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_encode(roswasmc_bounded_1_mecharm_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_mecharm_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mecharm_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_29cfb0267fbe68d5dafcd3c9d16a8d4dab6b21477921a48ba35135bc0c22bb32"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_valid(roswasmc_bounded_1_mecharm_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_mecharm_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_mecharm_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_encode(roswasmc_bounded_1_mecharm_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_mecharm_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/PumpStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_PumpStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mecharm_communication_PumpStatusRequest;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_PUMPSTATUSREQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_PUMPSTATUSREQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_PUMPSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_aa138db91e46b41659dfc5901225aaaae1c1ae0461c35d3f7a90d0cf86c9920d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_valid(roswasmc_bounded_1_mecharm_communication_PumpStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_wire_size(roswasmc_bounded_1_mecharm_communication_PumpStatusRequest const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_write_fields(roswasmc_bounded_1_mecharm_communication_PumpStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_PumpStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_encode(roswasmc_bounded_1_mecharm_communication_PumpStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_PumpStatusRequest* value) {
  roswasmc_bounded_1_mecharm_communication_PumpStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_PumpStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/PumpStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_PumpStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mecharm_communication_PumpStatusResponse;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_PUMPSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_PUMPSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_PUMPSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_876caa88c833bfa180b8747110683c789aa238c0cda162a8e262c66fddefdd8f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_valid(roswasmc_bounded_1_mecharm_communication_PumpStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_wire_size(roswasmc_bounded_1_mecharm_communication_PumpStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_write_fields(roswasmc_bounded_1_mecharm_communication_PumpStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_PumpStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_encode(roswasmc_bounded_1_mecharm_communication_PumpStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_PumpStatusResponse* value) {
  roswasmc_bounded_1_mecharm_communication_PumpStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_PumpStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_SetAnglesRequest {
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
} roswasmc_bounded_1_mecharm_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_7ad54edf4730457f2b2124c925b87a99bdbeddd8bfcc1e0d806f7b9dad31507b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_valid(roswasmc_bounded_1_mecharm_communication_SetAnglesRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_mecharm_communication_SetAnglesRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_mecharm_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_encode(roswasmc_bounded_1_mecharm_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_mecharm_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mecharm_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_940a294ef91d04afeb65b88bbdd91df3a7db0a07435ced3993db3a9d0f574c6e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_valid(roswasmc_bounded_1_mecharm_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_mecharm_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_mecharm_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_encode(roswasmc_bounded_1_mecharm_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_mecharm_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_SetCoordsRequest {
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
} roswasmc_bounded_1_mecharm_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_68deaf663a1fe2d3d7434935f10161eb444e7ea2f79e4b746fee9bd0383fc929"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_valid(roswasmc_bounded_1_mecharm_communication_SetCoordsRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_mecharm_communication_SetCoordsRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_mecharm_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_encode(roswasmc_bounded_1_mecharm_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_mecharm_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mecharm_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mecharm_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mecharm_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MECHARM_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_c66a6e14c4c7eba1b52d7ef57802cb6122a120f692bcc1552c51078301b1b409"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_valid(roswasmc_bounded_1_mecharm_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_mecharm_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_mecharm_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mecharm_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_encode(roswasmc_bounded_1_mecharm_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mecharm_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_mecharm_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mecharm_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/MyArmAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_MyArmAngles {
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
} roswasmc_bounded_1_myarm_communication_MyArmAngles;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMANGLES_MAX_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMANGLES_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMANGLES_CODEC_IDENTITY "UWVMROS1_eff11a03e974b4af030c2dc92d0111cf95e0fb3ce004235bada3fe358b63fd4c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmAngles_valid(roswasmc_bounded_1_myarm_communication_MyArmAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_MyArmAngles_wire_size(roswasmc_bounded_1_myarm_communication_MyArmAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_MyArmAngles_write_fields(roswasmc_bounded_1_myarm_communication_MyArmAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_MyArmAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_myarm_communication_MyArmAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmAngles_encode(roswasmc_bounded_1_myarm_communication_MyArmAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_MyArmAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_MyArmAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_MyArmAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmAngles_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_MyArmAngles* value) {
  roswasmc_bounded_1_myarm_communication_MyArmAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>28u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_MyArmAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/MyArmCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_MyArmCoords {
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
} roswasmc_bounded_1_myarm_communication_MyArmCoords;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMCOORDS_CODEC_IDENTITY "UWVMROS1_f0df1605988924f0263805fb2b27ab7ef9e2296691ed15e03a337606362c983c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmCoords_valid(roswasmc_bounded_1_myarm_communication_MyArmCoords const* value) {
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
static inline size_t roswasmc_bounded_1_myarm_communication_MyArmCoords_wire_size(roswasmc_bounded_1_myarm_communication_MyArmCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_MyArmCoords_write_fields(roswasmc_bounded_1_myarm_communication_MyArmCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_MyArmCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_myarm_communication_MyArmCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmCoords_encode(roswasmc_bounded_1_myarm_communication_MyArmCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_MyArmCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_MyArmCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_MyArmCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmCoords_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_MyArmCoords* value) {
  roswasmc_bounded_1_myarm_communication_MyArmCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_MyArmCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/MyArmGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_MyArmGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_myarm_communication_MyArmGripperStatus;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_7875df5b50417da1cef60d79c872aff0b40a08f9bbfe80c103729d3c4c892c6f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_valid(roswasmc_bounded_1_myarm_communication_MyArmGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_wire_size(roswasmc_bounded_1_myarm_communication_MyArmGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_write_fields(roswasmc_bounded_1_myarm_communication_MyArmGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_MyArmGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_encode(roswasmc_bounded_1_myarm_communication_MyArmGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_MyArmGripperStatus* value) {
  roswasmc_bounded_1_myarm_communication_MyArmGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_MyArmGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/MyArmPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_MyArmPumpStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_myarm_communication_MyArmPumpStatus;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_58765b94f1ecb3bb131f6480ea6830631cde9c96eeef94fd9b22598ccc9ea928"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_valid(roswasmc_bounded_1_myarm_communication_MyArmPumpStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_wire_size(roswasmc_bounded_1_myarm_communication_MyArmPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_write_fields(roswasmc_bounded_1_myarm_communication_MyArmPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_MyArmPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_encode(roswasmc_bounded_1_myarm_communication_MyArmPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_MyArmPumpStatus* value) {
  roswasmc_bounded_1_myarm_communication_MyArmPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_MyArmPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/MyArmSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_MyArmSetAngles {
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_1_myarm_communication_MyArmSetAngles;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMSETANGLES_MAX_WIRE_BYTES 29u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMSETANGLES_MIN_WIRE_BYTES 29u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMSETANGLES_CODEC_IDENTITY "UWVMROS1_6acbfd19e3864b66e47b9da5c7c0e8890e7336e8d6c9a5657c4f5eaa5285fe42"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmSetAngles_valid(roswasmc_bounded_1_myarm_communication_MyArmSetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_MyArmSetAngles_wire_size(roswasmc_bounded_1_myarm_communication_MyArmSetAngles const* value) {size_t n=0;
  n+=4;
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
static inline void roswasmc_bounded_1_myarm_communication_MyArmSetAngles_write_fields(roswasmc_bounded_1_myarm_communication_MyArmSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_MyArmSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_MyArmSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmSetAngles_encode(roswasmc_bounded_1_myarm_communication_MyArmSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_MyArmSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_MyArmSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_MyArmSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmSetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_MyArmSetAngles* value) {
  roswasmc_bounded_1_myarm_communication_MyArmSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<29u||length>29u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_MyArmSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/MyArmSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_MyArmSetCoords {
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
} roswasmc_bounded_1_myarm_communication_MyArmSetCoords;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMSETCOORDS_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMSETCOORDS_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_MYARMSETCOORDS_CODEC_IDENTITY "UWVMROS1_ce10c29be4cfd832d5076fead55817a0ac7ccde002d5bbbd7a2befd95e9752fd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_MyArmSetCoords_valid(roswasmc_bounded_1_myarm_communication_MyArmSetCoords const* value) {
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
static inline size_t roswasmc_bounded_1_myarm_communication_MyArmSetCoords_wire_size(roswasmc_bounded_1_myarm_communication_MyArmSetCoords const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_myarm_communication_MyArmSetCoords_write_fields(roswasmc_bounded_1_myarm_communication_MyArmSetCoords const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_myarm_communication_MyArmSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_MyArmSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_MyArmSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmSetCoords_encode(roswasmc_bounded_1_myarm_communication_MyArmSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_MyArmSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_MyArmSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_MyArmSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_MyArmSetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_MyArmSetCoords* value) {
  roswasmc_bounded_1_myarm_communication_MyArmSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_MyArmSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_myarm_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_18ea682d1c475321b67435e9f207a14627113ad237a38c759cfe4e1c92fded37"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_GetAnglesRequest_valid(roswasmc_bounded_1_myarm_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_myarm_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_myarm_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_myarm_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetAnglesRequest_encode(roswasmc_bounded_1_myarm_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_myarm_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_GetAnglesResponse {
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
} roswasmc_bounded_1_myarm_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_0c68b293384d21d06e23d4aba10b4a4c2eff4e1a0eaa81903e026fe28da410c7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_GetAnglesResponse_valid(roswasmc_bounded_1_myarm_communication_GetAnglesResponse const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_myarm_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_myarm_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_myarm_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetAnglesResponse_encode(roswasmc_bounded_1_myarm_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_myarm_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>28u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_myarm_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_04d9d79ad983cf4631cf65a48cae60d982cd0f234f0f4ed1afa0cc630bd63d95"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_GetCoordsRequest_valid(roswasmc_bounded_1_myarm_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_myarm_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_myarm_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_myarm_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetCoordsRequest_encode(roswasmc_bounded_1_myarm_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_myarm_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_GetCoordsResponse {
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
} roswasmc_bounded_1_myarm_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_752564704c08fb2076df8eea4f6f5b89621041372472f0f99b6627fdfda831b7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_GetCoordsResponse_valid(roswasmc_bounded_1_myarm_communication_GetCoordsResponse const* value) {
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
static inline size_t roswasmc_bounded_1_myarm_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_myarm_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_myarm_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_myarm_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetCoordsResponse_encode(roswasmc_bounded_1_myarm_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_myarm_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_myarm_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_adec2e0fbf48862c2245aea4c31c59ea12abdec26b4727b14005b65006c58976"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_GripperStatusRequest_valid(roswasmc_bounded_1_myarm_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_myarm_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_myarm_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GripperStatusRequest_encode(roswasmc_bounded_1_myarm_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_myarm_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_myarm_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_d703a0ec6f562f1deb444313e36e6a728b4e521d1196d90a63725753d5fe9659"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_GripperStatusResponse_valid(roswasmc_bounded_1_myarm_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_myarm_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_myarm_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GripperStatusResponse_encode(roswasmc_bounded_1_myarm_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_myarm_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/PumpStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_PumpStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_myarm_communication_PumpStatusRequest;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_PUMPSTATUSREQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_PUMPSTATUSREQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_PUMPSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_eac2f5c006a0fa86df638c52e9cfb7d2c17818ec10e32ad5277af216c6c944f4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_PumpStatusRequest_valid(roswasmc_bounded_1_myarm_communication_PumpStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_PumpStatusRequest_wire_size(roswasmc_bounded_1_myarm_communication_PumpStatusRequest const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_PumpStatusRequest_write_fields(roswasmc_bounded_1_myarm_communication_PumpStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_PumpStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_PumpStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_PumpStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_PumpStatusRequest_encode(roswasmc_bounded_1_myarm_communication_PumpStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_PumpStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_PumpStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_PumpStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_PumpStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_PumpStatusRequest* value) {
  roswasmc_bounded_1_myarm_communication_PumpStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_PumpStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/PumpStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_PumpStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_myarm_communication_PumpStatusResponse;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_PUMPSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_PUMPSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_PUMPSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_28533ec27c01d85c9a4644959ea8e5c3f2480f5203a9545e96e4242158f08044"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_PumpStatusResponse_valid(roswasmc_bounded_1_myarm_communication_PumpStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_PumpStatusResponse_wire_size(roswasmc_bounded_1_myarm_communication_PumpStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_PumpStatusResponse_write_fields(roswasmc_bounded_1_myarm_communication_PumpStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_PumpStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_PumpStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_PumpStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_PumpStatusResponse_encode(roswasmc_bounded_1_myarm_communication_PumpStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_PumpStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_PumpStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_PumpStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_PumpStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_PumpStatusResponse* value) {
  roswasmc_bounded_1_myarm_communication_PumpStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_PumpStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_SetAnglesRequest {
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
  /** Declared field joint_7; canonical capacity is part of CODEC_IDENTITY. */
  float joint_7;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int8_t speed;
} roswasmc_bounded_1_myarm_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 29u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 29u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_c15c81907fbace209686a98578019d893d86cecf4782427ac2c3858da8cd3e0a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_SetAnglesRequest_valid(roswasmc_bounded_1_myarm_communication_SetAnglesRequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_myarm_communication_SetAnglesRequest const* value) {size_t n=0;
  n+=4;
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
static inline void roswasmc_bounded_1_myarm_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_myarm_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetAnglesRequest_encode(roswasmc_bounded_1_myarm_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_myarm_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<29u||length>29u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_myarm_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_dbdbcb001f0478a4bba5ae9d7320d8dbfa4e36a5c1c130215ff5ab864c73af0f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_SetAnglesResponse_valid(roswasmc_bounded_1_myarm_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_myarm_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_myarm_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetAnglesResponse_encode(roswasmc_bounded_1_myarm_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_myarm_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_SetCoordsRequest {
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
} roswasmc_bounded_1_myarm_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_86cfc6edb6b86f90a21818e824aea50b697e314687472e1e96e9ce5399ef66c6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_SetCoordsRequest_valid(roswasmc_bounded_1_myarm_communication_SetCoordsRequest const* value) {
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
static inline size_t roswasmc_bounded_1_myarm_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_myarm_communication_SetCoordsRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_myarm_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_myarm_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_myarm_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetCoordsRequest_encode(roswasmc_bounded_1_myarm_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_myarm_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 myarm_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_myarm_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_myarm_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYARM_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_52a54371663141d4384dae8969b729baf097a0aa6105f263e78593743e44272d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_myarm_communication_SetCoordsResponse_valid(roswasmc_bounded_1_myarm_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_myarm_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_myarm_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_myarm_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_myarm_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_myarm_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_myarm_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_myarm_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetCoordsResponse_encode(roswasmc_bounded_1_myarm_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_myarm_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_myarm_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_myarm_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_myarm_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_myarm_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_myarm_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_myarm_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/MycobotAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_MycobotAngles {
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
} roswasmc_bounded_1_mycobot_320_communication_MycobotAngles;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTANGLES_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTANGLES_CODEC_IDENTITY "UWVMROS1_67ec61da0c2bf7ee461c21abbfcab14d74b84cb6c9a12430e2a306abdc65690a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_valid(roswasmc_bounded_1_mycobot_320_communication_MycobotAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_wire_size(roswasmc_bounded_1_mycobot_320_communication_MycobotAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_write_fields(roswasmc_bounded_1_mycobot_320_communication_MycobotAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_MycobotAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_encode(roswasmc_bounded_1_mycobot_320_communication_MycobotAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_MycobotAngles* value) {
  roswasmc_bounded_1_mycobot_320_communication_MycobotAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_MycobotAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/MycobotCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_MycobotCoords {
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
} roswasmc_bounded_1_mycobot_320_communication_MycobotCoords;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTCOORDS_CODEC_IDENTITY "UWVMROS1_1f2ada8331783764a52ed35c81c3ce9a2fdd260e2dffa2990751ce5156c3ad03"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_valid(roswasmc_bounded_1_mycobot_320_communication_MycobotCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_wire_size(roswasmc_bounded_1_mycobot_320_communication_MycobotCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_write_fields(roswasmc_bounded_1_mycobot_320_communication_MycobotCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_MycobotCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_encode(roswasmc_bounded_1_mycobot_320_communication_MycobotCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_MycobotCoords* value) {
  roswasmc_bounded_1_mycobot_320_communication_MycobotCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_MycobotCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/MycobotGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_efb4ccdb3ce35d67174700f3d3dd7332d0f7a834c5d76b6153e389e031907984"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_valid(roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_wire_size(roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_write_fields(roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_encode(roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus* value) {
  roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_MycobotGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/MycobotPumpStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTPUMPSTATUS_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTPUMPSTATUS_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTPUMPSTATUS_CODEC_IDENTITY "UWVMROS1_bb1d48067f44b7022820a6175afe05d5ba8caed9f750874a4e703389cb0d83f4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_valid(roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_wire_size(roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_write_fields(roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_encode(roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus* value) {
  roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_MycobotPumpStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/MycobotSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles {
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
} roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTSETANGLES_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTSETANGLES_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTSETANGLES_CODEC_IDENTITY "UWVMROS1_bf04b1bd24ab2f56c1835d1abe44b73fe023407c3ebdd3d35341242116a5d259"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_valid(roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_wire_size(roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_write_fields(roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_encode(roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles* value) {
  roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_MycobotSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/MycobotSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords {
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
} roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTSETCOORDS_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTSETCOORDS_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_MYCOBOTSETCOORDS_CODEC_IDENTITY "UWVMROS1_761235ef96bcc1c687ad8dd3f6d39982d1b0a7a24eaab3bb54feb9780b9e0092"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_valid(roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_wire_size(roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_write_fields(roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_encode(roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords* value) {
  roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_MycobotSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_fe13e75d9def5e6f7f831297c771cd4d319e1e9862b5026382fed23747b721b8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_valid(roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_encode(roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse {
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
} roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_c0cb03f21add6e566a30129ed7ce3174fa86786859c472a848c6768678adc80a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_valid(roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_encode(roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_82b2ae7ba4bf2aff8cd6c88ae344b376604624612abdd633c930695c1d1650d6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_valid(roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_encode(roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse {
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
} roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_821bac75a42a37fa06f51e57c09839f3557487dc8e883310601cb2e00bb6b05d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_valid(roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_encode(roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_68d072c047f4f026459309282a40c78c5db51571126efb85b7ca9e9f121b5eb5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_valid(roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_encode(roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_9c1387ec1a93d6674ac6dd735b2811957db57359e9e461dcedc70ae6fccc5544"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_valid(roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_encode(roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/PumpStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
  /** Declared field Pin1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin1;
  /** Declared field Pin2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t Pin2;
} roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_PUMPSTATUSREQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_PUMPSTATUSREQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_PUMPSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_23a6e68b36341b30ab13026cbddfd01b16d352105bb5faecd7c1ec6ad654ab60"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_valid(roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_wire_size(roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_write_fields(roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Pin2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Pin2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_encode(roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest* value) {
  roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_PumpStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/PumpStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_PUMPSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_PUMPSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_PUMPSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_037f53ebd0750c76a798ff17507759fcf71aa8a55496e5b151b8d977dff2c1c8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_valid(roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_wire_size(roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_write_fields(roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_encode(roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse* value) {
  roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_PumpStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest {
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
} roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_644a7c29bae881d5f29708b552dcd8fd4102289aa396614590bca33dbda9f7da"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_valid(roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_encode(roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_57578e8427fc6a3a89aa595259e03c3498153482afd716dd44c0d09ee9f59f20"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_valid(roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_encode(roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest {
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
} roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_f398444b45a64155c410a44da67043cee13993d75beabd6bc104b23ccd1d6656"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_valid(roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->model,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_encode(roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_320_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_320_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_3fef1bfe9036381f8ad45bc1490e96b4917df5689ccf3a84d66e5ba5145e56c4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_valid(roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_encode(roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_320_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
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
/** Complete ROS1 mycobot_communication/MycobotGetGripperValue value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue {
  /** Declared field gripper_angle; canonical capacity is part of CODEC_IDENTITY. */
  int32_t gripper_angle;
} roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_CODEC_IDENTITY "UWVMROS1_8080d3e4a1844f789af2ef6752e8b46654a9c595c7dcbb3495f49e6c62419ae9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_valid(roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->gripper_angle,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gripper_angle,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_encode(roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotGetGripperValue_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS1 mycobot_communication/MycobotSetEndType value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotSetEndType {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_MycobotSetEndType;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETENDTYPE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETENDTYPE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETENDTYPE_CODEC_IDENTITY "UWVMROS1_bb540db63ace33482ad35b74951f0423705297f9a70a4c0114cd604f6018f316"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_valid(roswasmc_bounded_1_mycobot_communication_MycobotSetEndType const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotSetEndType const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotSetEndType const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotSetEndType* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_encode(roswasmc_bounded_1_mycobot_communication_MycobotSetEndType const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotSetEndType* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotSetEndType next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotSetEndType_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotSetFreshMode value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETFRESHMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETFRESHMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETFRESHMODE_CODEC_IDENTITY "UWVMROS1_8e09301673b7d625945b4cd01e6bdd2ed1b7e6cfa1bd88beda7f409322a62c8f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_valid(roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_encode(roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotSetFreshMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotSetToolReference value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference {
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
} roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETTOOLREFERENCE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETTOOLREFERENCE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETTOOLREFERENCE_CODEC_IDENTITY "UWVMROS1_1fb3b9a6679217b95db0a430f5a1f0bece575615dfc80599277edf90190ea11c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_valid(roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_encode(roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotSetToolReference_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/MycobotSetVisionMode value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETVISIONMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETVISIONMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_MYCOBOTSETVISIONMODE_CODEC_IDENTITY "UWVMROS1_c930276fdaaef4870fc6058b156ccc574cc04b4ac96f0bc2f6031aba8016efe9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_valid(roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_wire_size(roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_write_fields(roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_encode(roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode* value) {
  roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_MycobotSetVisionMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS1 mycobot_communication/SetEndTypeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETENDTYPEREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETENDTYPEREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETENDTYPEREQUEST_CODEC_IDENTITY "UWVMROS1_59248e0ca199fb3bab9b866351648a6d1a58ed2240556122cfecb99331635472"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_valid(roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_wire_size(roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_write_fields(roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_encode(roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest* value) {
  roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetEndTypeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetEndTypeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETENDTYPERESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETENDTYPERESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETENDTYPERESPONSE_CODEC_IDENTITY "UWVMROS1_bf91211b167627be2325f38c0b2981bffa3984848681d52ed3c31d87586f9768"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_valid(roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_wire_size(roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_write_fields(roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_encode(roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse* value) {
  roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetEndTypeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetFreshModeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETFRESHMODEREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETFRESHMODEREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETFRESHMODEREQUEST_CODEC_IDENTITY "UWVMROS1_b2ed41361e0adfc856bea480c2703f183b397b4f8b53bc561c95be0ec94d9350"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_valid(roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_wire_size(roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_write_fields(roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_encode(roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest* value) {
  roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetFreshModeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetFreshModeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETFRESHMODERESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETFRESHMODERESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETFRESHMODERESPONSE_CODEC_IDENTITY "UWVMROS1_e075cb6f6112e26ec799de1a72299af83a6bb2361cb90b6aae88434c33023e6f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_valid(roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_wire_size(roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_write_fields(roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_encode(roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse* value) {
  roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetFreshModeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetToolReferenceRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest {
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
} roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETTOOLREFERENCEREQUEST_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETTOOLREFERENCEREQUEST_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETTOOLREFERENCEREQUEST_CODEC_IDENTITY "UWVMROS1_ef6dc2cc6e16fae40d20fa7d55d0d988204f4ca62fb99b755a2693aa628f7a79"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_valid(roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_wire_size(roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_write_fields(roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_encode(roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest* value) {
  roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetToolReferenceRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetToolReferenceResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETTOOLREFERENCERESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETTOOLREFERENCERESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETTOOLREFERENCERESPONSE_CODEC_IDENTITY "UWVMROS1_a14c04cc61a7b5233212072bd203a643b90c551dec9235e57d7eddf1c5e0b146"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_valid(roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_wire_size(roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_write_fields(roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_encode(roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse* value) {
  roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetToolReferenceResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetVisionModeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETVISIONMODEREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETVISIONMODEREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETVISIONMODEREQUEST_CODEC_IDENTITY "UWVMROS1_2715ecd55f7669963c076c781ceeef4ae54dac5ab71c378737ade7279c115c8b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_valid(roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_wire_size(roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_write_fields(roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_encode(roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest* value) {
  roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetVisionModeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_communication/SetVisionModeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETVISIONMODERESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETVISIONMODERESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_COMMUNICATION_SETVISIONMODERESPONSE_CODEC_IDENTITY "UWVMROS1_2caae9357e25a170c1d26630e666c2818a9a0f305e4608ad07031819f7d89cdb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_valid(roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_wire_size(roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_write_fields(roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_encode(roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse* value) {
  roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_communication_SetVisionModeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/MycobotAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles {
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
} roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTANGLES_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTANGLES_CODEC_IDENTITY "UWVMROS1_cebcee90c89b4e73d096135b0c70c8fb644f6aa32c5db066f0bf90982e25c72d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_valid(roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_encode(roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_MycobotAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/MycobotCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords {
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
} roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTCOORDS_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTCOORDS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTCOORDS_CODEC_IDENTITY "UWVMROS1_4a604335875e926bd7593712c93d8802048266e4ea926ba5447fa9b396ea32e6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_valid(roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_encode(roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_MycobotCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/MycobotGetGripperValue value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue {
  /** Declared field gripper_angle; canonical capacity is part of CODEC_IDENTITY. */
  int32_t gripper_angle;
} roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_CODEC_IDENTITY "UWVMROS1_88ad1a27a0982359460b5043bdd91a2c661ca0dc19009473ae7e0220c5215772"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_valid(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->gripper_angle,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gripper_angle,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_encode(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_MycobotGetGripperValue_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/MycobotGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_7c9ed441c8d5bac4dbf566f7a5052d99f0a01a8dcd281059fd7c329f4b232843"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_valid(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_encode(roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_MycobotGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/MycobotSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles {
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
} roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETANGLES_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETANGLES_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETANGLES_CODEC_IDENTITY "UWVMROS1_896b2be337b734707291fc763629a14362649079634aaa782630607986be6154"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_valid(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_encode(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/MycobotSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords {
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
} roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETCOORDS_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETCOORDS_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETCOORDS_CODEC_IDENTITY "UWVMROS1_27409949ade9bb2415cd1af1a278139049bc58098e7de7a5c8adc83d1ad851f6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_valid(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_encode(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/MycobotSetFreshMode value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETFRESHMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETFRESHMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_MYCOBOTSETFRESHMODE_CODEC_IDENTITY "UWVMROS1_9e06b1944e7fbab814bbaaa85f167d3804b5d11a1968f18b10e973222d0880ec"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_valid(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_encode(roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_MycobotSetFreshMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_07223209461cde1ca99ba125067ca257ceaa9bd1aeceed1ffeb6a3769c152278"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_valid(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_encode(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse {
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
} roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_000239e0594bda363723370cdde86974aa367ebf3bc93582779edbb2bf578ffb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_valid(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_encode(roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_db7906239cfb05812965b398a427fe4e23bf4f0f43fd927c82b0b2370132a649"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_valid(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_encode(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse {
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
} roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_46aee6d68fc728529b5662b86d4f6aa283ff4709a0fdeafa7604347a0ed50d72"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_valid(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_encode(roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GetGripperValueRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETGRIPPERVALUEREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETGRIPPERVALUEREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETGRIPPERVALUEREQUEST_CODEC_IDENTITY "UWVMROS1_63c7779d43e5fef7c0ec249a46840bfc1844f55efcd29933f6b7a6a9dbdadec6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_valid(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest* value) {
  return roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_encode(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GetGripperValueResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse {
  /** Declared field gripper_angle; canonical capacity is part of CODEC_IDENTITY. */
  int32_t gripper_angle;
} roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETGRIPPERVALUERESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETGRIPPERVALUERESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GETGRIPPERVALUERESPONSE_CODEC_IDENTITY "UWVMROS1_7c15e7468746ac577b70084cee685a3d71265a9f296caf5ae0b21755a2523b9e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_valid(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->gripper_angle,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gripper_angle,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_encode(roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GetGripperValueResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_10b38d264413567fdf84efc6b7f8effe15e69956db282f9b66dae8b32eddb363"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_valid(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_encode(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_0d8dc6af5bd1643e43105c6a91f9dfef97b23ea5b7ac2d55e3511959f3d75f56"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_valid(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_encode(roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest {
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
} roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_8868bfafb22b925be720901f303faeec6396f2aab40d9b3e4d94518abd5e7afd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_valid(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_encode(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_2228dd37fed5398cd2a53dd7653d6749fd5b70960c8eaa60fcfb09de4220474c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_valid(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_encode(roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest {
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
} roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_972fe4365e24ef46a54d7f655873ba4351aa6bdfd9caaec83f158655905d536d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_valid(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest const* value) {
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
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rz,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_encode(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_9c62a41f3a4198ab13a993036e78164f2cf09bb61b43a23d54f0674dec58435d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_valid(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_encode(roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/SetFreshModeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETFRESHMODEREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETFRESHMODEREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETFRESHMODEREQUEST_CODEC_IDENTITY "UWVMROS1_d744f6c3f5e9ddf6db911e423d5aa4904bbf32ff156121aa4ad7fe5ee8b28628"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_valid(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_encode(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 mycobot_pro450_communication/SetFreshModeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse;
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETFRESHMODERESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETFRESHMODERESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_MYCOBOT_PRO450_COMMUNICATION_SETFRESHMODERESPONSE_CODEC_IDENTITY "UWVMROS1_97d367a4d675b8762303e66c1d1aaab32277ac2c5fd92103eb60747214d9d431"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_valid(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_wire_size(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_write_fields(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_encode(roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse* value) {
  roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_mycobot_pro450_communication_SetFreshModeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS1 ultraarm_communication/MycobotAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_MycobotAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
} roswasmc_bounded_1_ultraarm_communication_MycobotAngles;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTANGLES_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTANGLES_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTANGLES_CODEC_IDENTITY "UWVMROS1_6bd94559b201105277e0638cff9820be0cf03efb492bc958a61a78222e1afe67"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotAngles_valid(roswasmc_bounded_1_ultraarm_communication_MycobotAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_MycobotAngles_wire_size(roswasmc_bounded_1_ultraarm_communication_MycobotAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_MycobotAngles_write_fields(roswasmc_bounded_1_ultraarm_communication_MycobotAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_MycobotAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_MycobotAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotAngles_encode(roswasmc_bounded_1_ultraarm_communication_MycobotAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_MycobotAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_MycobotAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_MycobotAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotAngles_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_MycobotAngles* value) {
  roswasmc_bounded_1_ultraarm_communication_MycobotAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_MycobotAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/MycobotCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_MycobotCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
} roswasmc_bounded_1_ultraarm_communication_MycobotCoords;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTCOORDS_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTCOORDS_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTCOORDS_CODEC_IDENTITY "UWVMROS1_f4228da3bdb19f7a63e4fbdddb7bc271debed1178663a855476c2b2425e36134"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotCoords_valid(roswasmc_bounded_1_ultraarm_communication_MycobotCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_MycobotCoords_wire_size(roswasmc_bounded_1_ultraarm_communication_MycobotCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_MycobotCoords_write_fields(roswasmc_bounded_1_ultraarm_communication_MycobotCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_MycobotCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_MycobotCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotCoords_encode(roswasmc_bounded_1_ultraarm_communication_MycobotCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_MycobotCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_MycobotCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_MycobotCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotCoords_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_MycobotCoords* value) {
  roswasmc_bounded_1_ultraarm_communication_MycobotCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_MycobotCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/MycobotGetGripperValue value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue {
  /** Declared field gripper_angle; canonical capacity is part of CODEC_IDENTITY. */
  int32_t gripper_angle;
} roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTGETGRIPPERVALUE_CODEC_IDENTITY "UWVMROS1_c5c4c82dfab7600d1ee48fc899c1e0906a80b59e03fee89ef75de5e8bbd1a672"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_valid(roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_wire_size(roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_write_fields(roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->gripper_angle,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gripper_angle,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_encode(roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue* value) {
  roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_MycobotGetGripperValue_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/MycobotGripperStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTGRIPPERSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTGRIPPERSTATUS_CODEC_IDENTITY "UWVMROS1_c3ca37a8201c045099baec080410ebfc399793a7953d36c0bce7d50e786611ce"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_valid(roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_wire_size(roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_write_fields(roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_encode(roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus* value) {
  roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_MycobotGripperStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/MycobotSetAngles value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int32_t speed;
} roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETANGLES_MAX_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETANGLES_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETANGLES_CODEC_IDENTITY "UWVMROS1_cb0fbf2cc911d7b907fbe1c09f7dbd4643439430b05976ce508bd7dbf71f9baa"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_valid(roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_wire_size(roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_write_fields(roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->speed,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_encode(roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles* value) {
  roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>20u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_MycobotSetAngles_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/MycobotSetCoords value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int32_t speed;
} roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETCOORDS_MAX_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETCOORDS_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETCOORDS_CODEC_IDENTITY "UWVMROS1_7eb6643462e1eec9cf943d9251c8b5fb47a6c1c63a4381bacd540e22c3584a68"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_valid(roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_wire_size(roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_write_fields(roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->speed,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_encode(roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords* value) {
  roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>20u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_MycobotSetCoords_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/MycobotSetFreshMode value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETFRESHMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETFRESHMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_MYCOBOTSETFRESHMODE_CODEC_IDENTITY "UWVMROS1_0934edaedfc4606d4301b38218c72e6fc323fcea3bb39143bd507ec3b410e638"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_valid(roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_wire_size(roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_write_fields(roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_encode(roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode* value) {
  roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_MycobotSetFreshMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETANGLESREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETANGLESREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_fd6922aabe6ec7f82731d19efb0963753d86a8f412de96f8bc67ea2615a7c860"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_valid(roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_wire_size(roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_write_fields(roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest* value) {
  return roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_encode(roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest* value) {
  roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
} roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETANGLESRESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETANGLESRESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_d6b80e48675b3a8f262cc463bca84353d52291cd450a656b14f5d81be851a8cf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_valid(roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_wire_size(roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_write_fields(roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_encode(roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse* value) {
  roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETCOORDSREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETCOORDSREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_40a6535b429baf0bbe0bc642ade7a8048e693633388ccd061bf12b6d795bbbd1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_valid(roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_wire_size(roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_write_fields(roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest* value) {
  return roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_encode(roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest* value) {
  roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
} roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETCOORDSRESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETCOORDSRESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_4ce97f0eb2846366887ab0ae90d2b2ca8f4571cddb5e0c7095ca7098b2e127a1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_valid(roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_wire_size(roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_write_fields(roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_encode(roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse* value) {
  roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GetGripperValueRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETGRIPPERVALUEREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETGRIPPERVALUEREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETGRIPPERVALUEREQUEST_CODEC_IDENTITY "UWVMROS1_f4daff0f9c6ba692e9b20e29cc67066afb3931d146ba995848dcdc83db2b52b1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_valid(roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_wire_size(roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_write_fields(roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest* value) {
  return roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_encode(roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest* value) {
  roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GetGripperValueRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GetGripperValueResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse {
  /** Declared field gripper_angle; canonical capacity is part of CODEC_IDENTITY. */
  int32_t gripper_angle;
} roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETGRIPPERVALUERESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETGRIPPERVALUERESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GETGRIPPERVALUERESPONSE_CODEC_IDENTITY "UWVMROS1_ff7452fb5fb1f5a55c8db0a2aa6d18df1a62a85d32a659506dd31a1ce1721c90"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_valid(roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_wire_size(roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_write_fields(roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->gripper_angle,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gripper_angle,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_encode(roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse* value) {
  roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GetGripperValueResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GripperStatusRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GRIPPERSTATUSREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GRIPPERSTATUSREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GRIPPERSTATUSREQUEST_CODEC_IDENTITY "UWVMROS1_c435d39bef04bd7022e048c0cf364d100bf7a92855f04c816d2a22f0e672d586"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_valid(roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest const* value) {
  if(!value)return 0;
  if(value->Status>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_wire_size(roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_write_fields(roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_encode(roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest* value) {
  roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GripperStatusRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/GripperStatusResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_GRIPPERSTATUSRESPONSE_CODEC_IDENTITY "UWVMROS1_fa0c99ff37acea63c5d507174f9725a9341ffaa007e722e21a8e0b93097e56d4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_valid(roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_wire_size(roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_write_fields(roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_encode(roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse* value) {
  roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_GripperStatusResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/SetAnglesRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest {
  /** Declared field joint_1; canonical capacity is part of CODEC_IDENTITY. */
  float joint_1;
  /** Declared field joint_2; canonical capacity is part of CODEC_IDENTITY. */
  float joint_2;
  /** Declared field joint_3; canonical capacity is part of CODEC_IDENTITY. */
  float joint_3;
  /** Declared field joint_4; canonical capacity is part of CODEC_IDENTITY. */
  float joint_4;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int32_t speed;
} roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETANGLESREQUEST_MAX_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETANGLESREQUEST_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETANGLESREQUEST_CODEC_IDENTITY "UWVMROS1_5a1aab8379585f404a15dbfd8832dd6434d969f41681127befca7458181fc65a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_valid(roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_wire_size(roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_write_fields(roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joint_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joint_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->speed,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_encode(roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest* value) {
  roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>20u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_SetAnglesRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/SetAnglesResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETANGLESRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETANGLESRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETANGLESRESPONSE_CODEC_IDENTITY "UWVMROS1_f1ecaff9a60a47ef978b0faf2b9b2b9166ae308b3df718b0536536004dc8a3b4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_valid(roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_wire_size(roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_write_fields(roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_encode(roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse* value) {
  roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_SetAnglesResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/SetCoordsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int32_t speed;
} roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETCOORDSREQUEST_MAX_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETCOORDSREQUEST_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETCOORDSREQUEST_CODEC_IDENTITY "UWVMROS1_54d83be688da3c885a7166e4d2c1c636a1144ba863a0a388907511896d5626e5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_valid(roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_wire_size(roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_write_fields(roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->speed,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_encode(roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest* value) {
  roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>20u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_SetCoordsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/SetCoordsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETCOORDSRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETCOORDSRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETCOORDSRESPONSE_CODEC_IDENTITY "UWVMROS1_02aa6c82612c5ffd43e890bb173a59245e00cd91dbc38303dcbdc0fb30683431"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_valid(roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_wire_size(roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_write_fields(roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_encode(roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse* value) {
  roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_SetCoordsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/SetFreshModeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest {
  /** Declared field Status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Status;
} roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETFRESHMODEREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETFRESHMODEREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETFRESHMODEREQUEST_CODEC_IDENTITY "UWVMROS1_142c43e6dcd72843cdd002e5b6815524b2b6a5a02372e5b966dc630947a90649"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_valid(roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_wire_size(roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_write_fields(roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Status,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_encode(roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest* value) {
  roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_SetFreshModeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ultraarm_communication/SetFreshModeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse {
  /** Declared field Flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Flag;
} roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse;
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETFRESHMODERESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETFRESHMODERESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ULTRAARM_COMMUNICATION_SETFRESHMODERESPONSE_CODEC_IDENTITY "UWVMROS1_764d9c9dfff1f18a5501c5f040a6bc66ae6f93bfb7b932a33c2ece2a70d38104"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_valid(roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse const* value) {
  if(!value)return 0;
  if(value->Flag>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_wire_size(roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_write_fields(roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->Flag,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Flag,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_encode(roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse* value) {
  roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ultraarm_communication_SetFreshModeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
