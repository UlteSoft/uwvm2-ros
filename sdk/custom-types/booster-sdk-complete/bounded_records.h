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
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/ImuState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState {
  /** Declared field rpy; canonical capacity is part of CODEC_IDENTITY. */
  float rpy[3];
  /** Declared field gyro; canonical capacity is part of CODEC_IDENTITY. */
  float gyro[3];
  /** Declared field acc; canonical capacity is part of CODEC_IDENTITY. */
  float acc[3];
} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState;
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_IMUSTATE_MAX_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_IMUSTATE_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_IMUSTATE_CODEC_IDENTITY "UWVMROS1_858ad4a47d2409da6b4e2513178d7d3b0d4568d6ce488729de0b9a220a100f04"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_valid(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rpy[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gyro[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->acc[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_wire_size(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState const* value) {size_t n=0;
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_write_fields(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rpy[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gyro[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->acc[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState* value) {
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rpy[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gyro[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->acc[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_encode(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState* value) {
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>36u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/MotorCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  float q;
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  float dq;
  /** Declared field tau; canonical capacity is part of CODEC_IDENTITY. */
  float tau;
  /** Declared field kp; canonical capacity is part of CODEC_IDENTITY. */
  float kp;
  /** Declared field kd; canonical capacity is part of CODEC_IDENTITY. */
  float kd;
  /** Declared field weight; canonical capacity is part of CODEC_IDENTITY. */
  float weight;
} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd;
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_MOTORCMD_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_MOTORCMD_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_MOTORCMD_CODEC_IDENTITY "UWVMROS1_66fec912031dfbd29b8e4e747dcdc2dc465506bd003323bc929929bf6a9d31a4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_valid(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->weight,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_wire_size(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_write_fields(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->weight,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kp,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kd,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->weight,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_encode(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd* value) {
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/MotorState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  float q;
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  float dq;
  /** Declared field ddq; canonical capacity is part of CODEC_IDENTITY. */
  float ddq;
  /** Declared field tau_est; canonical capacity is part of CODEC_IDENTITY. */
  float tau_est;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t temperature;
  /** Declared field lost; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t lost;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[2];
} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState;
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_MOTORSTATE_MAX_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_MOTORSTATE_MIN_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_MOTORSTATE_CODEC_IDENTITY "UWVMROS1_958168dc1e32d50827f255080117a82421d7fb6ae4ad6a05763ce2d3173a30af"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_valid(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau_est,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_wire_size(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_write_fields(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau_est,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->lost,4);*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ddq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau_est,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->temperature,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->lost,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_encode(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState* value) {
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState next={0};size_t offset=0;if(!value||(length&&!input)||length<30u||length>30u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most29 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd data[29];} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_motor_cmd_sequence;
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/LowCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd {
  /** Declared field cmd_type; canonical capacity is part of CODEC_IDENTITY. */
  int32_t cmd_type;
  /** Declared field motor_cmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_motor_cmd_sequence motor_cmd;
} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd;
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_LOWCMD_MAX_WIRE_BYTES 733u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_LOWCMD_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_LOWCMD_CODEC_IDENTITY "UWVMROS1_3324a161b056f9282ffa8dce3254a1456b75f6eae5f4a067e844e125e94cba3a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_valid(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd const* value) {
  if(!value)return 0;
  if(value->motor_cmd.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_valid(&value->motor_cmd.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_wire_size(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd const* value) {size_t n=0;
  n+=4;
  n+=4;
  if(value->motor_cmd.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  n+=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_wire_size(&value->motor_cmd.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_write_fields(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->cmd_type,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->motor_cmd.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_write_fields(&value->motor_cmd.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cmd_type,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->motor_cmd.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_cmd.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorCmd_read_fields(p,n,offset,&value->motor_cmd.data[i]))return 0;
  }}
  return roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_encode(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd* value) {
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>733u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most29 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState data[29];} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_motor_state_parallel_sequence;
/** At most29 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState data[29];} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_motor_state_serial_sequence;
/** Complete ROS2 uwvm_booster_sdk_bridge/msg/LowState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState {
  /** Declared field imu_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState imu_state;
  /** Declared field motor_state_parallel; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_motor_state_parallel_sequence motor_state_parallel;
  /** Declared field motor_state_serial; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_motor_state_serial_sequence motor_state_serial;
} roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState;
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_LOWSTATE_MAX_WIRE_BYTES 1784u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_LOWSTATE_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_2_UWVM_BOOSTER_SDK_BRIDGE_MSG_LOWSTATE_CODEC_IDENTITY "UWVMROS1_ca4bcb79077b0e026cf45a389981622ff9c80570ee813ff5633c605fe5021a58"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_valid(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_valid(&value->imu_state))return 0;
  if(value->motor_state_parallel.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_valid(&value->motor_state_parallel.data[i]))return 0;
  }}
  if(value->motor_state_serial.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_valid(&value->motor_state_serial.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_wire_size(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_wire_size(&value->imu_state);
  n+=4;
  if(value->motor_state_parallel.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  n+=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_wire_size(&value->motor_state_parallel.data[i]);
  }}
  n+=4;
  if(value->motor_state_serial.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  n+=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_wire_size(&value->motor_state_serial.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_write_fields(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_write_fields(&value->imu_state,p,offset);
  uwvm_bounded_put(p+*offset,value->motor_state_parallel.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_write_fields(&value->motor_state_parallel.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->motor_state_serial.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_write_fields(&value->motor_state_serial.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState* value) {
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_ImuState_read_fields(p,n,offset,&value->imu_state))return 0;
  if(*offset>n||n-*offset<4)return 0;value->motor_state_parallel.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_state_parallel.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_read_fields(p,n,offset,&value->motor_state_parallel.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->motor_state_serial.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_state_serial.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_MotorState_read_fields(p,n,offset,&value->motor_state_serial.data[i]))return 0;
  }}
  return roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_encode(roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState* value) {
  roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>1784u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_booster_sdk_bridge_msg_LowState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
