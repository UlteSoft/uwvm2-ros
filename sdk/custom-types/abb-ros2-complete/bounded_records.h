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
/** UTF-8 byte storage for abb_egm_msgs/msg/EGMChannelState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_name_text;
/** Complete ROS2 abb_egm_msgs/msg/EGMChannelState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_name_text name;
  /** Declared field active; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t active;
  /** Declared field egm_convergence_met; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t egm_convergence_met;
  /** Declared field egm_client_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t egm_client_state;
  /** Declared field motor_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t motor_state;
  /** Declared field rapid_execution_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rapid_execution_state;
  /** Declared field utilization_rate; canonical capacity is part of CODEC_IDENTITY. */
  double utilization_rate;
} roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState;
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_MAX_WIRE_BYTES 81u
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_CODEC_IDENTITY "UWVMROS1_fbd288b93963fccc92aa114570c0a4f6a1fdc5b473cb8e252922720f5d88b519"
/** Original scalar constant EGM_UNDEFINED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_EGM_UNDEFINED UINT32_C(1)
/** Original scalar constant EGM_ERROR=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_EGM_ERROR UINT32_C(2)
/** Original scalar constant EGM_STOPPED=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_EGM_STOPPED UINT32_C(3)
/** Original scalar constant EGM_RUNNING=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_EGM_RUNNING UINT32_C(4)
/** Original scalar constant MOTORS_UNDEFINED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_MOTORS_UNDEFINED UINT32_C(1)
/** Original scalar constant MOTORS_ON=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_MOTORS_ON UINT32_C(2)
/** Original scalar constant MOTORS_OFF=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_MOTORS_OFF UINT32_C(3)
/** Original scalar constant RAPID_UNDEFINED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_RAPID_UNDEFINED UINT32_C(1)
/** Original scalar constant RAPID_STOPPED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_RAPID_STOPPED UINT32_C(2)
/** Original scalar constant RAPID_RUNNING=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMCHANNELSTATE_RAPID_RUNNING UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_valid(roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  if(value->active>1)return 0;
  if(value->egm_convergence_met>1)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->utilization_rate,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_wire_size(roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_write_fields(roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->active,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->egm_convergence_met,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->egm_client_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->motor_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rapid_execution_state,1);*offset+=1;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->utilization_rate,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->active,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->egm_convergence_met,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->egm_client_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->motor_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rapid_execution_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->utilization_rate,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_encode(roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_decode(void const* input,size_t length,roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState* value) {
  roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>81u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_std_msgs_msg_Header_frame_id_text;
/** Complete ROS2 std_msgs/msg/Header value; no field projects away native data. */
typedef struct roswasmc_bounded_2_std_msgs_msg_Header {
  /** Declared field stamp; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Time stamp;
  /** Declared field frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header_frame_id_text frame_id;
} roswasmc_bounded_2_std_msgs_msg_Header;
#define ROSWASMC_BOUNDED_2_STD_MSGS_MSG_HEADER_MAX_WIRE_BYTES 76u
#define ROSWASMC_BOUNDED_2_STD_MSGS_MSG_HEADER_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_STD_MSGS_MSG_HEADER_CODEC_IDENTITY "UWVMROS1_afba9e74be3c5e3321750c1c4c30ba2925269f75b9da43a6d5c1e93d293d7396"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_std_msgs_msg_Header_valid(roswasmc_bounded_2_std_msgs_msg_Header const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(&value->stamp))return 0;
  if(value->frame_id.size>64u||!uwvm_bounded_utf8(value->frame_id.data,value->frame_id.size))return 0;
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
  if(value->frame_id.size>64u||value->frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->frame_id.data,p+*offset,value->frame_id.size);*offset+=value->frame_id.size;
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
  roswasmc_bounded_2_std_msgs_msg_Header next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>76u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState data[16];} roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_egm_channels_sequence;
/** Complete ROS2 abb_egm_msgs/msg/EGMState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_egm_msgs_msg_EGMState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field egm_channels; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_egm_channels_sequence egm_channels;
} roswasmc_bounded_2_abb_egm_msgs_msg_EGMState;
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMSTATE_MAX_WIRE_BYTES 1376u
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMSTATE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_EGM_MSGS_MSG_EGMSTATE_CODEC_IDENTITY "UWVMROS1_5b10efb13c3be7290c7e391d55679a174969887edd1b8baca682edac311a8e5d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_valid(roswasmc_bounded_2_abb_egm_msgs_msg_EGMState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->egm_channels.size>16u)return 0;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  if(!roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_valid(&value->egm_channels.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_wire_size(roswasmc_bounded_2_abb_egm_msgs_msg_EGMState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->egm_channels.size>16u)return 0;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  n+=roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_wire_size(&value->egm_channels.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_write_fields(roswasmc_bounded_2_abb_egm_msgs_msg_EGMState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->egm_channels.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_write_fields(&value->egm_channels.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_egm_msgs_msg_EGMState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->egm_channels.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->egm_channels.size>16u)return 0;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  if(!roswasmc_bounded_2_abb_egm_msgs_msg_EGMChannelState_read_fields(p,n,offset,&value->egm_channels.data[i]))return 0;
  }}
  return roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_encode(roswasmc_bounded_2_abb_egm_msgs_msg_EGMState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_decode(void const* input,size_t length,roswasmc_bounded_2_abb_egm_msgs_msg_EGMState* value) {
  roswasmc_bounded_2_abb_egm_msgs_msg_EGMState next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>1376u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_egm_msgs_msg_EGMState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/ConfData value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData {
  /** Declared field cf1; canonical capacity is part of CODEC_IDENTITY. */
  float cf1;
  /** Declared field cf4; canonical capacity is part of CODEC_IDENTITY. */
  float cf4;
  /** Declared field cf6; canonical capacity is part of CODEC_IDENTITY. */
  float cf6;
  /** Declared field cfx; canonical capacity is part of CODEC_IDENTITY. */
  float cfx;
} roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_CONFDATA_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_CONFDATA_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_CONFDATA_CODEC_IDENTITY "UWVMROS1_0c22cc29db62f5001ef20ea7b131de63099e31ac5f150d3e9f0bb62120d60571"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cfx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cfx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cf1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cf4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cf6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cfx,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/ExtJoint value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint {
  /** Declared field eax_a; canonical capacity is part of CODEC_IDENTITY. */
  float eax_a;
  /** Declared field eax_b; canonical capacity is part of CODEC_IDENTITY. */
  float eax_b;
  /** Declared field eax_c; canonical capacity is part of CODEC_IDENTITY. */
  float eax_c;
  /** Declared field eax_d; canonical capacity is part of CODEC_IDENTITY. */
  float eax_d;
  /** Declared field eax_e; canonical capacity is part of CODEC_IDENTITY. */
  float eax_e;
  /** Declared field eax_f; canonical capacity is part of CODEC_IDENTITY. */
  float eax_f;
} roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_EXTJOINT_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_EXTJOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_EXTJOINT_CODEC_IDENTITY "UWVMROS1_8b799c6bec9e4eab0900a633a93375a7792e49fde0bf302f6752115a001a4ae2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_a,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_b,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_c,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_d,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_e,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_f,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_a,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_b,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_c,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_d,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_e,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_f,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_a,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_b,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_c,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_d,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_e,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_f,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/RobJoint value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint {
  /** Declared field rax_1; canonical capacity is part of CODEC_IDENTITY. */
  float rax_1;
  /** Declared field rax_2; canonical capacity is part of CODEC_IDENTITY. */
  float rax_2;
  /** Declared field rax_3; canonical capacity is part of CODEC_IDENTITY. */
  float rax_3;
  /** Declared field rax_4; canonical capacity is part of CODEC_IDENTITY. */
  float rax_4;
  /** Declared field rax_5; canonical capacity is part of CODEC_IDENTITY. */
  float rax_5;
  /** Declared field rax_6; canonical capacity is part of CODEC_IDENTITY. */
  float rax_6;
} roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ROBJOINT_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ROBJOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ROBJOINT_CODEC_IDENTITY "UWVMROS1_6d842aebc0a3b222a727063a41ef7e96f2b8c14ffbba5c8d6e140ba2890e69cd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/JointTarget value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget {
  /** Declared field robax; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint robax;
  /** Declared field extax; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint extax;
} roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_JOINTTARGET_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_JOINTTARGET_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_JOINTTARGET_CODEC_IDENTITY "UWVMROS1_5949cf0121391646c1f04b54d8f7deaae0497f4920f7817739fc25bfddc4e540"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_valid(&value->robax))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_valid(&value->extax))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_wire_size(&value->robax);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_wire_size(&value->extax);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_write_fields(&value->robax,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_write_fields(&value->extax,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget* value) {
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_RobJoint_read_fields(p,n,offset,&value->robax))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_read_fields(p,n,offset,&value->extax))return 0;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_JointTarget_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/Pos value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_Pos {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
} roswasmc_bounded_2_abb_rapid_msgs_msg_Pos;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_POS_MAX_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_POS_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_POS_CODEC_IDENTITY "UWVMROS1_31d490d6ead5a1a78295dc4e934f0b89e74e4732da2b57d28d297477bf4cbdd2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_Pos const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_Pos const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_Pos const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_Pos* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_Pos const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_Pos* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>12u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/Orient value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_Orient {
  /** Declared field q1; canonical capacity is part of CODEC_IDENTITY. */
  float q1;
  /** Declared field q2; canonical capacity is part of CODEC_IDENTITY. */
  float q2;
  /** Declared field q3; canonical capacity is part of CODEC_IDENTITY. */
  float q3;
  /** Declared field q4; canonical capacity is part of CODEC_IDENTITY. */
  float q4;
} roswasmc_bounded_2_abb_rapid_msgs_msg_Orient;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ORIENT_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ORIENT_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ORIENT_CODEC_IDENTITY "UWVMROS1_821d887b88298422f9466aaf3926db580f878e964055fbd72226b7f76f2f536e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_Orient const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_Orient const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_Orient const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_Orient* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q4,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_Orient const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_Orient* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/LoadData value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field cog; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos cog;
  /** Declared field aom; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient aom;
  /** Declared field ix; canonical capacity is part of CODEC_IDENTITY. */
  float ix;
  /** Declared field iy; canonical capacity is part of CODEC_IDENTITY. */
  float iy;
  /** Declared field iz; canonical capacity is part of CODEC_IDENTITY. */
  float iz;
} roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_LOADDATA_MAX_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_LOADDATA_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_LOADDATA_CODEC_IDENTITY "UWVMROS1_f73e480a0e6746a4df8f4d62bc123d68d01d3dc48f0bc5f0eac51b859a0750f9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_valid(&value->cog))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_valid(&value->aom))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ix,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iy,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData const* value) {size_t n=0;
  n+=4;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_wire_size(&value->cog);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_wire_size(&value->aom);
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_write_fields(&value->cog,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_write_fields(&value->aom,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ix,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iy,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_read_fields(p,n,offset,&value->cog))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_read_fields(p,n,offset,&value->aom))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ix,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->iy,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->iz,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>44u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/Pose value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_Pose {
  /** Declared field trans; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos trans;
  /** Declared field rot; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient rot;
} roswasmc_bounded_2_abb_rapid_msgs_msg_Pose;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_POSE_MAX_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_POSE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_POSE_CODEC_IDENTITY "UWVMROS1_8f1535fe21326706fd0b16314857162d36bca91ce8e8f6a8bec3864dafb9e2f2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_Pose const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_valid(&value->trans))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_valid(&value->rot))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_Pose const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_wire_size(&value->trans);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_wire_size(&value->rot);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_Pose const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_write_fields(&value->trans,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_write_fields(&value->rot,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_Pose* value) {
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_read_fields(p,n,offset,&value->trans))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_read_fields(p,n,offset,&value->rot))return 0;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_Pose const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_Pose* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>28u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/RobTarget value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget {
  /** Declared field trans; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos trans;
  /** Declared field rot; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient rot;
  /** Declared field robconf; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData robconf;
  /** Declared field extax; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint extax;
} roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ROBTARGET_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ROBTARGET_MIN_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_ROBTARGET_CODEC_IDENTITY "UWVMROS1_8decb56e8faf0a6a954af974adfea5c500aea8392e6a778ae7394174323489d8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_valid(&value->trans))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_valid(&value->rot))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_valid(&value->robconf))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_valid(&value->extax))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_wire_size(&value->trans);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_wire_size(&value->rot);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_wire_size(&value->robconf);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_wire_size(&value->extax);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_write_fields(&value->trans,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_write_fields(&value->rot,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_write_fields(&value->robconf,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_write_fields(&value->extax,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget* value) {
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pos_read_fields(p,n,offset,&value->trans))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Orient_read_fields(p,n,offset,&value->rot))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ConfData_read_fields(p,n,offset,&value->robconf))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ExtJoint_read_fields(p,n,offset,&value->extax))return 0;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget next={0};size_t offset=0;if(!value||(length&&!input)||length<68u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_RobTarget_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_msgs/msg/ToolData value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData {
  /** Declared field robhold; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t robhold;
  /** Declared field tframe; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose tframe;
  /** Declared field tload; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData tload;
} roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_TOOLDATA_MAX_WIRE_BYTES 73u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_TOOLDATA_MIN_WIRE_BYTES 73u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_TOOLDATA_CODEC_IDENTITY "UWVMROS1_948fa0bbaaf95f0d88ca03cad687f16102261f54b0da889ae8bca82042fc5237"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData const* value) {
  if(!value)return 0;
  if(value->robhold>1)return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(&value->tframe))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_valid(&value->tload))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData const* value) {size_t n=0;
  n+=1;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(&value->tframe);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_wire_size(&value->tload);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->robhold,1);*offset+=1;
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(&value->tframe,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_write_fields(&value->tload,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->robhold,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields(p,n,offset,&value->tframe))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_LoadData_read_fields(p,n,offset,&value->tload))return 0;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData next={0};size_t offset=0;if(!value||(length&&!input)||length<73u||length>73u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_msgs/msg/WObjData.ufmec; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_ufmec_text;
/** Complete ROS2 abb_rapid_msgs/msg/WObjData value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData {
  /** Declared field robhold; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t robhold;
  /** Declared field ufprog; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t ufprog;
  /** Declared field ufmec; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_ufmec_text ufmec;
  /** Declared field uframe; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose uframe;
  /** Declared field oframe; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose oframe;
} roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_WOBJDATA_MAX_WIRE_BYTES 574u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_WOBJDATA_MIN_WIRE_BYTES 62u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_MSGS_MSG_WOBJDATA_CODEC_IDENTITY "UWVMROS1_b36f0448512fab6028431d19b39a8ae31b7b81b2615ffc8311139f63b5d8e7cd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_valid(roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData const* value) {
  if(!value)return 0;
  if(value->robhold>1)return 0;
  if(value->ufprog>1)return 0;
  if(value->ufmec.size>512u||!uwvm_bounded_utf8(value->ufmec.data,value->ufmec.size))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(&value->uframe))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(&value->oframe))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_wire_size(roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=4+value->ufmec.size;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(&value->uframe);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(&value->oframe);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_write_fields(roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->robhold,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->ufprog,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->ufmec.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->ufmec.data,value->ufmec.size);*offset+=value->ufmec.size;
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(&value->uframe,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(&value->oframe,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->robhold,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->ufprog,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->ufmec.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->ufmec.size>512u||value->ufmec.size>n-*offset)return 0;uwvm_bounded_copy(value->ufmec.data,p+*offset,value->ufmec.size);*offset+=value->ufmec.size;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields(p,n,offset,&value->uframe))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields(p,n,offset,&value->oframe))return 0;
  return roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_encode(roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData* value) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData next={0};size_t offset=0;if(!value||(length&&!input)||length<62u||length>574u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMActivateSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings {
  /** Declared field tool; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData tool;
  /** Declared field wobj; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData wobj;
  /** Declared field correction_frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose correction_frame;
  /** Declared field sensor_frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose sensor_frame;
  /** Declared field cond_min_max; canonical capacity is part of CODEC_IDENTITY. */
  float cond_min_max;
  /** Declared field lp_filter; canonical capacity is part of CODEC_IDENTITY. */
  float lp_filter;
  /** Declared field sample_rate; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t sample_rate;
  /** Declared field max_speed_deviation; canonical capacity is part of CODEC_IDENTITY. */
  float max_speed_deviation;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMACTIVATESETTINGS_MAX_WIRE_BYTES 716u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMACTIVATESETTINGS_MIN_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMACTIVATESETTINGS_CODEC_IDENTITY "UWVMROS1_fce73aea80d9b8a299b8e2ab742602871146b3ebbd624a2e807e25c1ac107cfa"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_valid(&value->tool))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_valid(&value->wobj))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(&value->correction_frame))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(&value->sensor_frame))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_min_max,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lp_filter,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->max_speed_deviation,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_wire_size(&value->tool);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_wire_size(&value->wobj);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(&value->correction_frame);
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(&value->sensor_frame);
  n+=4;
  n+=4;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_write_fields(&value->tool,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_write_fields(&value->wobj,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(&value->correction_frame,p,offset);
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(&value->sensor_frame,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_min_max,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lp_filter,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->sample_rate,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->max_speed_deviation,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings* value) {
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_ToolData_read_fields(p,n,offset,&value->tool))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_WObjData_read_fields(p,n,offset,&value->wobj))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields(p,n,offset,&value->correction_frame))return 0;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields(p,n,offset,&value->sensor_frame))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cond_min_max,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->lp_filter,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->sample_rate,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->max_speed_deviation,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<204u||length>716u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMRunSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings {
  /** Declared field cond_time; canonical capacity is part of CODEC_IDENTITY. */
  float cond_time;
  /** Declared field ramp_in_time; canonical capacity is part of CODEC_IDENTITY. */
  float ramp_in_time;
  /** Declared field offset; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose offset;
  /** Declared field pos_corr_gain; canonical capacity is part of CODEC_IDENTITY. */
  float pos_corr_gain;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMRUNSETTINGS_MAX_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMRUNSETTINGS_MIN_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMRUNSETTINGS_CODEC_IDENTITY "UWVMROS1_92a2a654eb2eda89077933f955f04ed87884827b8875cf563570eed5d211b0e2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_in_time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_valid(&value->offset))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pos_corr_gain,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_wire_size(&value->offset);
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_in_time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_write_fields(&value->offset,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pos_corr_gain,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cond_time,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ramp_in_time,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_abb_rapid_msgs_msg_Pose_read_fields(p,n,offset,&value->offset))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pos_corr_gain,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<40u||length>40u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMSetupSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings {
  /** Declared field use_filtering; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_filtering;
  /** Declared field comm_timeout; canonical capacity is part of CODEC_IDENTITY. */
  float comm_timeout;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSETUPSETTINGS_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSETUPSETTINGS_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSETUPSETTINGS_CODEC_IDENTITY "UWVMROS1_2565bed5b83b132c69f37d7294c69181800ad2974ddc54f60868da57e28e782c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings const* value) {
  if(!value)return 0;
  if(value->use_filtering>1)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->comm_timeout,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings const* value) {size_t n=0;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_filtering,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->comm_timeout,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_filtering,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->comm_timeout,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMStopSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings {
  /** Declared field ramp_out_time; canonical capacity is part of CODEC_IDENTITY. */
  float ramp_out_time;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSTOPSETTINGS_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSTOPSETTINGS_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSTOPSETTINGS_CODEC_IDENTITY "UWVMROS1_0f3f2f6034567a7d49dd140ff9ff620e8567548139597b9b823e63a947673cee"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_out_time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_out_time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ramp_out_time,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/EGMSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings {
  /** Declared field allow_egm_motions; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t allow_egm_motions;
  /** Declared field use_presync; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_presync;
  /** Declared field setup_uc; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings setup_uc;
  /** Declared field activate; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings activate;
  /** Declared field run; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings run;
  /** Declared field stop; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings stop;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSETTINGS_MAX_WIRE_BYTES 767u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSETTINGS_MIN_WIRE_BYTES 255u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_EGMSETTINGS_CODEC_IDENTITY "UWVMROS1_622d3a975eecb11f7729aecfadc161cada271208d6fe7599f455750b90144ef3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings const* value) {
  if(!value)return 0;
  if(value->allow_egm_motions>1)return 0;
  if(value->use_presync>1)return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_valid(&value->setup_uc))return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_valid(&value->activate))return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_valid(&value->run))return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_valid(&value->stop))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_wire_size(&value->setup_uc);
  n+=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_wire_size(&value->activate);
  n+=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_wire_size(&value->run);
  n+=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_wire_size(&value->stop);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->allow_egm_motions,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_presync,1);*offset+=1;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_write_fields(&value->setup_uc,p,offset);
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_write_fields(&value->activate,p,offset);
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_write_fields(&value->run,p,offset);
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_write_fields(&value->stop,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->allow_egm_motions,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_presync,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSetupSettings_read_fields(p,n,offset,&value->setup_uc))return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMActivateSettings_read_fields(p,n,offset,&value->activate))return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMRunSettings_read_fields(p,n,offset,&value->run))return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMStopSettings_read_fields(p,n,offset,&value->stop))return 0;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<255u||length>767u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/msg/StateMachineState.rapid_task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_rapid_task_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/StateMachineState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState {
  /** Declared field rapid_task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_rapid_task_text rapid_task;
  /** Declared field sm_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t sm_state;
  /** Declared field egm_action; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t egm_action;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_CODEC_IDENTITY "UWVMROS1_ff2a09e4b6e8ef26cbdc3ecdffdceb33d6b2c82f0d51ed2c38ce9648c6f47711"
/** Original scalar constant SM_STATE_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_SM_STATE_UNKNOWN UINT32_C(1)
/** Original scalar constant SM_STATE_IDLE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_SM_STATE_IDLE UINT32_C(2)
/** Original scalar constant SM_STATE_INITIALIZE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_SM_STATE_INITIALIZE UINT32_C(3)
/** Original scalar constant SM_STATE_RUN_RAPID_ROUTINE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_SM_STATE_RUN_RAPID_ROUTINE UINT32_C(4)
/** Original scalar constant SM_STATE_RUN_EGM_ROUTINE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_SM_STATE_RUN_EGM_ROUTINE UINT32_C(5)
/** Original scalar constant EGM_ACTION_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_EGM_ACTION_UNKNOWN UINT32_C(1)
/** Original scalar constant EGM_ACTION_NONE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_EGM_ACTION_NONE UINT32_C(2)
/** Original scalar constant EGM_ACTION_RUN_JOINT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_EGM_ACTION_RUN_JOINT UINT32_C(3)
/** Original scalar constant EGM_ACTION_RUN_POSE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_EGM_ACTION_RUN_POSE UINT32_C(4)
/** Original scalar constant EGM_ACTION_STOP=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_EGM_ACTION_STOP UINT32_C(5)
/** Original scalar constant EGM_ACTION_START_STREAM=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_EGM_ACTION_START_STREAM UINT32_C(6)
/** Original scalar constant EGM_ACTION_STOP_STREAM=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_STATEMACHINESTATE_EGM_ACTION_STOP_STREAM UINT32_C(7)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState const* value) {
  if(!value)return 0;
  if(value->rapid_task.size>512u||!uwvm_bounded_utf8(value->rapid_task.data,value->rapid_task.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState const* value) {size_t n=0;
  n+=4+value->rapid_task.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->rapid_task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->rapid_task.data,value->rapid_task.size);*offset+=value->rapid_task.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sm_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->egm_action,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState* value) {
  if(*offset>n||n-*offset<4)return 0;value->rapid_task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->rapid_task.size>512u||value->rapid_task.size>n-*offset)return 0;uwvm_bounded_copy(value->rapid_task.data,p+*offset,value->rapid_task.size);*offset+=value->rapid_task.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->sm_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->egm_action,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState data[16];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_state_machines_sequence;
/** Complete ROS2 abb_rapid_sm_addin_msgs/msg/RuntimeState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field state_machines; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_state_machines_sequence state_machines;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_RUNTIMESTATE_MAX_WIRE_BYTES 8368u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_RUNTIMESTATE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_MSG_RUNTIMESTATE_CODEC_IDENTITY "UWVMROS1_3c0f157544fa82c4e206a4fb42f8ba7f67213d19533f79b77c66dec6fd0460d9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->state_machines.size>16u)return 0;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_valid(&value->state_machines.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->state_machines.size>16u)return 0;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  n+=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_wire_size(&value->state_machines.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->state_machines.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_write_fields(&value->state_machines.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->state_machines.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state_machines.size>16u)return 0;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_StateMachineState_read_fields(p,n,offset,&value->state_machines.data[i]))return 0;
  }}
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>8368u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_RuntimeState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_task_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_task_text task;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_GETEGMSETTINGS_REQUEST_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_GETEGMSETTINGS_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_GETEGMSETTINGS_REQUEST_CODEC_IDENTITY "UWVMROS1_363be2459fcc9c475c6305cbcfebf7689e891eb27645b7667960aace23d56e8e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request const* value) {size_t n=0;
  n+=4+value->task.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_message_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/GetEGMSettings_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response {
  /** Declared field settings; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings settings;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_message_text message;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_GETEGMSETTINGS_RESPONSE_MAX_WIRE_BYTES 1285u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_GETEGMSETTINGS_RESPONSE_MIN_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_GETEGMSETTINGS_RESPONSE_CODEC_IDENTITY "UWVMROS1_d8f340e1c6d78fc0a782c3279efb34ae5e11df175a82f9dd5ee82e603d6b2ffb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_valid(&value->settings))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_wire_size(&value->settings);
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_write_fields(&value->settings,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response* value) {
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_read_fields(p,n,offset,&value->settings))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<261u||length>1285u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_GetEGMSettings_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_task_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_task_text task;
  /** Declared field settings; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings settings;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETEGMSETTINGS_REQUEST_MAX_WIRE_BYTES 835u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETEGMSETTINGS_REQUEST_MIN_WIRE_BYTES 259u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETEGMSETTINGS_REQUEST_CODEC_IDENTITY "UWVMROS1_c2f3dd535e9475526bf18bdffa0e9a839049d315c9aebb150f8561ee3361a2bd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_valid(&value->settings))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_wire_size(&value->settings);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_write_fields(&value->settings,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_msg_EGMSettings_read_fields(p,n,offset,&value->settings))return 0;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<259u||length>835u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_message_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetEGMSettings_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_message_text message;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETEGMSETTINGS_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETEGMSETTINGS_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETEGMSETTINGS_RESPONSE_CODEC_IDENTITY "UWVMROS1_6a1adeebe31dfe101a6d57019e26ee2f1144b9a6ce30aee08d4c6fe56ea6838d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetEGMSettings_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_task_text;
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request.routine; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_routine_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_task_text task;
  /** Declared field routine; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_routine_text routine;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETRAPIDROUTINE_REQUEST_MAX_WIRE_BYTES 584u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETRAPIDROUTINE_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETRAPIDROUTINE_REQUEST_CODEC_IDENTITY "UWVMROS1_4561b7e5e296a1a329d5f5684bcbde1ef41324554a34e9a79a85824286dd477c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  if(value->routine.size>512u||!uwvm_bounded_utf8(value->routine.data,value->routine.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=4+value->routine.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  uwvm_bounded_put(p+*offset,value->routine.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->routine.data,value->routine.size);*offset+=value->routine.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(*offset>n||n-*offset<4)return 0;value->routine.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->routine.size>512u||value->routine.size>n-*offset)return 0;uwvm_bounded_copy(value->routine.data,p+*offset,value->routine.size);*offset+=value->routine.size;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>584u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_message_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetRAPIDRoutine_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_message_text message;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETRAPIDROUTINE_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETRAPIDROUTINE_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETRAPIDROUTINE_RESPONSE_CODEC_IDENTITY "UWVMROS1_980d2a16536289f446161acdf2fc57d459baa2290d74f1dd5aaf8129126cbba1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetRAPIDRoutine_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_task_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetSGCommand_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_task_text task;
  /** Declared field command; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t command;
  /** Declared field target_position; canonical capacity is part of CODEC_IDENTITY. */
  float target_position;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_MAX_WIRE_BYTES 73u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_CODEC_IDENTITY "UWVMROS1_5d6983aa85c2c52f8ec06cbd723d5929cfc381db2e0bfffb4c27078497d86c90"
/** Original scalar constant SG_COMMAND_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_UNKNOWN UINT32_C(1)
/** Original scalar constant SG_COMMAND_NONE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_NONE UINT32_C(2)
/** Original scalar constant SG_COMMAND_INITIALIZE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_INITIALIZE UINT32_C(3)
/** Original scalar constant SG_COMMAND_CALIBRATE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_CALIBRATE UINT32_C(4)
/** Original scalar constant SG_COMMAND_MOVE_TO=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_MOVE_TO UINT32_C(5)
/** Original scalar constant SG_COMMAND_GRIP_IN=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_GRIP_IN UINT32_C(6)
/** Original scalar constant SG_COMMAND_GRIP_OUT=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_GRIP_OUT UINT32_C(7)
/** Original scalar constant SG_COMMAND_BLOW_ON_1=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_BLOW_ON_1 UINT32_C(8)
/** Original scalar constant SG_COMMAND_BLOW_ON_2=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_BLOW_ON_2 UINT32_C(9)
/** Original scalar constant SG_COMMAND_BLOW_OFF_1=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_BLOW_OFF_1 UINT32_C(10)
/** Original scalar constant SG_COMMAND_BLOW_OFF_2=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_BLOW_OFF_2 UINT32_C(11)
/** Original scalar constant SG_COMMAND_VACUUM_ON_1=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_VACUUM_ON_1 UINT32_C(12)
/** Original scalar constant SG_COMMAND_VACUUM_ON_2=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_VACUUM_ON_2 UINT32_C(13)
/** Original scalar constant SG_COMMAND_VACUUM_OFF_1=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_VACUUM_OFF_1 UINT32_C(14)
/** Original scalar constant SG_COMMAND_VACUUM_OFF_2=15; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_REQUEST_SG_COMMAND_VACUUM_OFF_2 UINT32_C(15)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->target_position,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->command,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->target_position,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->command,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->target_position,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>73u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_message_text;
/** Complete ROS2 abb_rapid_sm_addin_msgs/srv/SetSGCommand_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_message_text message;
} roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response;
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_RAPID_SM_ADDIN_MSGS_SRV_SETSGCOMMAND_RESPONSE_CODEC_IDENTITY "UWVMROS1_87cdc41c81d49930dce7193d4b7af98add266a6e9a7cf5e69457d01ded388cf8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_valid(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_wire_size(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_write_fields(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_encode(roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response* value) {
  roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_rapid_sm_addin_msgs_srv_SetSGCommand_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/msg/MechanicalUnitState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_name_text;
/** Complete ROS2 abb_robot_msgs/msg/MechanicalUnitState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_name_text name;
  /** Declared field activated; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t activated;
} roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_MECHANICALUNITSTATE_MAX_WIRE_BYTES 69u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_MECHANICALUNITSTATE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_MECHANICALUNITSTATE_CODEC_IDENTITY "UWVMROS1_1dc7a7dcbaa19e78cd6419f8844287a7fbd9d7a839ab1a8ad97ef0be34f3a3a8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_valid(roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  if(value->activated>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_wire_size(roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_write_fields(roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->activated,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->activated,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_encode(roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState* value) {
  roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>69u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/msg/RAPIDSymbolPath.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_task_text;
/** UTF-8 byte storage for abb_robot_msgs/msg/RAPIDSymbolPath.module; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_module_text;
/** UTF-8 byte storage for abb_robot_msgs/msg/RAPIDSymbolPath.symbol; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_symbol_text;
/** Complete ROS2 abb_robot_msgs/msg/RAPIDSymbolPath value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_task_text task;
  /** Declared field module; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_module_text module;
  /** Declared field symbol; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_symbol_text symbol;
} roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDSYMBOLPATH_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDSYMBOLPATH_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDSYMBOLPATH_CODEC_IDENTITY "UWVMROS1_249523197567fbf638f30487a7f5b8d8ff5ea746fcd4a65a35f2dbb18a3adf0e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  if(value->module.size>64u||!uwvm_bounded_utf8(value->module.data,value->module.size))return 0;
  if(value->symbol.size>64u||!uwvm_bounded_utf8(value->symbol.data,value->symbol.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=4+value->module.size;
  n+=4+value->symbol.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  uwvm_bounded_put(p+*offset,value->module.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->module.data,value->module.size);*offset+=value->module.size;
  uwvm_bounded_put(p+*offset,value->symbol.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->symbol.data,value->symbol.size);*offset+=value->symbol.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(*offset>n||n-*offset<4)return 0;value->module.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->module.size>64u||value->module.size>n-*offset)return 0;uwvm_bounded_copy(value->module.data,p+*offset,value->module.size);*offset+=value->module.size;
  if(*offset>n||n-*offset<4)return 0;value->symbol.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->symbol.size>64u||value->symbol.size>n-*offset)return 0;uwvm_bounded_copy(value->symbol.data,p+*offset,value->symbol.size);*offset+=value->symbol.size;
  return roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_encode(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath* value) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/msg/RAPIDTaskState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_name_text;
/** Complete ROS2 abb_robot_msgs/msg/RAPIDTaskState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_name_text name;
  /** Declared field activated; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t activated;
  /** Declared field execution_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t execution_state;
  /** Declared field motion_task; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t motion_task;
} roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_MAX_WIRE_BYTES 71u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_CODEC_IDENTITY "UWVMROS1_ff698e375b0ff33026cfd248c1ac734f939763aba25a6c4f7cac628d445422fd"
/** Original scalar constant EXECUTION_STATE_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_EXECUTION_STATE_UNKNOWN UINT32_C(1)
/** Original scalar constant EXECUTION_STATE_READY=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_EXECUTION_STATE_READY UINT32_C(2)
/** Original scalar constant EXECUTION_STATE_STOPPED=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_EXECUTION_STATE_STOPPED UINT32_C(3)
/** Original scalar constant EXECUTION_STATE_STARTED=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_EXECUTION_STATE_STARTED UINT32_C(4)
/** Original scalar constant EXECUTION_STATE_UNINITIALIZED=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_RAPIDTASKSTATE_EXECUTION_STATE_UNINITIALIZED UINT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_valid(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  if(value->activated>1)return 0;
  if(value->motion_task>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_wire_size(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_write_fields(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->activated,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->execution_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->motion_task,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->activated,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->execution_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->motion_task,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_encode(roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState* value) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>71u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/msg/ServiceResponses value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_CODEC_IDENTITY "UWVMROS1_4e01b972b67454e851801f28aee09c0774b67907d2241cc67cae7d045c18f646"
/** Original scalar constant SUCCESS=""; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_SUCCESS ""
/** Original scalar constant RC_SUCCESS=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_SUCCESS UINT32_C(1)
/** Original scalar constant FAILED="Service failed (e.g. timeout or resource not found, see the DEBUG log for details)"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_FAILED "\123\145\162\166\151\143\145\040\146\141\151\154\145\144\040\050\145\056\147\056\040\164\151\155\145\157\165\164\040\157\162\040\162\145\163\157\165\162\143\145\040\156\157\164\040\146\157\165\156\144\054\040\163\145\145\040\164\150\145\040\104\105\102\125\107\040\154\157\147\040\146\157\162\040\144\145\164\141\151\154\163\051"
/** Original scalar constant RC_FAILED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_FAILED UINT32_C(2)
/** Original scalar constant SERVER_IS_BUSY="Server is busy"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_SERVER_IS_BUSY "\123\145\162\166\145\162\040\151\163\040\142\165\163\171"
/** Original scalar constant RC_SERVER_IS_BUSY=1001; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_SERVER_IS_BUSY UINT32_C(1001)
/** Original scalar constant EMPTY_FILENAME="Filename is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_EMPTY_FILENAME "\106\151\154\145\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_FILENAME=2001; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_EMPTY_FILENAME UINT32_C(2001)
/** Original scalar constant EMPTY_SIGNAL_NAME="Signal name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_EMPTY_SIGNAL_NAME "\123\151\147\156\141\154\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_SIGNAL_NAME=2002; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_EMPTY_SIGNAL_NAME UINT32_C(2002)
/** Original scalar constant EMPTY_RAPID_TASK_NAME="RAPID task name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_EMPTY_RAPID_TASK_NAME "\122\101\120\111\104\040\164\141\163\153\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_RAPID_TASK_NAME=2003; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_EMPTY_RAPID_TASK_NAME UINT32_C(2003)
/** Original scalar constant EMPTY_RAPID_MODULE_NAME="RAPID module name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_EMPTY_RAPID_MODULE_NAME "\122\101\120\111\104\040\155\157\144\165\154\145\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_RAPID_MODULE_NAME=2004; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_EMPTY_RAPID_MODULE_NAME UINT32_C(2004)
/** Original scalar constant EMPTY_RAPID_SYMBOL_NAME="RAPID symbol name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_EMPTY_RAPID_SYMBOL_NAME "\122\101\120\111\104\040\163\171\155\142\157\154\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_RAPID_SYMBOL_NAME=2005; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_EMPTY_RAPID_SYMBOL_NAME UINT32_C(2005)
/** Original scalar constant NOT_IN_AUTO_MODE="Robot controller is not in AUTO mode"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_NOT_IN_AUTO_MODE "\122\157\142\157\164\040\143\157\156\164\162\157\154\154\145\162\040\151\163\040\156\157\164\040\151\156\040\101\125\124\117\040\155\157\144\145"
/** Original scalar constant RC_NOT_IN_AUTO_MODE=3001; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_NOT_IN_AUTO_MODE UINT32_C(3001)
/** Original scalar constant MOTORS_ARE_OFF="Motors are off"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_MOTORS_ARE_OFF "\115\157\164\157\162\163\040\141\162\145\040\157\146\146"
/** Original scalar constant RC_MOTORS_ARE_OFF=3002; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_MOTORS_ARE_OFF UINT32_C(3002)
/** Original scalar constant MOTORS_ARE_ON="Motors are on"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_MOTORS_ARE_ON "\115\157\164\157\162\163\040\141\162\145\040\157\156"
/** Original scalar constant RC_MOTORS_ARE_ON=3003; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_MOTORS_ARE_ON UINT32_C(3003)
/** Original scalar constant RAPID_NOT_STOPPED="RAPID has not been stopped"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RAPID_NOT_STOPPED "\122\101\120\111\104\040\150\141\163\040\156\157\164\040\142\145\145\156\040\163\164\157\160\160\145\144"
/** Original scalar constant RC_RAPID_NOT_STOPPED=3004; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_RAPID_NOT_STOPPED UINT32_C(3004)
/** Original scalar constant RAPID_NOT_RUNNING="RAPID is not running"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RAPID_NOT_RUNNING "\122\101\120\111\104\040\151\163\040\156\157\164\040\162\165\156\156\151\156\147"
/** Original scalar constant RC_RAPID_NOT_RUNNING=3005; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_RAPID_NOT_RUNNING UINT32_C(3005)
/** Original scalar constant SM_RUNTIME_STATES_MISSING="No runtime states received for any StateMachine Add-In instance"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_SM_RUNTIME_STATES_MISSING "\116\157\040\162\165\156\164\151\155\145\040\163\164\141\164\145\163\040\162\145\143\145\151\166\145\144\040\146\157\162\040\141\156\171\040\123\164\141\164\145\115\141\143\150\151\156\145\040\101\144\144\055\111\156\040\151\156\163\164\141\156\143\145"
/** Original scalar constant RC_SM_RUNTIME_STATES_MISSING=4001; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_SM_RUNTIME_STATES_MISSING UINT32_C(4001)
/** Original scalar constant SM_UNKNOWN_RAPID_TASK="RAPID task not found among known StateMachine Add-In instances"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_SM_UNKNOWN_RAPID_TASK "\122\101\120\111\104\040\164\141\163\153\040\156\157\164\040\146\157\165\156\144\040\141\155\157\156\147\040\153\156\157\167\156\040\123\164\141\164\145\115\141\143\150\151\156\145\040\101\144\144\055\111\156\040\151\156\163\164\141\156\143\145\163"
/** Original scalar constant RC_SM_UNKNOWN_RAPID_TASK=4002; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_SM_UNKNOWN_RAPID_TASK UINT32_C(4002)
/** Original scalar constant SM_UNINITIALIZED="StateMachine Add-In instance has not been initialized"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_SM_UNINITIALIZED "\123\164\141\164\145\115\141\143\150\151\156\145\040\101\144\144\055\111\156\040\151\156\163\164\141\156\143\145\040\150\141\163\040\156\157\164\040\142\145\145\156\040\151\156\151\164\151\141\154\151\172\145\144"
/** Original scalar constant RC_SM_UNINITIALIZED=4003; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SERVICERESPONSES_RC_SM_UNINITIALIZED UINT32_C(4003)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_valid(roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_wire_size(roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_write_fields(roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses* value) {
  return roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_encode(roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses* value) {
  roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_ServiceResponses_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState data[16];} roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_rapid_tasks_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState data[16];} roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_mechanical_units_sequence;
/** Complete ROS2 abb_robot_msgs/msg/SystemState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_msg_SystemState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field motors_on; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t motors_on;
  /** Declared field auto_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t auto_mode;
  /** Declared field rapid_running; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rapid_running;
  /** Declared field rapid_tasks; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_rapid_tasks_sequence rapid_tasks;
  /** Declared field mechanical_units; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_mechanical_units_sequence mechanical_units;
} roswasmc_bounded_2_abb_robot_msgs_msg_SystemState;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SYSTEMSTATE_MAX_WIRE_BYTES 2327u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SYSTEMSTATE_MIN_WIRE_BYTES 23u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_MSG_SYSTEMSTATE_CODEC_IDENTITY "UWVMROS1_c1cdbf0e578fddc5bf70cc2cc181d786f3e8189afc6a67962de6eb273973a38f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_valid(roswasmc_bounded_2_abb_robot_msgs_msg_SystemState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->motors_on>1)return 0;
  if(value->auto_mode>1)return 0;
  if(value->rapid_running>1)return 0;
  if(value->rapid_tasks.size>16u)return 0;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_valid(&value->rapid_tasks.data[i]))return 0;
  }}
  if(value->mechanical_units.size>16u)return 0;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_valid(&value->mechanical_units.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_wire_size(roswasmc_bounded_2_abb_robot_msgs_msg_SystemState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=1;
  n+=1;
  n+=1;
  n+=4;
  if(value->rapid_tasks.size>16u)return 0;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_wire_size(&value->rapid_tasks.data[i]);
  }}
  n+=4;
  if(value->mechanical_units.size>16u)return 0;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_wire_size(&value->mechanical_units.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_write_fields(roswasmc_bounded_2_abb_robot_msgs_msg_SystemState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->motors_on,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->auto_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rapid_running,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->rapid_tasks.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_write_fields(&value->rapid_tasks.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->mechanical_units.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_write_fields(&value->mechanical_units.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_msg_SystemState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->motors_on,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->auto_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rapid_running,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->rapid_tasks.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->rapid_tasks.size>16u)return 0;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDTaskState_read_fields(p,n,offset,&value->rapid_tasks.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->mechanical_units.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->mechanical_units.size>16u)return 0;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_MechanicalUnitState_read_fields(p,n,offset,&value->mechanical_units.data[i]))return 0;
  }}
  return roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_encode(roswasmc_bounded_2_abb_robot_msgs_msg_SystemState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_msg_SystemState* value) {
  roswasmc_bounded_2_abb_robot_msgs_msg_SystemState next={0};size_t offset=0;if(!value||(length&&!input)||length<23u||length>2327u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_SystemState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetFileContents_Request.filename; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_filename_text;
/** Complete ROS2 abb_robot_msgs/srv/GetFileContents_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request {
  /** Declared field filename; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_filename_text filename;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETFILECONTENTS_REQUEST_MAX_WIRE_BYTES 516u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETFILECONTENTS_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETFILECONTENTS_REQUEST_CODEC_IDENTITY "UWVMROS1_bdcf73a965bd05356e48019b74d633176fdeda62452bc7185d6b6ff30576016b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request const* value) {
  if(!value)return 0;
  if(value->filename.size>512u||!uwvm_bounded_utf8(value->filename.data,value->filename.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request const* value) {size_t n=0;
  n+=4+value->filename.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->filename.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->filename.data,value->filename.size);*offset+=value->filename.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->filename.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->filename.size>512u||value->filename.size>n-*offset)return 0;uwvm_bounded_copy(value->filename.data,p+*offset,value->filename.size);*offset+=value->filename.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>516u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetFileContents_Response.contents; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_contents_text;
/** UTF-8 byte storage for abb_robot_msgs/srv/GetFileContents_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetFileContents_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response {
  /** Declared field contents; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_contents_text contents;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETFILECONTENTS_RESPONSE_MAX_WIRE_BYTES 4618u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETFILECONTENTS_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETFILECONTENTS_RESPONSE_CODEC_IDENTITY "UWVMROS1_f05617145a40043e87cbc251f833274d2a962bd7cdaabc5f9a8e4591bd56e107"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response const* value) {
  if(!value)return 0;
  if(value->contents.size>4096u||!uwvm_bounded_utf8(value->contents.data,value->contents.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response const* value) {size_t n=0;
  n+=4+value->contents.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->contents.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->contents.data,value->contents.size);*offset+=value->contents.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->contents.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->contents.size>4096u||value->contents.size>n-*offset)return 0;uwvm_bounded_copy(value->contents.data,p+*offset,value->contents.size);*offset+=value->contents.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>4618u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetFileContents_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetIOSignal_Request.signal; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_signal_text;
/** Complete ROS2 abb_robot_msgs/srv/GetIOSignal_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request {
  /** Declared field signal; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_signal_text signal;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETIOSIGNAL_REQUEST_MAX_WIRE_BYTES 516u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETIOSIGNAL_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETIOSIGNAL_REQUEST_CODEC_IDENTITY "UWVMROS1_2406d53c2fce44e72ad7e6f1c9e75ea19c4df694ff9f00917229ec758209b1f5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request const* value) {
  if(!value)return 0;
  if(value->signal.size>512u||!uwvm_bounded_utf8(value->signal.data,value->signal.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request const* value) {size_t n=0;
  n+=4+value->signal.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->signal.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->signal.data,value->signal.size);*offset+=value->signal.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->signal.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->signal.size>512u||value->signal.size>n-*offset)return 0;uwvm_bounded_copy(value->signal.data,p+*offset,value->signal.size);*offset+=value->signal.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>516u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetIOSignal_Response.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_value_text;
/** UTF-8 byte storage for abb_robot_msgs/srv/GetIOSignal_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetIOSignal_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_value_text value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETIOSIGNAL_RESPONSE_MAX_WIRE_BYTES 1034u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETIOSIGNAL_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETIOSIGNAL_RESPONSE_CODEC_IDENTITY "UWVMROS1_1f668ef78c353f2ec8defdac619d349cda731576f2429cf07107b9b56ba9030e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response const* value) {
  if(!value)return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response const* value) {size_t n=0;
  n+=4+value->value.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>1034u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetIOSignal_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDBool_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDBOOL_REQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDBOOL_REQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDBOOL_REQUEST_CODEC_IDENTITY "UWVMROS1_c381080d235c78216a8e64275819ff3fb97e1b1de96e7da758c4889630b8e0ff"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRAPIDBool_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDBool_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDBOOL_RESPONSE_MAX_WIRE_BYTES 519u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDBOOL_RESPONSE_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDBOOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_b853e212489fb815866fa29b165eea474a977483158b6203699c4a1293aa6acd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response const* value) {
  if(!value)return 0;
  if(value->value>1)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response const* value) {size_t n=0;
  n+=1;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>519u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDBool_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDDnum_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDDNUM_REQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDDNUM_REQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDDNUM_REQUEST_CODEC_IDENTITY "UWVMROS1_71057c2fe44bc96505ec6e05d998fcb6323b281fff40808ce990207c48626979"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRAPIDDnum_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDDnum_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  double value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDDNUM_RESPONSE_MAX_WIRE_BYTES 526u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDDNUM_RESPONSE_MIN_WIRE_BYTES 14u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDDNUM_RESPONSE_CODEC_IDENTITY "UWVMROS1_af0420c126368a7e45b728c7fef76062b5ef2f0adea7535ad38e4771244edd56"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response const* value) {size_t n=0;
  n+=8;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->value,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<14u||length>526u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDDnum_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDNum_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDNUM_REQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDNUM_REQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDNUM_REQUEST_CODEC_IDENTITY "UWVMROS1_03d5da34af8d90ed8a20f8ca38fa75d9e777496828978f0f348e3c1c8f67d0a5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRAPIDNum_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDNum_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDNUM_RESPONSE_MAX_WIRE_BYTES 522u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDNUM_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDNUM_RESPONSE_CODEC_IDENTITY "UWVMROS1_a0da03014dd001e58efc4a5b274cd5c62016e7fa257af1120a8142a79635220d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response const* value) {size_t n=0;
  n+=4;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>522u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDNum_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDString_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSTRING_REQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSTRING_REQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSTRING_REQUEST_CODEC_IDENTITY "UWVMROS1_c7bc9c788301e619a165c18a1cdb0aca80df8def5be31da5dbf34c0038e73596"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRAPIDString_Response.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_value_text;
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRAPIDString_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDString_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_value_text value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSTRING_RESPONSE_MAX_WIRE_BYTES 1034u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSTRING_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSTRING_RESPONSE_CODEC_IDENTITY "UWVMROS1_cf7bfdb2af5b91eb58b19b909f7fa3357dc1dad02716f8fa4465b53f3f4a1458"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response const* value) {
  if(!value)return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response const* value) {size_t n=0;
  n+=4+value->value.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>1034u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDString_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDSymbol_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSYMBOL_REQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSYMBOL_REQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSYMBOL_REQUEST_CODEC_IDENTITY "UWVMROS1_e216a54cdae70f8147489928ff05a4b172ba4fc47e9ed5df4828b070354ac469"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRAPIDSymbol_Response.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_value_text;
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRAPIDSymbol_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetRAPIDSymbol_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_value_text value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSYMBOL_RESPONSE_MAX_WIRE_BYTES 1034u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSYMBOL_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETRAPIDSYMBOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_b1f6de7ad0640f5645678d327804dc7e732bdad38805e642b13ed7294c21d602"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response const* value) {
  if(!value)return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response const* value) {size_t n=0;
  n+=4+value->value.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>1034u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRAPIDSymbol_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/GetRobotControllerDescription_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETROBOTCONTROLLERDESCRIPTION_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETROBOTCONTROLLERDESCRIPTION_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETROBOTCONTROLLERDESCRIPTION_REQUEST_CODEC_IDENTITY "UWVMROS1_caf70e11b386a933f62d06a433ca1cedaa3c8ecc75b7e8de8c1d5ae00a29f03e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request* value) {
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRobotControllerDescription_Response.description; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_description_text;
/** UTF-8 byte storage for abb_robot_msgs/srv/GetRobotControllerDescription_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetRobotControllerDescription_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response {
  /** Declared field description; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_description_text description;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETROBOTCONTROLLERDESCRIPTION_RESPONSE_MAX_WIRE_BYTES 4618u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETROBOTCONTROLLERDESCRIPTION_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETROBOTCONTROLLERDESCRIPTION_RESPONSE_CODEC_IDENTITY "UWVMROS1_1af2591f035379597f376ed1e2343f73a89f9fef5c29269aa420243a82e6c368"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response const* value) {
  if(!value)return 0;
  if(value->description.size>4096u||!uwvm_bounded_utf8(value->description.data,value->description.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response const* value) {size_t n=0;
  n+=4+value->description.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->description.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->description.data,value->description.size);*offset+=value->description.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->description.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->description.size>4096u||value->description.size>n-*offset)return 0;uwvm_bounded_copy(value->description.data,p+*offset,value->description.size);*offset+=value->description.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>4618u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetRobotControllerDescription_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/GetSpeedRatio_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETSPEEDRATIO_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETSPEEDRATIO_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETSPEEDRATIO_REQUEST_CODEC_IDENTITY "UWVMROS1_54ee2c897e2bf2f39c96cc42246ffe5997c8df280280433e8748b3bcf24c299c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request* value) {
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/GetSpeedRatio_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/GetSpeedRatio_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response {
  /** Declared field speed_ratio; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t speed_ratio;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETSPEEDRATIO_RESPONSE_MAX_WIRE_BYTES 519u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETSPEEDRATIO_RESPONSE_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_GETSPEEDRATIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_baa271a258c7ca274e0145fea1abb7cbc43e98c82b8e40d67c4743237a19910b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response const* value) {size_t n=0;
  n+=1;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed_ratio,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed_ratio,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>519u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_GetSpeedRatio_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetFileContents_Request.filename; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_filename_text;
/** UTF-8 byte storage for abb_robot_msgs/srv/SetFileContents_Request.contents; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_contents_text;
/** Complete ROS2 abb_robot_msgs/srv/SetFileContents_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request {
  /** Declared field filename; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_filename_text filename;
  /** Declared field contents; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_contents_text contents;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETFILECONTENTS_REQUEST_MAX_WIRE_BYTES 4616u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETFILECONTENTS_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETFILECONTENTS_REQUEST_CODEC_IDENTITY "UWVMROS1_3bfa8ad1846fc31acae47a875d3555a78a64b777923eb9b62e87a836f041b26b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request const* value) {
  if(!value)return 0;
  if(value->filename.size>512u||!uwvm_bounded_utf8(value->filename.data,value->filename.size))return 0;
  if(value->contents.size>4096u||!uwvm_bounded_utf8(value->contents.data,value->contents.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request const* value) {size_t n=0;
  n+=4+value->filename.size;
  n+=4+value->contents.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->filename.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->filename.data,value->filename.size);*offset+=value->filename.size;
  uwvm_bounded_put(p+*offset,value->contents.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->contents.data,value->contents.size);*offset+=value->contents.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->filename.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->filename.size>512u||value->filename.size>n-*offset)return 0;uwvm_bounded_copy(value->filename.data,p+*offset,value->filename.size);*offset+=value->filename.size;
  if(*offset>n||n-*offset<4)return 0;value->contents.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->contents.size>4096u||value->contents.size>n-*offset)return 0;uwvm_bounded_copy(value->contents.data,p+*offset,value->contents.size);*offset+=value->contents.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>4616u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetFileContents_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetFileContents_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETFILECONTENTS_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETFILECONTENTS_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETFILECONTENTS_RESPONSE_CODEC_IDENTITY "UWVMROS1_1586344401d43055460b49f2487567638f66838cfe5cac99a47e0a39c0b06ede"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetFileContents_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetIOSignal_Request.signal; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_signal_text;
/** UTF-8 byte storage for abb_robot_msgs/srv/SetIOSignal_Request.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_value_text;
/** Complete ROS2 abb_robot_msgs/srv/SetIOSignal_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request {
  /** Declared field signal; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_signal_text signal;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_value_text value;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETIOSIGNAL_REQUEST_MAX_WIRE_BYTES 1032u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETIOSIGNAL_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETIOSIGNAL_REQUEST_CODEC_IDENTITY "UWVMROS1_2d206d0020179263e83e745a428616ed53d809b0c65a79ad495cb882ae9e49b7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request const* value) {
  if(!value)return 0;
  if(value->signal.size>512u||!uwvm_bounded_utf8(value->signal.data,value->signal.size))return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request const* value) {size_t n=0;
  n+=4+value->signal.size;
  n+=4+value->value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->signal.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->signal.data,value->signal.size);*offset+=value->signal.size;
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->signal.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->signal.size>512u||value->signal.size>n-*offset)return 0;uwvm_bounded_copy(value->signal.data,p+*offset,value->signal.size);*offset+=value->signal.size;
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>1032u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetIOSignal_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetIOSignal_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETIOSIGNAL_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETIOSIGNAL_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETIOSIGNAL_RESPONSE_CODEC_IDENTITY "UWVMROS1_1fa28cc3160d51f71d0740cbe145663ec360d6fa488c2a6bf14ad6ab92004a75"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetIOSignal_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDBool_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDBOOL_REQUEST_MAX_WIRE_BYTES 205u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDBOOL_REQUEST_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDBOOL_REQUEST_CODEC_IDENTITY "UWVMROS1_4e707e429acc860d032d216bcd2c1eb1a0f731fad7e7c0bef7beed810c6e4a52"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  if(value->value>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>205u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetRAPIDBool_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDBool_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDBOOL_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDBOOL_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDBOOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_80d2b31c93771cfa8a19ff82a3d9d63951bf9d28c039d2725017327fa3b0114a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDBool_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDDnum_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  double value;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDDNUM_REQUEST_MAX_WIRE_BYTES 212u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDDNUM_REQUEST_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDDNUM_REQUEST_CODEC_IDENTITY "UWVMROS1_cece086311a908f12c0b0ab3988ca829e891749c74687c717d2a52bfb8ceaf76"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->value,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>212u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetRAPIDDnum_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDDnum_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDDNUM_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDDNUM_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDDNUM_RESPONSE_CODEC_IDENTITY "UWVMROS1_e5f77fe9850f109d873a2784b9690c8aa0632b9e91d158ed4d14daf1aaf90ea6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDDnum_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDNum_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDNUM_REQUEST_MAX_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDNUM_REQUEST_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDNUM_REQUEST_CODEC_IDENTITY "UWVMROS1_a5bee9a64be69dc4246f68837a44362f52795ae3c6420a2df47685606283ea02"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>208u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetRAPIDNum_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDNum_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDNUM_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDNUM_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDNUM_RESPONSE_CODEC_IDENTITY "UWVMROS1_744f3c5fac58ff098c96cedcea94fe6fa87c88ceb89674449f50fbdcd1ca30bb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDNum_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetRAPIDString_Request.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_value_text;
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDString_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_value_text value;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSTRING_REQUEST_MAX_WIRE_BYTES 720u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSTRING_REQUEST_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSTRING_REQUEST_CODEC_IDENTITY "UWVMROS1_f21c45f3f8cd4c0b50781bdd487fdc6ec0c0b35adbc7e36e9ca5b7cc47c3d461"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  n+=4+value->value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>720u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetRAPIDString_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDString_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSTRING_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSTRING_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSTRING_RESPONSE_CODEC_IDENTITY "UWVMROS1_1496ef4f4ffe48fe74b9e25b925c19f10f0e171a9d33fa2e56c15aa5cfbe4774"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDString_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetRAPIDSymbol_Request.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_value_text;
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDSymbol_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_value_text value;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSYMBOL_REQUEST_MAX_WIRE_BYTES 720u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSYMBOL_REQUEST_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSYMBOL_REQUEST_CODEC_IDENTITY "UWVMROS1_928d427801b79775595fed6e3779f9ba9e7e8e27f2de66cf79610cb51bb72bcf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_valid(&value->path))return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_wire_size(&value->path);
  n+=4+value->value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request* value) {
  if(!roswasmc_bounded_2_abb_robot_msgs_msg_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>720u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetRAPIDSymbol_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetRAPIDSymbol_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSYMBOL_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSYMBOL_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETRAPIDSYMBOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_e156616a51bf4691a1c3853d62a8b0c43bb17d1a330cd4e7a58eceb5fe442382"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetRAPIDSymbol_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/SetSpeedRatio_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request {
  /** Declared field speed_ratio; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t speed_ratio;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETSPEEDRATIO_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETSPEEDRATIO_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETSPEEDRATIO_REQUEST_CODEC_IDENTITY "UWVMROS1_da30970377ba1bc151b544560773f02e17f4eaed56eb4c8d791de72bdef04355"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed_ratio,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed_ratio,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/SetSpeedRatio_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/SetSpeedRatio_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETSPEEDRATIO_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETSPEEDRATIO_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_SETSPEEDRATIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_25e6d7edaa0503a4d45d893e886db4215bfd0af8711307c141bc63552609e3a5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_SetSpeedRatio_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 abb_robot_msgs/srv/TriggerWithResultCode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_TRIGGERWITHRESULTCODE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_TRIGGERWITHRESULTCODE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_TRIGGERWITHRESULTCODE_REQUEST_CODEC_IDENTITY "UWVMROS1_fd1d49c3b4e489464c6662f306210137f71d15192f64cd8a8abf93acd5fbff8d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_valid(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request* value) {
  return roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_encode(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/srv/TriggerWithResultCode_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_message_text;
/** Complete ROS2 abb_robot_msgs/srv/TriggerWithResultCode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_message_text message;
} roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response;
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_TRIGGERWITHRESULTCODE_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_TRIGGERWITHRESULTCODE_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_ABB_ROBOT_MSGS_SRV_TRIGGERWITHRESULTCODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_e38422b2a008f07e108505ac3ce65c24189a1d6657e88a6289e17d7d4ae38d51"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_valid(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_wire_size(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_write_fields(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_encode(roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response* value) {
  roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_abb_robot_msgs_srv_TriggerWithResultCode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 builtin_interfaces/msg/Duration value; no field projects away native data. */
typedef struct roswasmc_bounded_2_builtin_interfaces_msg_Duration {
  /** Declared field sec; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sec;
  /** Declared field nanosec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t nanosec;
} roswasmc_bounded_2_builtin_interfaces_msg_Duration;
#define ROSWASMC_BOUNDED_2_BUILTIN_INTERFACES_MSG_DURATION_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_BUILTIN_INTERFACES_MSG_DURATION_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_BUILTIN_INTERFACES_MSG_DURATION_CODEC_IDENTITY "UWVMROS1_3804444fae7a82a57907d25c2c3dc54d9a42dcd305f41ca073b1221dca7d36e4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(roswasmc_bounded_2_builtin_interfaces_msg_Duration const* value) {
  if(!value)return 0;
  if(value->nanosec>=1000000000)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(roswasmc_bounded_2_builtin_interfaces_msg_Duration const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(roswasmc_bounded_2_builtin_interfaces_msg_Duration const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->sec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nanosec,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_builtin_interfaces_msg_Duration* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->nanosec,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_builtin_interfaces_msg_Duration_encode(roswasmc_bounded_2_builtin_interfaces_msg_Duration const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_builtin_interfaces_msg_Duration_decode(void const* input,size_t length,roswasmc_bounded_2_builtin_interfaces_msg_Duration* value) {
  roswasmc_bounded_2_builtin_interfaces_msg_Duration next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_positions_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_velocities_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_accelerations_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_effort_sequence;
/** Complete ROS2 trajectory_msgs/msg/JointTrajectoryPoint value; no field projects away native data. */
typedef struct roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint {
  /** Declared field positions; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_positions_sequence positions;
  /** Declared field velocities; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_velocities_sequence velocities;
  /** Declared field accelerations; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_accelerations_sequence accelerations;
  /** Declared field effort; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_effort_sequence effort;
  /** Declared field time_from_start; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration time_from_start;
} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint;
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_MAX_WIRE_BYTES 408u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_CODEC_IDENTITY "UWVMROS1_4d55f494f060a9a3ff23bedd54da49f9e70e6b77acbdf911878afbf672cbadc5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value) {
  if(!value)return 0;
  if(value->positions.size>12u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->positions.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->velocities.size>12u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocities.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->accelerations.size>12u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->accelerations.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->effort.size>12u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->effort.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->time_from_start))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value) {size_t n=0;
  n+=4;
  if(value->positions.size>12u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->velocities.size>12u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->accelerations.size>12u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->effort.size>12u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  n+=8;
  }}
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->time_from_start);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->positions.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->positions.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->positions.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->velocities.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocities.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->accelerations.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->accelerations.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->effort.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->effort.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->effort.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->time_from_start,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint* value) {
  if(*offset>n||n-*offset<4)return 0;value->positions.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->positions.size>12u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->positions.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocities.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocities.size>12u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocities.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->accelerations.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->accelerations.size>12u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->accelerations.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->effort.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->effort.size>12u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->effort.data[i],&bits,8);}*offset+=8;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->time_from_start))return 0;
  return roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_encode(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_decode(void const* input,size_t length,roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint* value) {
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>408u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for trajectory_msgs/msg/JointTrajectory.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_text;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_text data[12];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint data[8];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_points_sequence;
/** Complete ROS2 trajectory_msgs/msg/JointTrajectory value; no field projects away native data. */
typedef struct roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_sequence joint_names;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_points_sequence points;
} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory;
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_MAX_WIRE_BYTES 4164u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_CODEC_IDENTITY "UWVMROS1_d74956440763bd09c76ec623efeb322aa89be8ef6410b9125d4e2cb1aeeb69c0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>64u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(value->points.size>8u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->points.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=4;
  if(value->points.size>8u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->points.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_write_fields(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->points.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->points.size;++i){
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->points.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>64u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>8u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->points.data[i]))return 0;
  }}
  return roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_encode(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_decode(void const* input,size_t length,roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory* value) {
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>4164u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Vector3 value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Vector3 {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  double x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  double y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  double z;
} roswasmc_bounded_2_geometry_msgs_msg_Vector3;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_VECTOR3_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_VECTOR3_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_VECTOR3_CODEC_IDENTITY "UWVMROS1_5adebf211fe375fba92c0628f9b8ebf05bff9ab22ddc4bbf00629167af295d5e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(roswasmc_bounded_2_geometry_msgs_msg_Vector3 const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Vector3 const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Vector3 const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Vector3* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->x,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->y,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->z,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Vector3_encode(roswasmc_bounded_2_geometry_msgs_msg_Vector3 const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Vector3_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Vector3* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Quaternion value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Quaternion {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  double x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  double y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  double z;
  /** Declared field w; canonical capacity is part of CODEC_IDENTITY. */
  double w;
} roswasmc_bounded_2_geometry_msgs_msg_Quaternion;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_QUATERNION_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_QUATERNION_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_QUATERNION_CODEC_IDENTITY "UWVMROS1_34a32c3470cbfe2ff12340781a5bf22f6c370a8f5c06669f25a34321ade81672"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(roswasmc_bounded_2_geometry_msgs_msg_Quaternion* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.x=0x0.0p+0;
  next.y=0x0.0p+0;
  next.z=0x0.0p+0;
  next.w=0x1.0000000000000p+0;
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(roswasmc_bounded_2_geometry_msgs_msg_Quaternion const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->w,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Quaternion_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Quaternion const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Quaternion_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Quaternion const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->w,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Quaternion_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Quaternion* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->x,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->y,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->z,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->w,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Quaternion_encode(roswasmc_bounded_2_geometry_msgs_msg_Quaternion const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Quaternion_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Quaternion_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Quaternion* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Transform value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Transform {
  /** Declared field translation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 translation;
  /** Declared field rotation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion rotation;
} roswasmc_bounded_2_geometry_msgs_msg_Transform;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORM_MAX_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORM_MIN_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORM_CODEC_IDENTITY "UWVMROS1_7ddcb1c3fca43f4a0299d43fb6fac1b77a8f26001af809dc13307f4f9c61d65b"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Transform_default(roswasmc_bounded_2_geometry_msgs_msg_Transform* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Transform next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&next.rotation);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Transform_valid(roswasmc_bounded_2_geometry_msgs_msg_Transform const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->translation))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(&value->rotation))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Transform_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Transform const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->translation);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Quaternion_wire_size(&value->rotation);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Transform_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Transform const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->translation,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion_write_fields(&value->rotation,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Transform_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Transform* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->translation))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_read_fields(p,n,offset,&value->rotation))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_Transform_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Transform_encode(roswasmc_bounded_2_geometry_msgs_msg_Transform const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Transform_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Transform_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Transform_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Transform_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Transform* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Transform next={0};size_t offset=0;if(!value||(length&&!input)||length<56u||length>56u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Twist value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Twist {
  /** Declared field linear; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 linear;
  /** Declared field angular; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 angular;
} roswasmc_bounded_2_geometry_msgs_msg_Twist;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWIST_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWIST_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWIST_CODEC_IDENTITY "UWVMROS1_1a6c18d19bb6853a2cff3b775974ba31f8e3674c1bbe566444ccabf0ef9d117e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(roswasmc_bounded_2_geometry_msgs_msg_Twist const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->linear))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->angular))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Twist const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->linear);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->angular);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Twist_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Twist const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->linear,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->angular,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Twist* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->linear))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->angular))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Twist_encode(roswasmc_bounded_2_geometry_msgs_msg_Twist const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Twist_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Twist_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Twist* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Twist next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_geometry_msgs_msg_Transform data[2];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_transforms_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_geometry_msgs_msg_Twist data[2];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_velocities_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_geometry_msgs_msg_Twist data[2];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_accelerations_sequence;
/** Complete ROS2 trajectory_msgs/msg/MultiDOFJointTrajectoryPoint value; no field projects away native data. */
typedef struct roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint {
  /** Declared field transforms; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_transforms_sequence transforms;
  /** Declared field velocities; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_velocities_sequence velocities;
  /** Declared field accelerations; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_accelerations_sequence accelerations;
  /** Declared field time_from_start; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration time_from_start;
} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint;
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORYPOINT_MAX_WIRE_BYTES 324u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORYPOINT_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORYPOINT_CODEC_IDENTITY "UWVMROS1_6590b7ca1cdaa89da04b7a661266e2d4c6e3e2668dc90505a7c24d7be8a43242"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_default(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint* value) {
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint next={0};if(!value)return UWVM_BOUNDED_INVALID;
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint const* value) {
  if(!value)return 0;
  if(value->transforms.size>2u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_valid(&value->transforms.data[i]))return 0;
  }}
  if(value->velocities.size>2u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value->velocities.data[i]))return 0;
  }}
  if(value->accelerations.size>2u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value->accelerations.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->time_from_start))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint const* value) {size_t n=0;
  n+=4;
  if(value->transforms.size>2u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_Transform_wire_size(&value->transforms.data[i]);
  }}
  n+=4;
  if(value->velocities.size>2u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(&value->velocities.data[i]);
  }}
  n+=4;
  if(value->accelerations.size>2u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(&value->accelerations.data[i]);
  }}
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->time_from_start);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_write_fields(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->transforms.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  roswasmc_bounded_2_geometry_msgs_msg_Transform_write_fields(&value->transforms.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->velocities.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  roswasmc_bounded_2_geometry_msgs_msg_Twist_write_fields(&value->velocities.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->accelerations.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  roswasmc_bounded_2_geometry_msgs_msg_Twist_write_fields(&value->accelerations.data[i],p,offset);
  }}
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->time_from_start,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint* value) {
  if(*offset>n||n-*offset<4)return 0;value->transforms.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->transforms.size>2u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_read_fields(p,n,offset,&value->transforms.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocities.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocities.size>2u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields(p,n,offset,&value->velocities.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->accelerations.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->accelerations.size>2u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields(p,n,offset,&value->accelerations.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->time_from_start))return 0;
  return roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_encode(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_decode(void const* input,size_t length,roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint* value) {
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>324u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for trajectory_msgs/msg/MultiDOFJointTrajectory.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_text;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_text data[2];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_sequence;
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint data[4];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_points_sequence;
/** Complete ROS2 trajectory_msgs/msg/MultiDOFJointTrajectory value; no field projects away native data. */
typedef struct roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_sequence joint_names;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_points_sequence points;
} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory;
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORY_MAX_WIRE_BYTES 1516u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORY_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORY_CODEC_IDENTITY "UWVMROS1_6ea66f146cdce31369361e521be484d2c39bca818cc4b2069e1209d250ec2401"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_default(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory* value) {
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory next={0};if(!value)return UWVM_BOUNDED_INVALID;
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_valid(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->joint_names.size>2u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>64u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(&value->points.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>2u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=4;
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  n+=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(&value->points.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_write_fields(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->points.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->points.size;++i){
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_write_fields(&value->points.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>2u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>64u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_read_fields(p,n,offset,&value->points.data[i]))return 0;
  }}
  return roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_encode(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_decode(void const* input,size_t length,roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory* value) {
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>1516u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/msg/JointTolerance.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_control_msgs_msg_JointTolerance_name_text;
/** Complete ROS2 control_msgs/msg/JointTolerance value; no field projects away native data. */
typedef struct roswasmc_bounded_2_control_msgs_msg_JointTolerance {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_msg_JointTolerance_name_text name;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  double position;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  double velocity;
  /** Declared field acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double acceleration;
} roswasmc_bounded_2_control_msgs_msg_JointTolerance;
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTTOLERANCE_MAX_WIRE_BYTES 92u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTTOLERANCE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTTOLERANCE_CODEC_IDENTITY "UWVMROS1_ad7701ac1cb0dd68a947577272964b0d6051d1846019de97e9c9d0f7be7b2fa2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(roswasmc_bounded_2_control_msgs_msg_JointTolerance const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(roswasmc_bounded_2_control_msgs_msg_JointTolerance const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_control_msgs_msg_JointTolerance_write_fields(roswasmc_bounded_2_control_msgs_msg_JointTolerance const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_control_msgs_msg_JointTolerance* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->acceleration,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_control_msgs_msg_JointTolerance_encode(roswasmc_bounded_2_control_msgs_msg_JointTolerance const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_control_msgs_msg_JointTolerance_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_control_msgs_msg_JointTolerance_decode(void const* input,size_t length,roswasmc_bounded_2_control_msgs_msg_JointTolerance* value) {
  roswasmc_bounded_2_control_msgs_msg_JointTolerance next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>92u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/msg/JointComponentTolerance.joint_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_joint_name_text;
/** Complete ROS2 control_msgs/msg/JointComponentTolerance value; no field projects away native data. */
typedef struct roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance {
  /** Declared field joint_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_joint_name_text joint_name;
  /** Declared field component; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t component;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  double position;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  double velocity;
  /** Declared field acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double acceleration;
} roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance;
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_MAX_WIRE_BYTES 542u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_MIN_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_CODEC_IDENTITY "UWVMROS1_658f3d3f180b301e5c612188390187ec4034064976f133018048bb1fdec55342"
/** Original scalar constant X_AXIS=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_X_AXIS UINT32_C(1)
/** Original scalar constant Y_AXIS=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_Y_AXIS UINT32_C(2)
/** Original scalar constant Z_AXIS=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_Z_AXIS UINT32_C(3)
/** Original scalar constant TRANSLATION=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_TRANSLATION UINT32_C(4)
/** Original scalar constant ROTATION=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_ROTATION UINT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance const* value) {
  if(!value)return 0;
  if(value->joint_name.size>512u||!uwvm_bounded_utf8(value->joint_name.data,value->joint_name.size))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_wire_size(roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance const* value) {size_t n=0;
  n+=4+value->joint_name.size;
  n+=2;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_write_fields(roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->joint_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_name.data,value->joint_name.size);*offset+=value->joint_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->component,2);*offset+=2;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance* value) {
  if(*offset>n||n-*offset<4)return 0;value->joint_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_name.size>512u||value->joint_name.size>n-*offset)return 0;uwvm_bounded_copy(value->joint_name.data,p+*offset,value->joint_name.size);*offset+=value->joint_name.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->component,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->acceleration,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_encode(roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_decode(void const* input,size_t length,roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance* value) {
  roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance next={0};size_t offset=0;if(!value||(length&&!input)||length<30u||length>542u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointTolerance data[12];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_path_tolerance_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance data[12];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_component_path_tolerance_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointTolerance data[12];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_goal_tolerance_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance data[12];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_component_goal_tolerance_sequence;
/** Complete ROS2 control_msgs/action/FollowJointTrajectory_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal {
  /** Declared field trajectory; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory trajectory;
  /** Declared field multi_dof_trajectory; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory multi_dof_trajectory;
  /** Declared field path_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_path_tolerance_sequence path_tolerance;
  /** Declared field component_path_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_component_path_tolerance_sequence component_path_tolerance;
  /** Declared field goal_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_goal_tolerance_sequence goal_tolerance;
  /** Declared field component_goal_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_component_goal_tolerance_sequence component_goal_tolerance;
  /** Declared field goal_time_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration goal_time_tolerance;
} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal;
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_MAX_WIRE_BYTES 20920u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_MIN_WIRE_BYTES 64u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_CODEC_IDENTITY "UWVMROS1_8945d5b29832f1190b0a4f8c4af4e82686c4b913413affe9c0945570b4fb1f16"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_default(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal* value) {
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_default(&next.multi_dof_trajectory);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_valid(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(&value->trajectory))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_valid(&value->multi_dof_trajectory))return 0;
  if(value->path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value->path_tolerance.data[i]))return 0;
  }}
  if(value->component_path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->component_path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(&value->component_path_tolerance.data[i]))return 0;
  }}
  if(value->goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value->goal_tolerance.data[i]))return 0;
  }}
  if(value->component_goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->component_goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(&value->component_goal_tolerance.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->goal_time_tolerance))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_wire_size(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal const* value) {size_t n=0;
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(&value->trajectory);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_wire_size(&value->multi_dof_trajectory);
  n+=4;
  if(value->path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value->path_tolerance.data[i]);
  }}
  n+=4;
  if(value->component_path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->component_path_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_wire_size(&value->component_path_tolerance.data[i]);
  }}
  n+=4;
  if(value->goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value->goal_tolerance.data[i]);
  }}
  n+=4;
  if(value->component_goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->component_goal_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_wire_size(&value->component_goal_tolerance.data[i]);
  }}
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->goal_time_tolerance);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_write_fields(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_write_fields(&value->trajectory,p,offset);
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_write_fields(&value->multi_dof_trajectory,p,offset);
  uwvm_bounded_put(p+*offset,value->path_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  roswasmc_bounded_2_control_msgs_msg_JointTolerance_write_fields(&value->path_tolerance.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->component_path_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->component_path_tolerance.size;++i){
  roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_write_fields(&value->component_path_tolerance.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->goal_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  roswasmc_bounded_2_control_msgs_msg_JointTolerance_write_fields(&value->goal_tolerance.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->component_goal_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->component_goal_tolerance.size;++i){
  roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_write_fields(&value->component_goal_tolerance.data[i],p,offset);
  }}
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->goal_time_tolerance,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal* value) {
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_read_fields(p,n,offset,&value->trajectory))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_read_fields(p,n,offset,&value->multi_dof_trajectory))return 0;
  if(*offset>n||n-*offset<4)return 0;value->path_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields(p,n,offset,&value->path_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->component_path_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->component_path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->component_path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_read_fields(p,n,offset,&value->component_path_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->goal_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields(p,n,offset,&value->goal_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->component_goal_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->component_goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->component_goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_read_fields(p,n,offset,&value->component_goal_tolerance.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->goal_time_tolerance))return 0;
  return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_encode(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal* value) {
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<64u||length>20920u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/action/FollowJointTrajectory_Result.error_string; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_error_string_text;
/** Complete ROS2 control_msgs/action/FollowJointTrajectory_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result {
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  int32_t error_code;
  /** Declared field error_string; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_error_string_text error_string;
} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result;
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_MAX_WIRE_BYTES 4104u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_CODEC_IDENTITY "UWVMROS1_c698e3a3a772735ff6027b04af28e9bc88d6b7138863ce5737aa090f543dac70"
/** Original scalar constant SUCCESSFUL=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_SUCCESSFUL INT32_C(0)
/** Original scalar constant INVALID_GOAL=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_INVALID_GOAL -INT32_C(1)
/** Original scalar constant INVALID_JOINTS=-2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_INVALID_JOINTS -INT32_C(2)
/** Original scalar constant OLD_HEADER_TIMESTAMP=-3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_OLD_HEADER_TIMESTAMP -INT32_C(3)
/** Original scalar constant PATH_TOLERANCE_VIOLATED=-4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_PATH_TOLERANCE_VIOLATED -INT32_C(4)
/** Original scalar constant GOAL_TOLERANCE_VIOLATED=-5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_GOAL_TOLERANCE_VIOLATED -INT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_valid(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result const* value) {
  if(!value)return 0;
  if(value->error_string.size>4096u||!uwvm_bounded_utf8(value->error_string.data,value->error_string.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_wire_size(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result const* value) {size_t n=0;
  n+=4;
  n+=4+value->error_string.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_write_fields(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->error_string.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_string.data,value->error_string.size);*offset+=value->error_string.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->error_string.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_string.size>4096u||value->error_string.size>n-*offset)return 0;uwvm_bounded_copy(value->error_string.data,p+*offset,value->error_string.size);*offset+=value->error_string.size;
  return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_encode(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_decode(void const* input,size_t length,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result* value) {
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>4104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/action/FollowJointTrajectory_Feedback.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_joint_names_text;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_joint_names_text data[12];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_joint_names_sequence;
/** UTF-8 byte storage for control_msgs/action/FollowJointTrajectory_Feedback.multi_dof_joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_multi_dof_joint_names_text;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_multi_dof_joint_names_text data[2];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_multi_dof_joint_names_sequence;
/** Complete ROS2 control_msgs/action/FollowJointTrajectory_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_joint_names_sequence joint_names;
  /** Declared field desired; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint desired;
  /** Declared field actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint actual;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint error;
  /** Declared field multi_dof_joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_multi_dof_joint_names_sequence multi_dof_joint_names;
  /** Declared field multi_dof_desired; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint multi_dof_desired;
  /** Declared field multi_dof_actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint multi_dof_actual;
  /** Declared field multi_dof_error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint multi_dof_error;
} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback;
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_MAX_WIRE_BYTES 4128u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_MIN_WIRE_BYTES 152u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_CODEC_IDENTITY "UWVMROS1_68f3809217bd914cd971e1365ca825111c1394b2587c6989736adcaa8a40a7cb"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_default(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback* value) {
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_default(&next.multi_dof_desired);
  (void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_default(&next.multi_dof_actual);
  (void)roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_default(&next.multi_dof_error);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_valid(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>64u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->error))return 0;
  if(value->multi_dof_joint_names.size>2u)return 0;
  {size_t i;for(i=0;i<value->multi_dof_joint_names.size;++i){
  if(value->multi_dof_joint_names.data[i].size>512u||!uwvm_bounded_utf8(value->multi_dof_joint_names.data[i].data,value->multi_dof_joint_names.data[i].size))return 0;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(&value->multi_dof_desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(&value->multi_dof_actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(&value->multi_dof_error))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_wire_size(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->desired);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->actual);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->error);
  n+=4;
  if(value->multi_dof_joint_names.size>2u)return 0;
  {size_t i;for(i=0;i<value->multi_dof_joint_names.size;++i){
  n+=4+value->multi_dof_joint_names.data[i].size;
  }}
  n+=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(&value->multi_dof_desired);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(&value->multi_dof_actual);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(&value->multi_dof_error);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_write_fields(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->desired,p,offset);
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->actual,p,offset);
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->error,p,offset);
  uwvm_bounded_put(p+*offset,value->multi_dof_joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->multi_dof_joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->multi_dof_joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->multi_dof_joint_names.data[i].data,value->multi_dof_joint_names.data[i].size);*offset+=value->multi_dof_joint_names.data[i].size;
  }}
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_write_fields(&value->multi_dof_desired,p,offset);
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_write_fields(&value->multi_dof_actual,p,offset);
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_write_fields(&value->multi_dof_error,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>64u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->error))return 0;
  if(*offset>n||n-*offset<4)return 0;value->multi_dof_joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->multi_dof_joint_names.size>2u)return 0;
  {size_t i;for(i=0;i<value->multi_dof_joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->multi_dof_joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->multi_dof_joint_names.data[i].size>512u||value->multi_dof_joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->multi_dof_joint_names.data[i].data,p+*offset,value->multi_dof_joint_names.data[i].size);*offset+=value->multi_dof_joint_names.data[i].size;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_read_fields(p,n,offset,&value->multi_dof_desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_read_fields(p,n,offset,&value->multi_dof_actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_read_fields(p,n,offset,&value->multi_dof_error))return 0;
  return roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_encode(roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback* value) {
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<152u||length>4128u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for sensor_msgs/msg/JointState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text data[12];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_2_sensor_msgs_msg_JointState_position_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_2_sensor_msgs_msg_JointState_velocity_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_2_sensor_msgs_msg_JointState_effort_sequence;
/** Complete ROS2 sensor_msgs/msg/JointState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_sensor_msgs_msg_JointState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState_name_sequence name;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState_position_sequence position;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState_velocity_sequence velocity;
  /** Declared field effort; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState_effort_sequence effort;
} roswasmc_bounded_2_sensor_msgs_msg_JointState;
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MAX_WIRE_BYTES 1196u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_CODEC_IDENTITY "UWVMROS1_88d44f7bdb40f8c45539a041a25903609561fa74db90b9972e1369ebbff6c884"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(roswasmc_bounded_2_sensor_msgs_msg_JointState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->name.size>12u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(value->name.data[i].size>64u||!uwvm_bounded_utf8(value->name.data[i].data,value->name.data[i].size))return 0;
  }}
  if(value->position.size>12u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->velocity.size>12u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->effort.size>12u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->effort.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if((value->name.size!=0&&value->position.size!=0&&value->name.size!=value->position.size)||(value->name.size!=0&&value->velocity.size!=0&&value->name.size!=value->velocity.size)||(value->name.size!=0&&value->effort.size!=0&&value->name.size!=value->effort.size)||(value->position.size!=0&&value->velocity.size!=0&&value->position.size!=value->velocity.size)||(value->position.size!=0&&value->effort.size!=0&&value->position.size!=value->effort.size)||(value->velocity.size!=0&&value->effort.size!=0&&value->velocity.size!=value->effort.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(roswasmc_bounded_2_sensor_msgs_msg_JointState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->name.size>12u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  n+=4+value->name.data[i].size;
  }}
  n+=4;
  if(value->position.size>12u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->velocity.size>12u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->effort.size>12u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_sensor_msgs_msg_JointState_write_fields(roswasmc_bounded_2_sensor_msgs_msg_JointState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->name.size;++i){
  uwvm_bounded_put(p+*offset,value->name.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data[i].data,value->name.data[i].size);*offset+=value->name.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->position.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->position.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->velocity.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->effort.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->effort.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->effort.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_sensor_msgs_msg_JointState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>12u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->name.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.data[i].size>64u||value->name.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->name.data[i].data,p+*offset,value->name.data[i].size);*offset+=value->name.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->position.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->position.size>12u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocity.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocity.size>12u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->effort.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->effort.size>12u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->effort.data[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_JointState_encode(roswasmc_bounded_2_sensor_msgs_msg_JointState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_sensor_msgs_msg_JointState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_JointState_decode(void const* input,size_t length,roswasmc_bounded_2_sensor_msgs_msg_JointState* value) {
  roswasmc_bounded_2_sensor_msgs_msg_JointState next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>1196u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 std_srvs/srv/Trigger_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_std_srvs_srv_Trigger_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_std_srvs_srv_Trigger_Request;
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_REQUEST_CODEC_IDENTITY "UWVMROS1_f1d8ad4ab7cf476cada6301f71ae24e046c244ba62bb6735183efe8e8c530a77"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_std_srvs_srv_Trigger_Request_valid(roswasmc_bounded_2_std_srvs_srv_Trigger_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_std_srvs_srv_Trigger_Request_wire_size(roswasmc_bounded_2_std_srvs_srv_Trigger_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_std_srvs_srv_Trigger_Request_write_fields(roswasmc_bounded_2_std_srvs_srv_Trigger_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_std_srvs_srv_Trigger_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_std_srvs_srv_Trigger_Request* value) {
  return roswasmc_bounded_2_std_srvs_srv_Trigger_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_Trigger_Request_encode(roswasmc_bounded_2_std_srvs_srv_Trigger_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_std_srvs_srv_Trigger_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_std_srvs_srv_Trigger_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_std_srvs_srv_Trigger_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_Trigger_Request_decode(void const* input,size_t length,roswasmc_bounded_2_std_srvs_srv_Trigger_Request* value) {
  roswasmc_bounded_2_std_srvs_srv_Trigger_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_std_srvs_srv_Trigger_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for std_srvs/srv/Trigger_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_std_srvs_srv_Trigger_Response_message_text;
/** Complete ROS2 std_srvs/srv/Trigger_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_std_srvs_srv_Trigger_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_srvs_srv_Trigger_Response_message_text message;
} roswasmc_bounded_2_std_srvs_srv_Trigger_Response;
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_RESPONSE_MAX_WIRE_BYTES 517u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_RESPONSE_CODEC_IDENTITY "UWVMROS1_95fe65bd59112bb00093f66c6853844b01ccea8e6ded47e73038718aa886f39d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_std_srvs_srv_Trigger_Response_valid(roswasmc_bounded_2_std_srvs_srv_Trigger_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_std_srvs_srv_Trigger_Response_wire_size(roswasmc_bounded_2_std_srvs_srv_Trigger_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_std_srvs_srv_Trigger_Response_write_fields(roswasmc_bounded_2_std_srvs_srv_Trigger_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_std_srvs_srv_Trigger_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_std_srvs_srv_Trigger_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_std_srvs_srv_Trigger_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_Trigger_Response_encode(roswasmc_bounded_2_std_srvs_srv_Trigger_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_std_srvs_srv_Trigger_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_std_srvs_srv_Trigger_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_std_srvs_srv_Trigger_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_Trigger_Response_decode(void const* input,size_t length,roswasmc_bounded_2_std_srvs_srv_Trigger_Response* value) {
  roswasmc_bounded_2_std_srvs_srv_Trigger_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>517u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_std_srvs_srv_Trigger_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
