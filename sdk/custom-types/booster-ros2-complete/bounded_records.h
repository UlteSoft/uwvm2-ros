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
/** UTF-8 byte storage for booster_interface/msg/BoosterApiReqMsg.body; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_body_text;
/** Complete ROS2 booster_interface/msg/BoosterApiReqMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg {
  /** Declared field api_id; canonical capacity is part of CODEC_IDENTITY. */
  int64_t api_id;
  /** Declared field body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_body_text body;
} roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BOOSTERAPIREQMSG_MAX_WIRE_BYTES 4108u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BOOSTERAPIREQMSG_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BOOSTERAPIREQMSG_CODEC_IDENTITY "UWVMROS1_e2f9b66467bcb40a8194c744694f314e7fca56061f1cf3121eed565f724edd40"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_valid(roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg const* value) {
  if(!value)return 0;
  if(value->body.size>4096u||!uwvm_bounded_utf8(value->body.data,value->body.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_wire_size(roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg const* value) {size_t n=0;
  n+=8;
  n+=4+value->body.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_write_fields(roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->api_id,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->body.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->body.data,value->body.size);*offset+=value->body.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->api_id,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->body.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->body.size>4096u||value->body.size>n-*offset)return 0;uwvm_bounded_copy(value->body.data,p+*offset,value->body.size);*offset+=value->body.size;
  return roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_encode(roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg* value) {
  roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>4108u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for booster_interface/msg/BoosterApiRespMsg.body; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_body_text;
/** Complete ROS2 booster_interface/msg/BoosterApiRespMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  int64_t status;
  /** Declared field body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_body_text body;
} roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BOOSTERAPIRESPMSG_MAX_WIRE_BYTES 4108u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BOOSTERAPIRESPMSG_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BOOSTERAPIRESPMSG_CODEC_IDENTITY "UWVMROS1_21ce208a51f60843eecb72edbc408e926353b1a6f178424c0b691ac237fed11f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_valid(roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg const* value) {
  if(!value)return 0;
  if(value->body.size>4096u||!uwvm_bounded_utf8(value->body.data,value->body.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_wire_size(roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg const* value) {size_t n=0;
  n+=8;
  n+=4+value->body.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_write_fields(roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->body.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->body.data,value->body.size);*offset+=value->body.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->status,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->body.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->body.size>4096u||value->body.size>n-*offset)return 0;uwvm_bounded_copy(value->body.data,p+*offset,value->body.size);*offset+=value->body.size;
  return roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_encode(roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg* value) {
  roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>4108u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/ButtonEventMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg {
  /** Declared field event; canonical capacity is part of CODEC_IDENTITY. */
  int8_t event;
  /** Declared field button; canonical capacity is part of CODEC_IDENTITY. */
  int32_t button;
} roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_CODEC_IDENTITY "UWVMROS1_b61682fd9c35bd54c52000224411f4373b8620e16587b31d8a683076e0d14fe4"
/** Original scalar constant PRESS_DOWN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_PRESS_DOWN INT32_C(0)
/** Original scalar constant PRESS_UP=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_PRESS_UP INT32_C(1)
/** Original scalar constant SINGLE_CLICK=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_SINGLE_CLICK INT32_C(0)
/** Original scalar constant DOUBLE_CLICK=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_DOUBLE_CLICK INT32_C(1)
/** Original scalar constant TRIPLE_CLICK=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_TRIPLE_CLICK INT32_C(0)
/** Original scalar constant LONG_PRESS_START=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_LONG_PRESS_START INT32_C(1)
/** Original scalar constant LONG_PRESS_HOLD=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_LONG_PRESS_HOLD INT32_C(0)
/** Original scalar constant LONG_PRESS_END=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_BUTTONEVENTMSG_LONG_PRESS_END INT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_valid(roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_wire_size(roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg const* value) {size_t n=0;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_write_fields(roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->event,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->button,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->event,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->button,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_encode(roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg* value) {
  roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_ButtonEventMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/FallDownState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_FallDownState {
  /** Declared field fall_down_state; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t fall_down_state;
  /** Declared field is_recovery_available; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_recovery_available;
} roswasmc_bounded_2_booster_interface_msg_FallDownState;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_FALLDOWNSTATE_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_FALLDOWNSTATE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_FALLDOWNSTATE_CODEC_IDENTITY "UWVMROS1_8bf168ccefac72eda6070704c70e12e62776d29ad3db908095cc01220f4b533f"
/** Original scalar constant IS_READY=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_FALLDOWNSTATE_IS_READY UINT32_C(0)
/** Original scalar constant IS_FALLING=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_FALLDOWNSTATE_IS_FALLING UINT32_C(1)
/** Original scalar constant HAS_FALLEN=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_FALLDOWNSTATE_HAS_FALLEN UINT32_C(2)
/** Original scalar constant IS_GETTING_UP=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_FALLDOWNSTATE_IS_GETTING_UP UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_FallDownState_valid(roswasmc_bounded_2_booster_interface_msg_FallDownState const* value) {
  if(!value)return 0;
  if(value->is_recovery_available>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_FallDownState_wire_size(roswasmc_bounded_2_booster_interface_msg_FallDownState const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_FallDownState_write_fields(roswasmc_bounded_2_booster_interface_msg_FallDownState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->fall_down_state,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_recovery_available,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_FallDownState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_FallDownState* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->fall_down_state,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_recovery_available,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_booster_interface_msg_FallDownState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_FallDownState_encode(roswasmc_bounded_2_booster_interface_msg_FallDownState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_FallDownState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_FallDownState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_FallDownState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_FallDownState_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_FallDownState* value) {
  roswasmc_bounded_2_booster_interface_msg_FallDownState next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_FallDownState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/HandParam value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_HandParam {
  /** Declared field angle; canonical capacity is part of CODEC_IDENTITY. */
  int32_t angle;
  /** Declared field force; canonical capacity is part of CODEC_IDENTITY. */
  int32_t force;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  int32_t speed;
  /** Declared field seq; canonical capacity is part of CODEC_IDENTITY. */
  int32_t seq;
} roswasmc_bounded_2_booster_interface_msg_HandParam;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDPARAM_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDPARAM_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDPARAM_CODEC_IDENTITY "UWVMROS1_3b91722e751e7c00c773335bb42af34850190ecc26a029e8792c49d912def158"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_HandParam_valid(roswasmc_bounded_2_booster_interface_msg_HandParam const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_HandParam_wire_size(roswasmc_bounded_2_booster_interface_msg_HandParam const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_HandParam_write_fields(roswasmc_bounded_2_booster_interface_msg_HandParam const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->angle,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->force,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->seq,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_HandParam_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_HandParam* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->angle,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->force,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->speed,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->seq,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_booster_interface_msg_HandParam_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_HandParam_encode(roswasmc_bounded_2_booster_interface_msg_HandParam const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_HandParam_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_HandParam_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_HandParam_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_HandParam_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_HandParam* value) {
  roswasmc_bounded_2_booster_interface_msg_HandParam next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_HandParam_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_booster_interface_msg_HandParam data[32];} roswasmc_bounded_2_booster_interface_msg_HandCommand_hand_param_sequence;
/** Complete ROS2 booster_interface/msg/HandCommand value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_HandCommand {
  /** Declared field hand_param; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_HandCommand_hand_param_sequence hand_param;
  /** Declared field force_mode; canonical capacity is part of CODEC_IDENTITY. */
  int32_t force_mode;
  /** Declared field hand_index; canonical capacity is part of CODEC_IDENTITY. */
  int32_t hand_index;
  /** Declared field hand_type; canonical capacity is part of CODEC_IDENTITY. */
  int32_t hand_type;
} roswasmc_bounded_2_booster_interface_msg_HandCommand;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDCOMMAND_MAX_WIRE_BYTES 528u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDCOMMAND_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDCOMMAND_CODEC_IDENTITY "UWVMROS1_06f3d6c0e9bef1da409330038aadbfacfe60b02e099f2ae0c4dcb92a632b20a0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_HandCommand_valid(roswasmc_bounded_2_booster_interface_msg_HandCommand const* value) {
  if(!value)return 0;
  if(value->hand_param.size>32u)return 0;
  {size_t i;for(i=0;i<value->hand_param.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_HandParam_valid(&value->hand_param.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_HandCommand_wire_size(roswasmc_bounded_2_booster_interface_msg_HandCommand const* value) {size_t n=0;
  n+=4;
  if(value->hand_param.size>32u)return 0;
  {size_t i;for(i=0;i<value->hand_param.size;++i){
  n+=roswasmc_bounded_2_booster_interface_msg_HandParam_wire_size(&value->hand_param.data[i]);
  }}
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_HandCommand_write_fields(roswasmc_bounded_2_booster_interface_msg_HandCommand const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->hand_param.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->hand_param.size;++i){
  roswasmc_bounded_2_booster_interface_msg_HandParam_write_fields(&value->hand_param.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->force_mode,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hand_index,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hand_type,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_HandCommand_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_HandCommand* value) {
  if(*offset>n||n-*offset<4)return 0;value->hand_param.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->hand_param.size>32u)return 0;
  {size_t i;for(i=0;i<value->hand_param.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_HandParam_read_fields(p,n,offset,&value->hand_param.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->force_mode,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->hand_index,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->hand_type,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_booster_interface_msg_HandCommand_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_HandCommand_encode(roswasmc_bounded_2_booster_interface_msg_HandCommand const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_HandCommand_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_HandCommand_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_HandCommand_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_HandCommand_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_HandCommand* value) {
  roswasmc_bounded_2_booster_interface_msg_HandCommand next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>528u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_HandCommand_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_booster_interface_msg_HandCommand data[2];} roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_hands_vec_sequence;
/** Complete ROS2 booster_interface/msg/HandDdsMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_HandDdsMsg {
  /** Declared field hands_vec; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_hands_vec_sequence hands_vec;
} roswasmc_bounded_2_booster_interface_msg_HandDdsMsg;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDDDSMSG_MAX_WIRE_BYTES 1060u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDDDSMSG_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_HANDDDSMSG_CODEC_IDENTITY "UWVMROS1_55af9d6f272a72e75e86e1a6164d235aca5f2ce99c2529749c0d5e33d6091397"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_valid(roswasmc_bounded_2_booster_interface_msg_HandDdsMsg const* value) {
  if(!value)return 0;
  if(value->hands_vec.size>2u)return 0;
  {size_t i;for(i=0;i<value->hands_vec.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_HandCommand_valid(&value->hands_vec.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_wire_size(roswasmc_bounded_2_booster_interface_msg_HandDdsMsg const* value) {size_t n=0;
  n+=4;
  if(value->hands_vec.size>2u)return 0;
  {size_t i;for(i=0;i<value->hands_vec.size;++i){
  n+=roswasmc_bounded_2_booster_interface_msg_HandCommand_wire_size(&value->hands_vec.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_write_fields(roswasmc_bounded_2_booster_interface_msg_HandDdsMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->hands_vec.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->hands_vec.size;++i){
  roswasmc_bounded_2_booster_interface_msg_HandCommand_write_fields(&value->hands_vec.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_HandDdsMsg* value) {
  if(*offset>n||n-*offset<4)return 0;value->hands_vec.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->hands_vec.size>2u)return 0;
  {size_t i;for(i=0;i<value->hands_vec.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_HandCommand_read_fields(p,n,offset,&value->hands_vec.data[i]))return 0;
  }}
  return roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_encode(roswasmc_bounded_2_booster_interface_msg_HandDdsMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_HandDdsMsg* value) {
  roswasmc_bounded_2_booster_interface_msg_HandDdsMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>1060u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_HandDdsMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/ImuState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_ImuState {
  /** Declared field rpy; canonical capacity is part of CODEC_IDENTITY. */
  float rpy[3];
  /** Declared field gyro; canonical capacity is part of CODEC_IDENTITY. */
  float gyro[3];
  /** Declared field acc; canonical capacity is part of CODEC_IDENTITY. */
  float acc[3];
} roswasmc_bounded_2_booster_interface_msg_ImuState;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_IMUSTATE_MAX_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_IMUSTATE_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_IMUSTATE_CODEC_IDENTITY "UWVMROS1_838ee8d4b93b0c3673e0b535fd3905913e94be24d6d21a01504dedaaaaa81092"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_ImuState_valid(roswasmc_bounded_2_booster_interface_msg_ImuState const* value) {
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
static inline size_t roswasmc_bounded_2_booster_interface_msg_ImuState_wire_size(roswasmc_bounded_2_booster_interface_msg_ImuState const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_booster_interface_msg_ImuState_write_fields(roswasmc_bounded_2_booster_interface_msg_ImuState const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_2_booster_interface_msg_ImuState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_ImuState* value) {
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rpy[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gyro[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->acc[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_booster_interface_msg_ImuState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_ImuState_encode(roswasmc_bounded_2_booster_interface_msg_ImuState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_ImuState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_ImuState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_ImuState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_ImuState_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_ImuState* value) {
  roswasmc_bounded_2_booster_interface_msg_ImuState next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>36u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_ImuState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/MotorCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_MotorCmd {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t mode;
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
} roswasmc_bounded_2_booster_interface_msg_MotorCmd;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_MOTORCMD_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_MOTORCMD_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_MOTORCMD_CODEC_IDENTITY "UWVMROS1_69d814e13595d53dcf44f2a5ccb19df0f8f2be0908c310c3b60cec33bcf27392"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_MotorCmd_valid(roswasmc_bounded_2_booster_interface_msg_MotorCmd const* value) {
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
static inline size_t roswasmc_bounded_2_booster_interface_msg_MotorCmd_wire_size(roswasmc_bounded_2_booster_interface_msg_MotorCmd const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_booster_interface_msg_MotorCmd_write_fields(roswasmc_bounded_2_booster_interface_msg_MotorCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->weight,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_MotorCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_MotorCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kp,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kd,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->weight,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_booster_interface_msg_MotorCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_MotorCmd_encode(roswasmc_bounded_2_booster_interface_msg_MotorCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_MotorCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_MotorCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_MotorCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_MotorCmd_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_MotorCmd* value) {
  roswasmc_bounded_2_booster_interface_msg_MotorCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_MotorCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most29 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_booster_interface_msg_MotorCmd data[29];} roswasmc_bounded_2_booster_interface_msg_LowCmd_motor_cmd_sequence;
/** Complete ROS2 booster_interface/msg/LowCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_LowCmd {
  /** Declared field cmd_type; canonical capacity is part of CODEC_IDENTITY. */
  int8_t cmd_type;
  /** Declared field motor_cmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_LowCmd_motor_cmd_sequence motor_cmd;
} roswasmc_bounded_2_booster_interface_msg_LowCmd;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWCMD_MAX_WIRE_BYTES 730u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWCMD_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWCMD_CODEC_IDENTITY "UWVMROS1_845726a14e61d211b493041f4dda56e1556b76b9d30337a2ee5f6319a3d1e870"
/** Original scalar constant CMD_TYPE_PARALLEL=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWCMD_CMD_TYPE_PARALLEL INT32_C(0)
/** Original scalar constant CMD_TYPE_SERIAL=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWCMD_CMD_TYPE_SERIAL INT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_LowCmd_valid(roswasmc_bounded_2_booster_interface_msg_LowCmd const* value) {
  if(!value)return 0;
  if(value->motor_cmd.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_MotorCmd_valid(&value->motor_cmd.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_LowCmd_wire_size(roswasmc_bounded_2_booster_interface_msg_LowCmd const* value) {size_t n=0;
  n+=1;
  n+=4;
  if(value->motor_cmd.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  n+=roswasmc_bounded_2_booster_interface_msg_MotorCmd_wire_size(&value->motor_cmd.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_LowCmd_write_fields(roswasmc_bounded_2_booster_interface_msg_LowCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->cmd_type,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->motor_cmd.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  roswasmc_bounded_2_booster_interface_msg_MotorCmd_write_fields(&value->motor_cmd.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_LowCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_LowCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cmd_type,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->motor_cmd.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_cmd.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_cmd.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_MotorCmd_read_fields(p,n,offset,&value->motor_cmd.data[i]))return 0;
  }}
  return roswasmc_bounded_2_booster_interface_msg_LowCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_LowCmd_encode(roswasmc_bounded_2_booster_interface_msg_LowCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_LowCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_LowCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_LowCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_LowCmd_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_LowCmd* value) {
  roswasmc_bounded_2_booster_interface_msg_LowCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>730u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_LowCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/MotorState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_MotorState {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t mode;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  float q;
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  float dq;
  /** Declared field ddq; canonical capacity is part of CODEC_IDENTITY. */
  float ddq;
  /** Declared field tau_est; canonical capacity is part of CODEC_IDENTITY. */
  float tau_est;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int8_t temperature;
  /** Declared field lost; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t lost;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[2];
} roswasmc_bounded_2_booster_interface_msg_MotorState;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_MOTORSTATE_MAX_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_MOTORSTATE_MIN_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_MOTORSTATE_CODEC_IDENTITY "UWVMROS1_6b19c0e8865e796fff5a498cff731ba268fa92ea2944e6c2c5869009b3aa066b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_MotorState_valid(roswasmc_bounded_2_booster_interface_msg_MotorState const* value) {
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
static inline size_t roswasmc_bounded_2_booster_interface_msg_MotorState_wire_size(roswasmc_bounded_2_booster_interface_msg_MotorState const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_booster_interface_msg_MotorState_write_fields(roswasmc_bounded_2_booster_interface_msg_MotorState const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_2_booster_interface_msg_MotorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_MotorState* value) {
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
  return roswasmc_bounded_2_booster_interface_msg_MotorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_MotorState_encode(roswasmc_bounded_2_booster_interface_msg_MotorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_MotorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_MotorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_MotorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_MotorState_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_MotorState* value) {
  roswasmc_bounded_2_booster_interface_msg_MotorState next={0};size_t offset=0;if(!value||(length&&!input)||length<30u||length>30u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_MotorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most29 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_booster_interface_msg_MotorState data[29];} roswasmc_bounded_2_booster_interface_msg_LowState_motor_state_parallel_sequence;
/** At most29 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_booster_interface_msg_MotorState data[29];} roswasmc_bounded_2_booster_interface_msg_LowState_motor_state_serial_sequence;
/** Complete ROS2 booster_interface/msg/LowState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_LowState {
  /** Declared field imu_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_ImuState imu_state;
  /** Declared field motor_state_parallel; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_LowState_motor_state_parallel_sequence motor_state_parallel;
  /** Declared field motor_state_serial; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_LowState_motor_state_serial_sequence motor_state_serial;
} roswasmc_bounded_2_booster_interface_msg_LowState;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWSTATE_MAX_WIRE_BYTES 1784u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWSTATE_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_LOWSTATE_CODEC_IDENTITY "UWVMROS1_811db19cab7b790ed48506bca3e3ef512c1a661dc3ac99cbce6234efb6de631f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_LowState_valid(roswasmc_bounded_2_booster_interface_msg_LowState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_booster_interface_msg_ImuState_valid(&value->imu_state))return 0;
  if(value->motor_state_parallel.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_MotorState_valid(&value->motor_state_parallel.data[i]))return 0;
  }}
  if(value->motor_state_serial.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_MotorState_valid(&value->motor_state_serial.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_LowState_wire_size(roswasmc_bounded_2_booster_interface_msg_LowState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_booster_interface_msg_ImuState_wire_size(&value->imu_state);
  n+=4;
  if(value->motor_state_parallel.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  n+=roswasmc_bounded_2_booster_interface_msg_MotorState_wire_size(&value->motor_state_parallel.data[i]);
  }}
  n+=4;
  if(value->motor_state_serial.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  n+=roswasmc_bounded_2_booster_interface_msg_MotorState_wire_size(&value->motor_state_serial.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_LowState_write_fields(roswasmc_bounded_2_booster_interface_msg_LowState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_booster_interface_msg_ImuState_write_fields(&value->imu_state,p,offset);
  uwvm_bounded_put(p+*offset,value->motor_state_parallel.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  roswasmc_bounded_2_booster_interface_msg_MotorState_write_fields(&value->motor_state_parallel.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->motor_state_serial.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  roswasmc_bounded_2_booster_interface_msg_MotorState_write_fields(&value->motor_state_serial.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_LowState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_LowState* value) {
  if(!roswasmc_bounded_2_booster_interface_msg_ImuState_read_fields(p,n,offset,&value->imu_state))return 0;
  if(*offset>n||n-*offset<4)return 0;value->motor_state_parallel.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_state_parallel.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_parallel.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_MotorState_read_fields(p,n,offset,&value->motor_state_parallel.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->motor_state_serial.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_state_serial.size>29u)return 0;
  {size_t i;for(i=0;i<value->motor_state_serial.size;++i){
  if(!roswasmc_bounded_2_booster_interface_msg_MotorState_read_fields(p,n,offset,&value->motor_state_serial.data[i]))return 0;
  }}
  return roswasmc_bounded_2_booster_interface_msg_LowState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_LowState_encode(roswasmc_bounded_2_booster_interface_msg_LowState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_LowState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_LowState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_LowState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_LowState_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_LowState* value) {
  roswasmc_bounded_2_booster_interface_msg_LowState next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>1784u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_LowState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/Odometer value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_Odometer {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field theta; canonical capacity is part of CODEC_IDENTITY. */
  float theta;
} roswasmc_bounded_2_booster_interface_msg_Odometer;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_ODOMETER_MAX_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_ODOMETER_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_ODOMETER_CODEC_IDENTITY "UWVMROS1_4fea33e3e2ae186dbeaf49c6068019b702e742266541087a5d280b9ce42c671a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_Odometer_valid(roswasmc_bounded_2_booster_interface_msg_Odometer const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->theta,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_Odometer_wire_size(roswasmc_bounded_2_booster_interface_msg_Odometer const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_Odometer_write_fields(roswasmc_bounded_2_booster_interface_msg_Odometer const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->theta,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_Odometer_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_Odometer* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->theta,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_booster_interface_msg_Odometer_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_Odometer_encode(roswasmc_bounded_2_booster_interface_msg_Odometer const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_Odometer_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_Odometer_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_Odometer_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_Odometer_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_Odometer* value) {
  roswasmc_bounded_2_booster_interface_msg_Odometer next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>12u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_Odometer_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2048 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2048];} roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_msg_sequence;
/** Complete ROS2 booster_interface/msg/RawBytesMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_RawBytesMsg {
  /** Declared field msg; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_msg_sequence msg;
} roswasmc_bounded_2_booster_interface_msg_RawBytesMsg;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_RAWBYTESMSG_MAX_WIRE_BYTES 2052u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_RAWBYTESMSG_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_RAWBYTESMSG_CODEC_IDENTITY "UWVMROS1_557e8e79f206c9cc8b820ed7c2ad8cc01e9b01327197b745709993d958b4578c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_valid(roswasmc_bounded_2_booster_interface_msg_RawBytesMsg const* value) {
  if(!value)return 0;
  if(value->msg.size>2048u)return 0;
  {size_t i;for(i=0;i<value->msg.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_wire_size(roswasmc_bounded_2_booster_interface_msg_RawBytesMsg const* value) {size_t n=0;
  n+=4;
  if(value->msg.size>2048u)return 0;
  {size_t i;for(i=0;i<value->msg.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_write_fields(roswasmc_bounded_2_booster_interface_msg_RawBytesMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->msg.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->msg.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->msg.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_RawBytesMsg* value) {
  if(*offset>n||n-*offset<4)return 0;value->msg.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->msg.size>2048u)return 0;
  {size_t i;for(i=0;i<value->msg.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->msg.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_encode(roswasmc_bounded_2_booster_interface_msg_RawBytesMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_RawBytesMsg* value) {
  roswasmc_bounded_2_booster_interface_msg_RawBytesMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>2052u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_RawBytesMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 builtin_interfaces/msg/Time value; no field projects away native data. */
typedef struct roswasmc_bounded_2_builtin_interfaces_msg_Time {
  /** Declared field sec; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sec;
  /** Declared field nanosec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t nanosec;
} roswasmc_bounded_2_builtin_interfaces_msg_Time;
#define ROSWASMC_BOUNDED_2_BUILTIN_INTERFACES_MSG_TIME_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_BUILTIN_INTERFACES_MSG_TIME_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_BUILTIN_INTERFACES_MSG_TIME_CODEC_IDENTITY "UWVMROS1_0fd520c23fd8cb06e2c27292fdc20694732fa9332d452fc3fa31da90f7605fb7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(roswasmc_bounded_2_builtin_interfaces_msg_Time const* value) {
  if(!value)return 0;
  if(value->nanosec>=1000000000)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(roswasmc_bounded_2_builtin_interfaces_msg_Time const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_builtin_interfaces_msg_Time_write_fields(roswasmc_bounded_2_builtin_interfaces_msg_Time const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->sec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nanosec,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_builtin_interfaces_msg_Time_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_builtin_interfaces_msg_Time* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->nanosec,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_builtin_interfaces_msg_Time_encode(roswasmc_bounded_2_builtin_interfaces_msg_Time const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_builtin_interfaces_msg_Time_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_builtin_interfaces_msg_Time_decode(void const* input,size_t length,roswasmc_bounded_2_builtin_interfaces_msg_Time* value) {
  roswasmc_bounded_2_builtin_interfaces_msg_Time next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for std_msgs/msg/Header.frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_std_msgs_msg_Header_frame_id_text;
/** Complete ROS2 std_msgs/msg/Header value; no field projects away native data. */
typedef struct roswasmc_bounded_2_std_msgs_msg_Header {
  /** Declared field stamp; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Time stamp;
  /** Declared field frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header_frame_id_text frame_id;
} roswasmc_bounded_2_std_msgs_msg_Header;
#define ROSWASMC_BOUNDED_2_STD_MSGS_MSG_HEADER_MAX_WIRE_BYTES 140u
#define ROSWASMC_BOUNDED_2_STD_MSGS_MSG_HEADER_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_STD_MSGS_MSG_HEADER_CODEC_IDENTITY "UWVMROS1_ce081e9caac05e464c4cff0da5e94cad44014652c0ddf244ee6c35ac3b0d1e96"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_std_msgs_msg_Header_valid(roswasmc_bounded_2_std_msgs_msg_Header const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(&value->stamp))return 0;
  if(value->frame_id.size>128u||!uwvm_bounded_utf8(value->frame_id.data,value->frame_id.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_std_msgs_msg_Header_wire_size(roswasmc_bounded_2_std_msgs_msg_Header const* value) {size_t n=0;
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(&value->stamp);
  n+=4+value->frame_id.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_std_msgs_msg_Header_write_fields(roswasmc_bounded_2_std_msgs_msg_Header const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_builtin_interfaces_msg_Time_write_fields(&value->stamp,p,offset);
  uwvm_bounded_put(p+*offset,value->frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->frame_id.data,value->frame_id.size);*offset+=value->frame_id.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_std_msgs_msg_Header_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_std_msgs_msg_Header* value) {
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_read_fields(p,n,offset,&value->stamp))return 0;
  if(*offset>n||n-*offset<4)return 0;value->frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->frame_id.size>128u||value->frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->frame_id.data,p+*offset,value->frame_id.size);*offset+=value->frame_id.size;
  return roswasmc_bounded_2_std_msgs_msg_Header_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_std_msgs_msg_Header_encode(roswasmc_bounded_2_std_msgs_msg_Header const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_std_msgs_msg_Header_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_std_msgs_msg_Header_decode(void const* input,size_t length,roswasmc_bounded_2_std_msgs_msg_Header* value) {
  roswasmc_bounded_2_std_msgs_msg_Header next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>140u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2048 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2048];} roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_msg_sequence;
/** Complete ROS2 booster_interface/msg/RawBytesStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_RawBytesStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field msg; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_msg_sequence msg;
} roswasmc_bounded_2_booster_interface_msg_RawBytesStamped;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_RAWBYTESSTAMPED_MAX_WIRE_BYTES 2192u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_RAWBYTESSTAMPED_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_RAWBYTESSTAMPED_CODEC_IDENTITY "UWVMROS1_27cddd472b2d8c41c39effb87a8a53261aff309500dbb68df6ebd69a19c7720e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_valid(roswasmc_bounded_2_booster_interface_msg_RawBytesStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->msg.size>2048u)return 0;
  {size_t i;for(i=0;i<value->msg.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_wire_size(roswasmc_bounded_2_booster_interface_msg_RawBytesStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->msg.size>2048u)return 0;
  {size_t i;for(i=0;i<value->msg.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_write_fields(roswasmc_bounded_2_booster_interface_msg_RawBytesStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->msg.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->msg.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->msg.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_RawBytesStamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->msg.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->msg.size>2048u)return 0;
  {size_t i;for(i=0;i<value->msg.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->msg.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_encode(roswasmc_bounded_2_booster_interface_msg_RawBytesStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_RawBytesStamped* value) {
  roswasmc_bounded_2_booster_interface_msg_RawBytesStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>2192u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_RawBytesStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/msg/RemoteControllerState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_msg_RemoteControllerState {
  /** Declared field event; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t event;
  /** Declared field lx; canonical capacity is part of CODEC_IDENTITY. */
  float lx;
  /** Declared field ly; canonical capacity is part of CODEC_IDENTITY. */
  float ly;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field a; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t a;
  /** Declared field b; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t b;
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t y;
  /** Declared field lb; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t lb;
  /** Declared field rb; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rb;
  /** Declared field lt; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t lt;
  /** Declared field rt; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rt;
  /** Declared field ls; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t ls;
  /** Declared field rs; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rs;
  /** Declared field back; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t back;
  /** Declared field start; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t start;
  /** Declared field hat_c; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_c;
  /** Declared field hat_u; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_u;
  /** Declared field hat_d; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_d;
  /** Declared field hat_l; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_l;
  /** Declared field hat_r; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_r;
  /** Declared field hat_lu; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_lu;
  /** Declared field hat_ld; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_ld;
  /** Declared field hat_ru; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_ru;
  /** Declared field hat_rd; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_rd;
  /** Declared field hat_pos; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t hat_pos;
} roswasmc_bounded_2_booster_interface_msg_RemoteControllerState;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_REMOTECONTROLLERSTATE_MAX_WIRE_BYTES 42u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_REMOTECONTROLLERSTATE_MIN_WIRE_BYTES 42u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_MSG_REMOTECONTROLLERSTATE_CODEC_IDENTITY "UWVMROS1_f8a21705728862632c27b71d98c044a12bf1f9f9e0c1729fec27afe2c60c48a7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_valid(roswasmc_bounded_2_booster_interface_msg_RemoteControllerState const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ly,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->a>1)return 0;
  if(value->b>1)return 0;
  if(value->x>1)return 0;
  if(value->y>1)return 0;
  if(value->lb>1)return 0;
  if(value->rb>1)return 0;
  if(value->lt>1)return 0;
  if(value->rt>1)return 0;
  if(value->ls>1)return 0;
  if(value->rs>1)return 0;
  if(value->back>1)return 0;
  if(value->start>1)return 0;
  if(value->hat_c>1)return 0;
  if(value->hat_u>1)return 0;
  if(value->hat_d>1)return 0;
  if(value->hat_l>1)return 0;
  if(value->hat_r>1)return 0;
  if(value->hat_lu>1)return 0;
  if(value->hat_ld>1)return 0;
  if(value->hat_ru>1)return 0;
  if(value->hat_rd>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_wire_size(roswasmc_bounded_2_booster_interface_msg_RemoteControllerState const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_write_fields(roswasmc_bounded_2_booster_interface_msg_RemoteControllerState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->event,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ly,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->a,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->b,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->x,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->y,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->lb,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rb,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->lt,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rt,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->ls,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rs,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->back,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->start,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_c,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_u,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_d,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_l,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_r,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_lu,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_ld,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_ru,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_rd,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->hat_pos,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_msg_RemoteControllerState* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->event,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->lx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ly,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->a,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->b,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->x,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->y,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->lb,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rb,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->lt,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rt,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->ls,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rs,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->back,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->start,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_c,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_u,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_d,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_l,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_r,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_lu,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_ld,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_ru,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_rd,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->hat_pos,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_encode(roswasmc_bounded_2_booster_interface_msg_RemoteControllerState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_msg_RemoteControllerState* value) {
  roswasmc_bounded_2_booster_interface_msg_RemoteControllerState next={0};size_t offset=0;if(!value||(length&&!input)||length<42u||length>42u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_msg_RemoteControllerState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for booster_interface/srv/AgentService_Request.body; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_booster_interface_srv_AgentService_Request_body_text;
/** Complete ROS2 booster_interface/srv/AgentService_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_srv_AgentService_Request {
  /** Declared field body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_srv_AgentService_Request_body_text body;
} roswasmc_bounded_2_booster_interface_srv_AgentService_Request;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_AGENTSERVICE_REQUEST_MAX_WIRE_BYTES 4100u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_AGENTSERVICE_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_AGENTSERVICE_REQUEST_CODEC_IDENTITY "UWVMROS1_d8b972ee6c7f82ee948c03b22354d1073f120ad2616a9e833b578348775404e8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_srv_AgentService_Request_valid(roswasmc_bounded_2_booster_interface_srv_AgentService_Request const* value) {
  if(!value)return 0;
  if(value->body.size>4096u||!uwvm_bounded_utf8(value->body.data,value->body.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_srv_AgentService_Request_wire_size(roswasmc_bounded_2_booster_interface_srv_AgentService_Request const* value) {size_t n=0;
  n+=4+value->body.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_srv_AgentService_Request_write_fields(roswasmc_bounded_2_booster_interface_srv_AgentService_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->body.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->body.data,value->body.size);*offset+=value->body.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_srv_AgentService_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_srv_AgentService_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->body.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->body.size>4096u||value->body.size>n-*offset)return 0;uwvm_bounded_copy(value->body.data,p+*offset,value->body.size);*offset+=value->body.size;
  return roswasmc_bounded_2_booster_interface_srv_AgentService_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_AgentService_Request_encode(roswasmc_bounded_2_booster_interface_srv_AgentService_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_srv_AgentService_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_srv_AgentService_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_srv_AgentService_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_AgentService_Request_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_srv_AgentService_Request* value) {
  roswasmc_bounded_2_booster_interface_srv_AgentService_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4100u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_srv_AgentService_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for booster_interface/srv/AgentService_Response.body; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_booster_interface_srv_AgentService_Response_body_text;
/** Complete ROS2 booster_interface/srv/AgentService_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_srv_AgentService_Response {
  /** Declared field body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_srv_AgentService_Response_body_text body;
} roswasmc_bounded_2_booster_interface_srv_AgentService_Response;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_AGENTSERVICE_RESPONSE_MAX_WIRE_BYTES 4100u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_AGENTSERVICE_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_AGENTSERVICE_RESPONSE_CODEC_IDENTITY "UWVMROS1_86aecf9ee8652016b5c74e30d2dd4100e8b547542a81a98e7795942b47b87520"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_srv_AgentService_Response_valid(roswasmc_bounded_2_booster_interface_srv_AgentService_Response const* value) {
  if(!value)return 0;
  if(value->body.size>4096u||!uwvm_bounded_utf8(value->body.data,value->body.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_srv_AgentService_Response_wire_size(roswasmc_bounded_2_booster_interface_srv_AgentService_Response const* value) {size_t n=0;
  n+=4+value->body.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_srv_AgentService_Response_write_fields(roswasmc_bounded_2_booster_interface_srv_AgentService_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->body.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->body.data,value->body.size);*offset+=value->body.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_srv_AgentService_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_srv_AgentService_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->body.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->body.size>4096u||value->body.size>n-*offset)return 0;uwvm_bounded_copy(value->body.data,p+*offset,value->body.size);*offset+=value->body.size;
  return roswasmc_bounded_2_booster_interface_srv_AgentService_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_AgentService_Response_encode(roswasmc_bounded_2_booster_interface_srv_AgentService_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_srv_AgentService_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_srv_AgentService_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_srv_AgentService_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_AgentService_Response_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_srv_AgentService_Response* value) {
  roswasmc_bounded_2_booster_interface_srv_AgentService_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4100u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_srv_AgentService_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/srv/RpcService_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_srv_RpcService_Request {
  /** Declared field msg; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg msg;
} roswasmc_bounded_2_booster_interface_srv_RpcService_Request;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_RPCSERVICE_REQUEST_MAX_WIRE_BYTES 4108u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_RPCSERVICE_REQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_RPCSERVICE_REQUEST_CODEC_IDENTITY "UWVMROS1_cc8eb67d80bdf131f1878a1be998d68560e235ffef6a539876410dfcd6c00150"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_srv_RpcService_Request_valid(roswasmc_bounded_2_booster_interface_srv_RpcService_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_valid(&value->msg))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_srv_RpcService_Request_wire_size(roswasmc_bounded_2_booster_interface_srv_RpcService_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_wire_size(&value->msg);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_srv_RpcService_Request_write_fields(roswasmc_bounded_2_booster_interface_srv_RpcService_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_write_fields(&value->msg,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_srv_RpcService_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_srv_RpcService_Request* value) {
  if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiReqMsg_read_fields(p,n,offset,&value->msg))return 0;
  return roswasmc_bounded_2_booster_interface_srv_RpcService_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_RpcService_Request_encode(roswasmc_bounded_2_booster_interface_srv_RpcService_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_srv_RpcService_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_srv_RpcService_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_srv_RpcService_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_RpcService_Request_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_srv_RpcService_Request* value) {
  roswasmc_bounded_2_booster_interface_srv_RpcService_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>4108u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_srv_RpcService_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 booster_interface/srv/RpcService_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_booster_interface_srv_RpcService_Response {
  /** Declared field msg; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg msg;
} roswasmc_bounded_2_booster_interface_srv_RpcService_Response;
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_RPCSERVICE_RESPONSE_MAX_WIRE_BYTES 4108u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_RPCSERVICE_RESPONSE_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_BOOSTER_INTERFACE_SRV_RPCSERVICE_RESPONSE_CODEC_IDENTITY "UWVMROS1_7aeb7b45542f9f594e1f61be2f715abcde54b1e55efdde15100b0c553ae96322"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_booster_interface_srv_RpcService_Response_valid(roswasmc_bounded_2_booster_interface_srv_RpcService_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_valid(&value->msg))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_booster_interface_srv_RpcService_Response_wire_size(roswasmc_bounded_2_booster_interface_srv_RpcService_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_wire_size(&value->msg);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_booster_interface_srv_RpcService_Response_write_fields(roswasmc_bounded_2_booster_interface_srv_RpcService_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_write_fields(&value->msg,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_booster_interface_srv_RpcService_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_booster_interface_srv_RpcService_Response* value) {
  if(!roswasmc_bounded_2_booster_interface_msg_BoosterApiRespMsg_read_fields(p,n,offset,&value->msg))return 0;
  return roswasmc_bounded_2_booster_interface_srv_RpcService_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_RpcService_Response_encode(roswasmc_bounded_2_booster_interface_srv_RpcService_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_booster_interface_srv_RpcService_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_booster_interface_srv_RpcService_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_booster_interface_srv_RpcService_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_booster_interface_srv_RpcService_Response_decode(void const* input,size_t length,roswasmc_bounded_2_booster_interface_srv_RpcService_Response* value) {
  roswasmc_bounded_2_booster_interface_srv_RpcService_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>4108u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_booster_interface_srv_RpcService_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
