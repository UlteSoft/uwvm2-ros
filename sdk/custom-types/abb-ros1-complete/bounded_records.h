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
/** UTF-8 byte storage for abb_egm_msgs/EGMChannelState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_name_text;
/** Complete ROS1 abb_egm_msgs/EGMChannelState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_egm_msgs_EGMChannelState {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_name_text name;
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
} roswasmc_bounded_1_abb_egm_msgs_EGMChannelState;
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_MAX_WIRE_BYTES 81u
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_CODEC_IDENTITY "UWVMROS1_452d2adde3b1475fe3ff09ea11ff2ec7105a58c32e20d764e0c2ff152cb432f2"
/** Original scalar constant EGM_UNDEFINED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_EGM_UNDEFINED UINT32_C(1)
/** Original scalar constant EGM_ERROR=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_EGM_ERROR UINT32_C(2)
/** Original scalar constant EGM_STOPPED=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_EGM_STOPPED UINT32_C(3)
/** Original scalar constant EGM_RUNNING=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_EGM_RUNNING UINT32_C(4)
/** Original scalar constant MOTORS_UNDEFINED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_MOTORS_UNDEFINED UINT32_C(1)
/** Original scalar constant MOTORS_ON=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_MOTORS_ON UINT32_C(2)
/** Original scalar constant MOTORS_OFF=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_MOTORS_OFF UINT32_C(3)
/** Original scalar constant RAPID_UNDEFINED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_RAPID_UNDEFINED UINT32_C(1)
/** Original scalar constant RAPID_STOPPED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_RAPID_STOPPED UINT32_C(2)
/** Original scalar constant RAPID_RUNNING=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMCHANNELSTATE_RAPID_RUNNING UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_valid(roswasmc_bounded_1_abb_egm_msgs_EGMChannelState const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  if(value->active>1)return 0;
  if(value->egm_convergence_met>1)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->utilization_rate,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_wire_size(roswasmc_bounded_1_abb_egm_msgs_EGMChannelState const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_write_fields(roswasmc_bounded_1_abb_egm_msgs_EGMChannelState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->active,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->egm_convergence_met,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->egm_client_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->motor_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rapid_execution_state,1);*offset+=1;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->utilization_rate,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_egm_msgs_EGMChannelState* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->active,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->egm_convergence_met,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->egm_client_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->motor_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rapid_execution_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->utilization_rate,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_encode(roswasmc_bounded_1_abb_egm_msgs_EGMChannelState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_decode(void const* input,size_t length,roswasmc_bounded_1_abb_egm_msgs_EGMChannelState* value) {
  roswasmc_bounded_1_abb_egm_msgs_EGMChannelState next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>81u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 time value; no field projects away native data. */
typedef struct roswasmc_bounded_1_time {
  /** Declared field sec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t sec;
  /** Declared field nsec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t nsec;
} roswasmc_bounded_1_time;
#define ROSWASMC_BOUNDED_1_TIME_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_TIME_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_TIME_CODEC_IDENTITY "UWVMROS1_90b75571d2bcc21b824eb993b485a001bf97e281adda7907e1b2e3357e4804b0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_time_valid(roswasmc_bounded_1_time const* value) {
  if(!value)return 0;
  if(value->nsec>=1000000000)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_time_wire_size(roswasmc_bounded_1_time const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_time_write_fields(roswasmc_bounded_1_time const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->sec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nsec,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_time_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_time* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->nsec,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_time_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_time_encode(roswasmc_bounded_1_time const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_time_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_time_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_time_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_time_decode(void const* input,size_t length,roswasmc_bounded_1_time* value) {
  roswasmc_bounded_1_time next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_time_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for std_msgs/Header.frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_std_msgs_Header_frame_id_text;
/** Complete ROS1 std_msgs/Header value; no field projects away native data. */
typedef struct roswasmc_bounded_1_std_msgs_Header {
  /** Declared field seq; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t seq;
  /** Declared field stamp; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_time stamp;
  /** Declared field frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header_frame_id_text frame_id;
} roswasmc_bounded_1_std_msgs_Header;
#define ROSWASMC_BOUNDED_1_STD_MSGS_HEADER_MAX_WIRE_BYTES 80u
#define ROSWASMC_BOUNDED_1_STD_MSGS_HEADER_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_STD_MSGS_HEADER_CODEC_IDENTITY "UWVMROS1_ca1b8ffce276bdc4f86dccae58ede9f860ffe8778c6d4ffe0fb70675777801db"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_std_msgs_Header_valid(roswasmc_bounded_1_std_msgs_Header const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_time_valid(&value->stamp))return 0;
  if(value->frame_id.size>64u||!uwvm_bounded_utf8(value->frame_id.data,value->frame_id.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_std_msgs_Header_wire_size(roswasmc_bounded_1_std_msgs_Header const* value) {size_t n=0;
  n+=4;
  n+=roswasmc_bounded_1_time_wire_size(&value->stamp);
  n+=4+value->frame_id.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_std_msgs_Header_write_fields(roswasmc_bounded_1_std_msgs_Header const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->seq,4);*offset+=4;
  roswasmc_bounded_1_time_write_fields(&value->stamp,p,offset);
  uwvm_bounded_put(p+*offset,value->frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->frame_id.data,value->frame_id.size);*offset+=value->frame_id.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_std_msgs_Header_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_std_msgs_Header* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->seq,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_1_time_read_fields(p,n,offset,&value->stamp))return 0;
  if(*offset>n||n-*offset<4)return 0;value->frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->frame_id.size>64u||value->frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->frame_id.data,p+*offset,value->frame_id.size);*offset+=value->frame_id.size;
  return roswasmc_bounded_1_std_msgs_Header_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_std_msgs_Header_encode(roswasmc_bounded_1_std_msgs_Header const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_std_msgs_Header_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_std_msgs_Header_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_std_msgs_Header_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_std_msgs_Header_decode(void const* input,size_t length,roswasmc_bounded_1_std_msgs_Header* value) {
  roswasmc_bounded_1_std_msgs_Header next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>80u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_abb_egm_msgs_EGMChannelState data[16];} roswasmc_bounded_1_abb_egm_msgs_EGMState_egm_channels_sequence;
/** Complete ROS1 abb_egm_msgs/EGMState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_egm_msgs_EGMState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field egm_channels; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_egm_msgs_EGMState_egm_channels_sequence egm_channels;
} roswasmc_bounded_1_abb_egm_msgs_EGMState;
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMSTATE_MAX_WIRE_BYTES 1380u
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMSTATE_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ABB_EGM_MSGS_EGMSTATE_CODEC_IDENTITY "UWVMROS1_f7fe1d3924dc4e497c58aab27da69cfa6ab1a11f64b970b42e2d040a1312c4d8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_egm_msgs_EGMState_valid(roswasmc_bounded_1_abb_egm_msgs_EGMState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->egm_channels.size>16u)return 0;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  if(!roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_valid(&value->egm_channels.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_egm_msgs_EGMState_wire_size(roswasmc_bounded_1_abb_egm_msgs_EGMState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=4;
  if(value->egm_channels.size>16u)return 0;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  n+=roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_wire_size(&value->egm_channels.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_egm_msgs_EGMState_write_fields(roswasmc_bounded_1_abb_egm_msgs_EGMState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->egm_channels.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_write_fields(&value->egm_channels.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_egm_msgs_EGMState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_egm_msgs_EGMState* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->egm_channels.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->egm_channels.size>16u)return 0;
  {size_t i;for(i=0;i<value->egm_channels.size;++i){
  if(!roswasmc_bounded_1_abb_egm_msgs_EGMChannelState_read_fields(p,n,offset,&value->egm_channels.data[i]))return 0;
  }}
  return roswasmc_bounded_1_abb_egm_msgs_EGMState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_egm_msgs_EGMState_encode(roswasmc_bounded_1_abb_egm_msgs_EGMState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_egm_msgs_EGMState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_egm_msgs_EGMState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_egm_msgs_EGMState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_egm_msgs_EGMState_decode(void const* input,size_t length,roswasmc_bounded_1_abb_egm_msgs_EGMState* value) {
  roswasmc_bounded_1_abb_egm_msgs_EGMState next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>1380u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_egm_msgs_EGMState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/ConfData value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_ConfData {
  /** Declared field cf1; canonical capacity is part of CODEC_IDENTITY. */
  float cf1;
  /** Declared field cf4; canonical capacity is part of CODEC_IDENTITY. */
  float cf4;
  /** Declared field cf6; canonical capacity is part of CODEC_IDENTITY. */
  float cf6;
  /** Declared field cfx; canonical capacity is part of CODEC_IDENTITY. */
  float cfx;
} roswasmc_bounded_1_abb_rapid_msgs_ConfData;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_CONFDATA_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_CONFDATA_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_CONFDATA_CODEC_IDENTITY "UWVMROS1_293eb01e8fa150a8a3c741626b76804255d0eb321a0484aac402201447392af2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_ConfData_valid(roswasmc_bounded_1_abb_rapid_msgs_ConfData const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cfx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_ConfData_wire_size(roswasmc_bounded_1_abb_rapid_msgs_ConfData const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_ConfData_write_fields(roswasmc_bounded_1_abb_rapid_msgs_ConfData const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cf6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cfx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_ConfData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_ConfData* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cf1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cf4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cf6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cfx,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_msgs_ConfData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_ConfData_encode(roswasmc_bounded_1_abb_rapid_msgs_ConfData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_ConfData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_ConfData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_ConfData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_ConfData_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_ConfData* value) {
  roswasmc_bounded_1_abb_rapid_msgs_ConfData next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ConfData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/ExtJoint value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_ExtJoint {
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
} roswasmc_bounded_1_abb_rapid_msgs_ExtJoint;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_EXTJOINT_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_EXTJOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_EXTJOINT_CODEC_IDENTITY "UWVMROS1_653a47c44ab85151fd9e6215901fb154962c32fe1082386733ec6205eaae613a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_valid(roswasmc_bounded_1_abb_rapid_msgs_ExtJoint const* value) {
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
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_wire_size(roswasmc_bounded_1_abb_rapid_msgs_ExtJoint const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_write_fields(roswasmc_bounded_1_abb_rapid_msgs_ExtJoint const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_a,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_b,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_c,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_d,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_e,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->eax_f,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_ExtJoint* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_a,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_b,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_c,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_d,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_e,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->eax_f,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_encode(roswasmc_bounded_1_abb_rapid_msgs_ExtJoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_ExtJoint* value) {
  roswasmc_bounded_1_abb_rapid_msgs_ExtJoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/RobJoint value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_RobJoint {
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
} roswasmc_bounded_1_abb_rapid_msgs_RobJoint;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ROBJOINT_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ROBJOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ROBJOINT_CODEC_IDENTITY "UWVMROS1_5d45bc0efae8c4f5b26e4043fe537e163c4cc4c6458d1eac9be429c37aaf82f0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_RobJoint_valid(roswasmc_bounded_1_abb_rapid_msgs_RobJoint const* value) {
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
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_RobJoint_wire_size(roswasmc_bounded_1_abb_rapid_msgs_RobJoint const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_RobJoint_write_fields(roswasmc_bounded_1_abb_rapid_msgs_RobJoint const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rax_6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_RobJoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_RobJoint* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rax_6,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_msgs_RobJoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_RobJoint_encode(roswasmc_bounded_1_abb_rapid_msgs_RobJoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_RobJoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_RobJoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_RobJoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_RobJoint_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_RobJoint* value) {
  roswasmc_bounded_1_abb_rapid_msgs_RobJoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_RobJoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/JointTarget value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_JointTarget {
  /** Declared field robax; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_RobJoint robax;
  /** Declared field extax; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_ExtJoint extax;
} roswasmc_bounded_1_abb_rapid_msgs_JointTarget;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_JOINTTARGET_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_JOINTTARGET_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_JOINTTARGET_CODEC_IDENTITY "UWVMROS1_4a1cc175468da9204c5f2f399b76712617daec21a0d1675a6e73c84ff170dc24"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_JointTarget_valid(roswasmc_bounded_1_abb_rapid_msgs_JointTarget const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_RobJoint_valid(&value->robax))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_valid(&value->extax))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_JointTarget_wire_size(roswasmc_bounded_1_abb_rapid_msgs_JointTarget const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_rapid_msgs_RobJoint_wire_size(&value->robax);
  n+=roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_wire_size(&value->extax);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_JointTarget_write_fields(roswasmc_bounded_1_abb_rapid_msgs_JointTarget const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_rapid_msgs_RobJoint_write_fields(&value->robax,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_write_fields(&value->extax,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_JointTarget_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_JointTarget* value) {
  if(!roswasmc_bounded_1_abb_rapid_msgs_RobJoint_read_fields(p,n,offset,&value->robax))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_read_fields(p,n,offset,&value->extax))return 0;
  return roswasmc_bounded_1_abb_rapid_msgs_JointTarget_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_JointTarget_encode(roswasmc_bounded_1_abb_rapid_msgs_JointTarget const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_JointTarget_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_JointTarget_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_JointTarget_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_JointTarget_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_JointTarget* value) {
  roswasmc_bounded_1_abb_rapid_msgs_JointTarget next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_JointTarget_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/Pos value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_Pos {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
} roswasmc_bounded_1_abb_rapid_msgs_Pos;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_POS_MAX_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_POS_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_POS_CODEC_IDENTITY "UWVMROS1_79edd4553fad8a8f833a3399e354c93538d4971bc3820cc14cf540e960356c01"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_Pos_valid(roswasmc_bounded_1_abb_rapid_msgs_Pos const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_Pos_wire_size(roswasmc_bounded_1_abb_rapid_msgs_Pos const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_Pos_write_fields(roswasmc_bounded_1_abb_rapid_msgs_Pos const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_Pos_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_Pos* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_msgs_Pos_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_Pos_encode(roswasmc_bounded_1_abb_rapid_msgs_Pos const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_Pos_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_Pos_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_Pos_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_Pos_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_Pos* value) {
  roswasmc_bounded_1_abb_rapid_msgs_Pos next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>12u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/Orient value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_Orient {
  /** Declared field q1; canonical capacity is part of CODEC_IDENTITY. */
  float q1;
  /** Declared field q2; canonical capacity is part of CODEC_IDENTITY. */
  float q2;
  /** Declared field q3; canonical capacity is part of CODEC_IDENTITY. */
  float q3;
  /** Declared field q4; canonical capacity is part of CODEC_IDENTITY. */
  float q4;
} roswasmc_bounded_1_abb_rapid_msgs_Orient;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ORIENT_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ORIENT_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ORIENT_CODEC_IDENTITY "UWVMROS1_5a9c29323a5e3bc0c4c0afa7d305738cd52b49ae339e72f6b34b35b165f3d270"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_Orient_valid(roswasmc_bounded_1_abb_rapid_msgs_Orient const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_Orient_wire_size(roswasmc_bounded_1_abb_rapid_msgs_Orient const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_Orient_write_fields(roswasmc_bounded_1_abb_rapid_msgs_Orient const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_Orient_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_Orient* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q4,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_msgs_Orient_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_Orient_encode(roswasmc_bounded_1_abb_rapid_msgs_Orient const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_Orient_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_Orient_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_Orient_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_Orient_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_Orient* value) {
  roswasmc_bounded_1_abb_rapid_msgs_Orient next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/LoadData value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_LoadData {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field cog; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pos cog;
  /** Declared field aom; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Orient aom;
  /** Declared field ix; canonical capacity is part of CODEC_IDENTITY. */
  float ix;
  /** Declared field iy; canonical capacity is part of CODEC_IDENTITY. */
  float iy;
  /** Declared field iz; canonical capacity is part of CODEC_IDENTITY. */
  float iz;
} roswasmc_bounded_1_abb_rapid_msgs_LoadData;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_LOADDATA_MAX_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_LOADDATA_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_LOADDATA_CODEC_IDENTITY "UWVMROS1_2cc985cc188a03e7c6de3d22310802cac1e0a748bae7f828d957436c1e03303d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_LoadData_valid(roswasmc_bounded_1_abb_rapid_msgs_LoadData const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_valid(&value->cog))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_valid(&value->aom))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ix,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iy,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iz,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_LoadData_wire_size(roswasmc_bounded_1_abb_rapid_msgs_LoadData const* value) {size_t n=0;
  n+=4;
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pos_wire_size(&value->cog);
  n+=roswasmc_bounded_1_abb_rapid_msgs_Orient_wire_size(&value->aom);
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_LoadData_write_fields(roswasmc_bounded_1_abb_rapid_msgs_LoadData const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_1_abb_rapid_msgs_Pos_write_fields(&value->cog,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_Orient_write_fields(&value->aom,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ix,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iy,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->iz,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_LoadData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_LoadData* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_read_fields(p,n,offset,&value->cog))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_read_fields(p,n,offset,&value->aom))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ix,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->iy,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->iz,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_msgs_LoadData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_LoadData_encode(roswasmc_bounded_1_abb_rapid_msgs_LoadData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_LoadData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_LoadData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_LoadData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_LoadData_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_LoadData* value) {
  roswasmc_bounded_1_abb_rapid_msgs_LoadData next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>44u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_LoadData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/Pose value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_Pose {
  /** Declared field trans; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pos trans;
  /** Declared field rot; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Orient rot;
} roswasmc_bounded_1_abb_rapid_msgs_Pose;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_POSE_MAX_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_POSE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_POSE_CODEC_IDENTITY "UWVMROS1_3e0cfbb7e25376639d1ff6e555a86e69f8c9ed177c1ef14b301d90b206a19c99"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(roswasmc_bounded_1_abb_rapid_msgs_Pose const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_valid(&value->trans))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_valid(&value->rot))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(roswasmc_bounded_1_abb_rapid_msgs_Pose const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pos_wire_size(&value->trans);
  n+=roswasmc_bounded_1_abb_rapid_msgs_Orient_wire_size(&value->rot);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(roswasmc_bounded_1_abb_rapid_msgs_Pose const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_rapid_msgs_Pos_write_fields(&value->trans,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_Orient_write_fields(&value->rot,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_Pose* value) {
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_read_fields(p,n,offset,&value->trans))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_read_fields(p,n,offset,&value->rot))return 0;
  return roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_Pose_encode(roswasmc_bounded_1_abb_rapid_msgs_Pose const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_Pose_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_Pose* value) {
  roswasmc_bounded_1_abb_rapid_msgs_Pose next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>28u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/RobTarget value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_RobTarget {
  /** Declared field trans; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pos trans;
  /** Declared field rot; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Orient rot;
  /** Declared field robconf; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_ConfData robconf;
  /** Declared field extax; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_ExtJoint extax;
} roswasmc_bounded_1_abb_rapid_msgs_RobTarget;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ROBTARGET_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ROBTARGET_MIN_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_ROBTARGET_CODEC_IDENTITY "UWVMROS1_0abb3aee8b1303ccf9419f2774829d1231de3ac033e4b644d585f195972f09ce"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_RobTarget_valid(roswasmc_bounded_1_abb_rapid_msgs_RobTarget const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_valid(&value->trans))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_valid(&value->rot))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ConfData_valid(&value->robconf))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_valid(&value->extax))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_RobTarget_wire_size(roswasmc_bounded_1_abb_rapid_msgs_RobTarget const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pos_wire_size(&value->trans);
  n+=roswasmc_bounded_1_abb_rapid_msgs_Orient_wire_size(&value->rot);
  n+=roswasmc_bounded_1_abb_rapid_msgs_ConfData_wire_size(&value->robconf);
  n+=roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_wire_size(&value->extax);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_RobTarget_write_fields(roswasmc_bounded_1_abb_rapid_msgs_RobTarget const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_rapid_msgs_Pos_write_fields(&value->trans,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_Orient_write_fields(&value->rot,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_ConfData_write_fields(&value->robconf,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_write_fields(&value->extax,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_RobTarget_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_RobTarget* value) {
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pos_read_fields(p,n,offset,&value->trans))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Orient_read_fields(p,n,offset,&value->rot))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ConfData_read_fields(p,n,offset,&value->robconf))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ExtJoint_read_fields(p,n,offset,&value->extax))return 0;
  return roswasmc_bounded_1_abb_rapid_msgs_RobTarget_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_RobTarget_encode(roswasmc_bounded_1_abb_rapid_msgs_RobTarget const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_RobTarget_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_RobTarget_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_RobTarget_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_RobTarget_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_RobTarget* value) {
  roswasmc_bounded_1_abb_rapid_msgs_RobTarget next={0};size_t offset=0;if(!value||(length&&!input)||length<68u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_RobTarget_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_msgs/ToolData value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_ToolData {
  /** Declared field robhold; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t robhold;
  /** Declared field tframe; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pose tframe;
  /** Declared field tload; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_LoadData tload;
} roswasmc_bounded_1_abb_rapid_msgs_ToolData;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_TOOLDATA_MAX_WIRE_BYTES 73u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_TOOLDATA_MIN_WIRE_BYTES 73u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_TOOLDATA_CODEC_IDENTITY "UWVMROS1_e2feaee4bda891f594542384d70883184262777e359fb037a648b7526ef76cba"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_ToolData_valid(roswasmc_bounded_1_abb_rapid_msgs_ToolData const* value) {
  if(!value)return 0;
  if(value->robhold>1)return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(&value->tframe))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_LoadData_valid(&value->tload))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_ToolData_wire_size(roswasmc_bounded_1_abb_rapid_msgs_ToolData const* value) {size_t n=0;
  n+=1;
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(&value->tframe);
  n+=roswasmc_bounded_1_abb_rapid_msgs_LoadData_wire_size(&value->tload);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_ToolData_write_fields(roswasmc_bounded_1_abb_rapid_msgs_ToolData const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->robhold,1);*offset+=1;
  roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(&value->tframe,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_LoadData_write_fields(&value->tload,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_ToolData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_ToolData* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->robhold,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields(p,n,offset,&value->tframe))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_LoadData_read_fields(p,n,offset,&value->tload))return 0;
  return roswasmc_bounded_1_abb_rapid_msgs_ToolData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_ToolData_encode(roswasmc_bounded_1_abb_rapid_msgs_ToolData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_ToolData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_ToolData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_ToolData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_ToolData_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_ToolData* value) {
  roswasmc_bounded_1_abb_rapid_msgs_ToolData next={0};size_t offset=0;if(!value||(length&&!input)||length<73u||length>73u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ToolData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_msgs/WObjData.ufmec; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_rapid_msgs_WObjData_ufmec_text;
/** Complete ROS1 abb_rapid_msgs/WObjData value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_msgs_WObjData {
  /** Declared field robhold; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t robhold;
  /** Declared field ufprog; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t ufprog;
  /** Declared field ufmec; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_WObjData_ufmec_text ufmec;
  /** Declared field uframe; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pose uframe;
  /** Declared field oframe; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pose oframe;
} roswasmc_bounded_1_abb_rapid_msgs_WObjData;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_WOBJDATA_MAX_WIRE_BYTES 574u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_WOBJDATA_MIN_WIRE_BYTES 62u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_MSGS_WOBJDATA_CODEC_IDENTITY "UWVMROS1_6de4cf654a15ef3b63d9b0f3d0984438f21963c10ffd1b00238113b6fae8454f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_WObjData_valid(roswasmc_bounded_1_abb_rapid_msgs_WObjData const* value) {
  if(!value)return 0;
  if(value->robhold>1)return 0;
  if(value->ufprog>1)return 0;
  if(value->ufmec.size>512u||!uwvm_bounded_utf8(value->ufmec.data,value->ufmec.size))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(&value->uframe))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(&value->oframe))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_msgs_WObjData_wire_size(roswasmc_bounded_1_abb_rapid_msgs_WObjData const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=4+value->ufmec.size;
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(&value->uframe);
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(&value->oframe);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_msgs_WObjData_write_fields(roswasmc_bounded_1_abb_rapid_msgs_WObjData const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->robhold,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->ufprog,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->ufmec.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->ufmec.data,value->ufmec.size);*offset+=value->ufmec.size;
  roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(&value->uframe,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(&value->oframe,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_msgs_WObjData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_msgs_WObjData* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->robhold,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->ufprog,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->ufmec.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->ufmec.size>512u||value->ufmec.size>n-*offset)return 0;uwvm_bounded_copy(value->ufmec.data,p+*offset,value->ufmec.size);*offset+=value->ufmec.size;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields(p,n,offset,&value->uframe))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields(p,n,offset,&value->oframe))return 0;
  return roswasmc_bounded_1_abb_rapid_msgs_WObjData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_WObjData_encode(roswasmc_bounded_1_abb_rapid_msgs_WObjData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_msgs_WObjData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_msgs_WObjData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_msgs_WObjData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_msgs_WObjData_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_msgs_WObjData* value) {
  roswasmc_bounded_1_abb_rapid_msgs_WObjData next={0};size_t offset=0;if(!value||(length&&!input)||length<62u||length>574u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_msgs_WObjData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMActivateSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings {
  /** Declared field tool; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_ToolData tool;
  /** Declared field wobj; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_WObjData wobj;
  /** Declared field correction_frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pose correction_frame;
  /** Declared field sensor_frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pose sensor_frame;
  /** Declared field cond_min_max; canonical capacity is part of CODEC_IDENTITY. */
  float cond_min_max;
  /** Declared field lp_filter; canonical capacity is part of CODEC_IDENTITY. */
  float lp_filter;
  /** Declared field sample_rate; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t sample_rate;
  /** Declared field max_speed_deviation; canonical capacity is part of CODEC_IDENTITY. */
  float max_speed_deviation;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMACTIVATESETTINGS_MAX_WIRE_BYTES 716u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMACTIVATESETTINGS_MIN_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMACTIVATESETTINGS_CODEC_IDENTITY "UWVMROS1_c50be5682021e5d1a7a938e6fe3595cec7c0b0ae727070c0924da731c2c73d4d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_ToolData_valid(&value->tool))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_WObjData_valid(&value->wobj))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(&value->correction_frame))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(&value->sensor_frame))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_min_max,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lp_filter,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->max_speed_deviation,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_rapid_msgs_ToolData_wire_size(&value->tool);
  n+=roswasmc_bounded_1_abb_rapid_msgs_WObjData_wire_size(&value->wobj);
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(&value->correction_frame);
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(&value->sensor_frame);
  n+=4;
  n+=4;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_rapid_msgs_ToolData_write_fields(&value->tool,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_WObjData_write_fields(&value->wobj,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(&value->correction_frame,p,offset);
  roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(&value->sensor_frame,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_min_max,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lp_filter,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->sample_rate,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->max_speed_deviation,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings* value) {
  if(!roswasmc_bounded_1_abb_rapid_msgs_ToolData_read_fields(p,n,offset,&value->tool))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_WObjData_read_fields(p,n,offset,&value->wobj))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields(p,n,offset,&value->correction_frame))return 0;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields(p,n,offset,&value->sensor_frame))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cond_min_max,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->lp_filter,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->sample_rate,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->max_speed_deviation,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<204u||length>716u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMRunSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings {
  /** Declared field cond_time; canonical capacity is part of CODEC_IDENTITY. */
  float cond_time;
  /** Declared field ramp_in_time; canonical capacity is part of CODEC_IDENTITY. */
  float ramp_in_time;
  /** Declared field offset; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_msgs_Pose offset;
  /** Declared field pos_corr_gain; canonical capacity is part of CODEC_IDENTITY. */
  float pos_corr_gain;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMRUNSETTINGS_MAX_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMRUNSETTINGS_MIN_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMRUNSETTINGS_CODEC_IDENTITY "UWVMROS1_09a843da7aa89c60e6404f8d36b9a1a57f75e330329ac043449fd438b7029506"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_in_time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_valid(&value->offset))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pos_corr_gain,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=roswasmc_bounded_1_abb_rapid_msgs_Pose_wire_size(&value->offset);
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cond_time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_in_time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_1_abb_rapid_msgs_Pose_write_fields(&value->offset,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pos_corr_gain,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cond_time,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ramp_in_time,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_1_abb_rapid_msgs_Pose_read_fields(p,n,offset,&value->offset))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pos_corr_gain,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<40u||length>40u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMSetupSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings {
  /** Declared field use_filtering; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_filtering;
  /** Declared field comm_timeout; canonical capacity is part of CODEC_IDENTITY. */
  float comm_timeout;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSETUPSETTINGS_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSETUPSETTINGS_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSETUPSETTINGS_CODEC_IDENTITY "UWVMROS1_ece31a7631bb3bb248aece52d4d5f6e190a94dcc3052b2761b759bed24513779"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings const* value) {
  if(!value)return 0;
  if(value->use_filtering>1)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->comm_timeout,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings const* value) {size_t n=0;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_filtering,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->comm_timeout,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_filtering,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->comm_timeout,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMStopSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings {
  /** Declared field ramp_out_time; canonical capacity is part of CODEC_IDENTITY. */
  float ramp_out_time;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSTOPSETTINGS_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSTOPSETTINGS_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSTOPSETTINGS_CODEC_IDENTITY "UWVMROS1_64009cc452b9b2b12b3501d150e8c88c9cc07c796df06775195fa17c2a103a08"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_out_time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ramp_out_time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ramp_out_time,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_rapid_sm_addin_msgs/EGMSettings value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings {
  /** Declared field allow_egm_motions; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t allow_egm_motions;
  /** Declared field use_presync; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_presync;
  /** Declared field setup_uc; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings setup_uc;
  /** Declared field activate; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings activate;
  /** Declared field run; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings run;
  /** Declared field stop; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings stop;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSETTINGS_MAX_WIRE_BYTES 767u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSETTINGS_MIN_WIRE_BYTES 255u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_EGMSETTINGS_CODEC_IDENTITY "UWVMROS1_ebc0ca24a54451f863964c74bf0257b5b25546d0d4b8a75ac75c265e1d62317b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings const* value) {
  if(!value)return 0;
  if(value->allow_egm_motions>1)return 0;
  if(value->use_presync>1)return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_valid(&value->setup_uc))return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_valid(&value->activate))return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_valid(&value->run))return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_valid(&value->stop))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_wire_size(&value->setup_uc);
  n+=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_wire_size(&value->activate);
  n+=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_wire_size(&value->run);
  n+=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_wire_size(&value->stop);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->allow_egm_motions,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_presync,1);*offset+=1;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_write_fields(&value->setup_uc,p,offset);
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_write_fields(&value->activate,p,offset);
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_write_fields(&value->run,p,offset);
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_write_fields(&value->stop,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->allow_egm_motions,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_presync,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSetupSettings_read_fields(p,n,offset,&value->setup_uc))return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMActivateSettings_read_fields(p,n,offset,&value->activate))return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMRunSettings_read_fields(p,n,offset,&value->run))return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMStopSettings_read_fields(p,n,offset,&value->stop))return 0;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings next={0};size_t offset=0;if(!value||(length&&!input)||length<255u||length>767u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/GetEGMSettingsRequest.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_task_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/GetEGMSettingsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_task_text task;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_GETEGMSETTINGSREQUEST_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_GETEGMSETTINGSREQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_GETEGMSETTINGSREQUEST_CODEC_IDENTITY "UWVMROS1_42cca857367e08720f989416d18d6a1f7d677f7e5bf49435b9735a9eb10a8f9c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest const* value) {size_t n=0;
  n+=4+value->task.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/GetEGMSettingsResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_message_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/GetEGMSettingsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse {
  /** Declared field settings; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings settings;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_message_text message;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_GETEGMSETTINGSRESPONSE_MAX_WIRE_BYTES 1285u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_GETEGMSETTINGSRESPONSE_MIN_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_GETEGMSETTINGSRESPONSE_CODEC_IDENTITY "UWVMROS1_094fd8b0b992c26d708bbd09952d36a3921ad5e5e66f45edb9d19af5ec3472bc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_valid(&value->settings))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_wire_size(&value->settings);
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_write_fields(&value->settings,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse* value) {
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_read_fields(p,n,offset,&value->settings))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<261u||length>1285u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_GetEGMSettingsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/StateMachineState.rapid_task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_rapid_task_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/StateMachineState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState {
  /** Declared field rapid_task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_rapid_task_text rapid_task;
  /** Declared field sm_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t sm_state;
  /** Declared field egm_action; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t egm_action;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_CODEC_IDENTITY "UWVMROS1_9ef8cc3a0438ad892acc1a2f485039ca49abce6fedf40cc5d9b2fe03dd9490bb"
/** Original scalar constant SM_STATE_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_SM_STATE_UNKNOWN UINT32_C(1)
/** Original scalar constant SM_STATE_IDLE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_SM_STATE_IDLE UINT32_C(2)
/** Original scalar constant SM_STATE_INITIALIZE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_SM_STATE_INITIALIZE UINT32_C(3)
/** Original scalar constant SM_STATE_RUN_RAPID_ROUTINE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_SM_STATE_RUN_RAPID_ROUTINE UINT32_C(4)
/** Original scalar constant SM_STATE_RUN_EGM_ROUTINE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_SM_STATE_RUN_EGM_ROUTINE UINT32_C(5)
/** Original scalar constant EGM_ACTION_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_EGM_ACTION_UNKNOWN UINT32_C(1)
/** Original scalar constant EGM_ACTION_NONE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_EGM_ACTION_NONE UINT32_C(2)
/** Original scalar constant EGM_ACTION_RUN_JOINT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_EGM_ACTION_RUN_JOINT UINT32_C(3)
/** Original scalar constant EGM_ACTION_RUN_POSE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_EGM_ACTION_RUN_POSE UINT32_C(4)
/** Original scalar constant EGM_ACTION_STOP=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_EGM_ACTION_STOP UINT32_C(5)
/** Original scalar constant EGM_ACTION_START_STREAM=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_EGM_ACTION_START_STREAM UINT32_C(6)
/** Original scalar constant EGM_ACTION_STOP_STREAM=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_STATEMACHINESTATE_EGM_ACTION_STOP_STREAM UINT32_C(7)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState const* value) {
  if(!value)return 0;
  if(value->rapid_task.size>512u||!uwvm_bounded_utf8(value->rapid_task.data,value->rapid_task.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState const* value) {size_t n=0;
  n+=4+value->rapid_task.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->rapid_task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->rapid_task.data,value->rapid_task.size);*offset+=value->rapid_task.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sm_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->egm_action,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState* value) {
  if(*offset>n||n-*offset<4)return 0;value->rapid_task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->rapid_task.size>512u||value->rapid_task.size>n-*offset)return 0;uwvm_bounded_copy(value->rapid_task.data,p+*offset,value->rapid_task.size);*offset+=value->rapid_task.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->sm_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->egm_action,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState data[16];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_state_machines_sequence;
/** Complete ROS1 abb_rapid_sm_addin_msgs/RuntimeState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field state_machines; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_state_machines_sequence state_machines;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_RUNTIMESTATE_MAX_WIRE_BYTES 8372u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_RUNTIMESTATE_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_RUNTIMESTATE_CODEC_IDENTITY "UWVMROS1_50d7eab2eed3ce0b55160e85b44b4108d966290f6b8f126980ee452846f3660c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->state_machines.size>16u)return 0;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_valid(&value->state_machines.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=4;
  if(value->state_machines.size>16u)return 0;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  n+=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_wire_size(&value->state_machines.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->state_machines.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_write_fields(&value->state_machines.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->state_machines.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state_machines.size>16u)return 0;
  {size_t i;for(i=0;i<value->state_machines.size;++i){
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_StateMachineState_read_fields(p,n,offset,&value->state_machines.data[i]))return 0;
  }}
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>8372u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_RuntimeState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/SetEGMSettingsRequest.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_task_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetEGMSettingsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_task_text task;
  /** Declared field settings; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings settings;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETEGMSETTINGSREQUEST_MAX_WIRE_BYTES 835u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETEGMSETTINGSREQUEST_MIN_WIRE_BYTES 259u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETEGMSETTINGSREQUEST_CODEC_IDENTITY "UWVMROS1_fe85c34f8dff2c0a547f89385d75771af13a03d997b880ab7be2eb84ea800155"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_valid(&value->settings))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_wire_size(&value->settings);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_write_fields(&value->settings,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_EGMSettings_read_fields(p,n,offset,&value->settings))return 0;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<259u||length>835u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/SetEGMSettingsResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_message_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetEGMSettingsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_message_text message;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETEGMSETTINGSRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETEGMSETTINGSRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETEGMSETTINGSRESPONSE_CODEC_IDENTITY "UWVMROS1_c0d4e594a51a6cdd245ad03b6e27bbdc298c5c9fbe26cc25fa3594deef9b2591"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetEGMSettingsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_task_text;
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest.routine; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_routine_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetRAPIDRoutineRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_task_text task;
  /** Declared field routine; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_routine_text routine;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETRAPIDROUTINEREQUEST_MAX_WIRE_BYTES 584u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETRAPIDROUTINEREQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETRAPIDROUTINEREQUEST_CODEC_IDENTITY "UWVMROS1_b16ba3e98ee1d45864532b01f914746f041e49463b9330ef6056e427d199a5a3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  if(value->routine.size>512u||!uwvm_bounded_utf8(value->routine.data,value->routine.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=4+value->routine.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  uwvm_bounded_put(p+*offset,value->routine.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->routine.data,value->routine.size);*offset+=value->routine.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(*offset>n||n-*offset<4)return 0;value->routine.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->routine.size>512u||value->routine.size>n-*offset)return 0;uwvm_bounded_copy(value->routine.data,p+*offset,value->routine.size);*offset+=value->routine.size;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>584u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/SetRAPIDRoutineResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_message_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetRAPIDRoutineResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_message_text message;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETRAPIDROUTINERESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETRAPIDROUTINERESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETRAPIDROUTINERESPONSE_CODEC_IDENTITY "UWVMROS1_44de2385f12a8612fa75ca7c3f97b07f22d04e920bdeca0f0bd7fed5322c680e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetRAPIDRoutineResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/SetSGCommandRequest.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_task_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetSGCommandRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_task_text task;
  /** Declared field command; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t command;
  /** Declared field target_position; canonical capacity is part of CODEC_IDENTITY. */
  float target_position;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_MAX_WIRE_BYTES 73u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_CODEC_IDENTITY "UWVMROS1_e39d7fee66c1efb9ea963216823f6e10bd632d78634d8c47f3283a1a1779f0c2"
/** Original scalar constant SG_COMMAND_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_UNKNOWN UINT32_C(1)
/** Original scalar constant SG_COMMAND_NONE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_NONE UINT32_C(2)
/** Original scalar constant SG_COMMAND_INITIALIZE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_INITIALIZE UINT32_C(3)
/** Original scalar constant SG_COMMAND_CALIBRATE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_CALIBRATE UINT32_C(4)
/** Original scalar constant SG_COMMAND_MOVE_TO=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_MOVE_TO UINT32_C(5)
/** Original scalar constant SG_COMMAND_GRIP_IN=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_GRIP_IN UINT32_C(6)
/** Original scalar constant SG_COMMAND_GRIP_OUT=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_GRIP_OUT UINT32_C(7)
/** Original scalar constant SG_COMMAND_BLOW_ON_1=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_BLOW_ON_1 UINT32_C(8)
/** Original scalar constant SG_COMMAND_BLOW_ON_2=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_BLOW_ON_2 UINT32_C(9)
/** Original scalar constant SG_COMMAND_BLOW_OFF_1=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_BLOW_OFF_1 UINT32_C(10)
/** Original scalar constant SG_COMMAND_BLOW_OFF_2=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_BLOW_OFF_2 UINT32_C(11)
/** Original scalar constant SG_COMMAND_VACUUM_ON_1=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_VACUUM_ON_1 UINT32_C(12)
/** Original scalar constant SG_COMMAND_VACUUM_ON_2=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_VACUUM_ON_2 UINT32_C(13)
/** Original scalar constant SG_COMMAND_VACUUM_OFF_1=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_VACUUM_OFF_1 UINT32_C(14)
/** Original scalar constant SG_COMMAND_VACUUM_OFF_2=15; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDREQUEST_SG_COMMAND_VACUUM_OFF_2 UINT32_C(15)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->target_position,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->command,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->target_position,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->command,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->target_position,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>73u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_rapid_sm_addin_msgs/SetSGCommandResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_message_text;
/** Complete ROS1 abb_rapid_sm_addin_msgs/SetSGCommandResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_message_text message;
} roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse;
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_RAPID_SM_ADDIN_MSGS_SETSGCOMMANDRESPONSE_CODEC_IDENTITY "UWVMROS1_2f22b2098d86dd0764277247eb7f64dccecebf0cddce85654b01c1928eb9c192"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_valid(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_wire_size(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_write_fields(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_encode(roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse* value) {
  roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_rapid_sm_addin_msgs_SetSGCommandResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetFileContentsRequest.filename; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_filename_text;
/** Complete ROS1 abb_robot_msgs/GetFileContentsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest {
  /** Declared field filename; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_filename_text filename;
} roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETFILECONTENTSREQUEST_MAX_WIRE_BYTES 516u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETFILECONTENTSREQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETFILECONTENTSREQUEST_CODEC_IDENTITY "UWVMROS1_75f5802b385cc1efb524fadea657fda1d5cc4298c5720222d33cd3dd39656f21"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest const* value) {
  if(!value)return 0;
  if(value->filename.size>512u||!uwvm_bounded_utf8(value->filename.data,value->filename.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest const* value) {size_t n=0;
  n+=4+value->filename.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->filename.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->filename.data,value->filename.size);*offset+=value->filename.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->filename.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->filename.size>512u||value->filename.size>n-*offset)return 0;uwvm_bounded_copy(value->filename.data,p+*offset,value->filename.size);*offset+=value->filename.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>516u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetFileContentsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetFileContentsResponse.contents; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_contents_text;
/** UTF-8 byte storage for abb_robot_msgs/GetFileContentsResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetFileContentsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse {
  /** Declared field contents; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_contents_text contents;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETFILECONTENTSRESPONSE_MAX_WIRE_BYTES 4618u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETFILECONTENTSRESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETFILECONTENTSRESPONSE_CODEC_IDENTITY "UWVMROS1_693b53ed168f81a7c93ac8083977c4f8bd6f4a3999d079f5b47029699ba68fdb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse const* value) {
  if(!value)return 0;
  if(value->contents.size>4096u||!uwvm_bounded_utf8(value->contents.data,value->contents.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse const* value) {size_t n=0;
  n+=4+value->contents.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->contents.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->contents.data,value->contents.size);*offset+=value->contents.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->contents.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->contents.size>4096u||value->contents.size>n-*offset)return 0;uwvm_bounded_copy(value->contents.data,p+*offset,value->contents.size);*offset+=value->contents.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>4618u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetFileContentsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetIOSignalRequest.signal; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_signal_text;
/** Complete ROS1 abb_robot_msgs/GetIOSignalRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest {
  /** Declared field signal; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_signal_text signal;
} roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETIOSIGNALREQUEST_MAX_WIRE_BYTES 516u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETIOSIGNALREQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETIOSIGNALREQUEST_CODEC_IDENTITY "UWVMROS1_d98f7577756b476cac0dd325d45f333fcd2b735df7dce58fb660790b5a91f83b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest const* value) {
  if(!value)return 0;
  if(value->signal.size>512u||!uwvm_bounded_utf8(value->signal.data,value->signal.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest const* value) {size_t n=0;
  n+=4+value->signal.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->signal.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->signal.data,value->signal.size);*offset+=value->signal.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->signal.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->signal.size>512u||value->signal.size>n-*offset)return 0;uwvm_bounded_copy(value->signal.data,p+*offset,value->signal.size);*offset+=value->signal.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>516u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetIOSignalRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetIOSignalResponse.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_value_text;
/** UTF-8 byte storage for abb_robot_msgs/GetIOSignalResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetIOSignalResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_value_text value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETIOSIGNALRESPONSE_MAX_WIRE_BYTES 1034u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETIOSIGNALRESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETIOSIGNALRESPONSE_CODEC_IDENTITY "UWVMROS1_10a66146c4815b0d758ec81dde1c552695c97a8727ee995299018b23f129d79e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse const* value) {
  if(!value)return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse const* value) {size_t n=0;
  n+=4+value->value.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>1034u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetIOSignalResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/RAPIDSymbolPath.task; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_task_text;
/** UTF-8 byte storage for abb_robot_msgs/RAPIDSymbolPath.module; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_module_text;
/** UTF-8 byte storage for abb_robot_msgs/RAPIDSymbolPath.symbol; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_symbol_text;
/** Complete ROS1 abb_robot_msgs/RAPIDSymbolPath value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath {
  /** Declared field task; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_task_text task;
  /** Declared field module; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_module_text module;
  /** Declared field symbol; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_symbol_text symbol;
} roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDSYMBOLPATH_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDSYMBOLPATH_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDSYMBOLPATH_CODEC_IDENTITY "UWVMROS1_07308d9ce1e71a02f5cc3546cdbaaa0b24a06534a55aa3ee3354b498038faad2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath const* value) {
  if(!value)return 0;
  if(value->task.size>64u||!uwvm_bounded_utf8(value->task.data,value->task.size))return 0;
  if(value->module.size>64u||!uwvm_bounded_utf8(value->module.data,value->module.size))return 0;
  if(value->symbol.size>64u||!uwvm_bounded_utf8(value->symbol.data,value->symbol.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath const* value) {size_t n=0;
  n+=4+value->task.size;
  n+=4+value->module.size;
  n+=4+value->symbol.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->task.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->task.data,value->task.size);*offset+=value->task.size;
  uwvm_bounded_put(p+*offset,value->module.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->module.data,value->module.size);*offset+=value->module.size;
  uwvm_bounded_put(p+*offset,value->symbol.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->symbol.data,value->symbol.size);*offset+=value->symbol.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath* value) {
  if(*offset>n||n-*offset<4)return 0;value->task.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->task.size>64u||value->task.size>n-*offset)return 0;uwvm_bounded_copy(value->task.data,p+*offset,value->task.size);*offset+=value->task.size;
  if(*offset>n||n-*offset<4)return 0;value->module.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->module.size>64u||value->module.size>n-*offset)return 0;uwvm_bounded_copy(value->module.data,p+*offset,value->module.size);*offset+=value->module.size;
  if(*offset>n||n-*offset<4)return 0;value->symbol.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->symbol.size>64u||value->symbol.size>n-*offset)return 0;uwvm_bounded_copy(value->symbol.data,p+*offset,value->symbol.size);*offset+=value->symbol.size;
  return roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_encode(roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath* value) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/GetRAPIDBoolRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDBOOLREQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDBOOLREQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDBOOLREQUEST_CODEC_IDENTITY "UWVMROS1_46e49c0d6796175d0c56460dd4c7d2557deaa8db4f1a3bec40b53636d3b44b96"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetRAPIDBoolResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetRAPIDBoolResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDBOOLRESPONSE_MAX_WIRE_BYTES 519u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDBOOLRESPONSE_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDBOOLRESPONSE_CODEC_IDENTITY "UWVMROS1_bfe21e95cb79f1d9bc1bc1dc2f61c744a2a7dc49e5a5ed07b6c8619344b97b3f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse const* value) {
  if(!value)return 0;
  if(value->value>1)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse const* value) {size_t n=0;
  n+=1;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>519u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDBoolResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/GetRAPIDDnumRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDDNUMREQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDDNUMREQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDDNUMREQUEST_CODEC_IDENTITY "UWVMROS1_9058d8774de0da657344a00d75a7f75878da2b0453bf25708bc8ed7f45c90c9d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetRAPIDDnumResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetRAPIDDnumResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  double value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDDNUMRESPONSE_MAX_WIRE_BYTES 526u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDDNUMRESPONSE_MIN_WIRE_BYTES 14u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDDNUMRESPONSE_CODEC_IDENTITY "UWVMROS1_9c209b4b12d733e33b76cc33ae93b6d561ce1eb7f8864429a56bd073317a01b3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse const* value) {size_t n=0;
  n+=8;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->value,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<14u||length>526u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDDnumResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/GetRAPIDNumRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDNUMREQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDNUMREQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDNUMREQUEST_CODEC_IDENTITY "UWVMROS1_a316395c8a19b7c0a9c3a555e625fffc7d647c7b567f38c53b1c3735b50ee910"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetRAPIDNumResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetRAPIDNumResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDNUMRESPONSE_MAX_WIRE_BYTES 522u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDNUMRESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDNUMRESPONSE_CODEC_IDENTITY "UWVMROS1_1573cba9a9046a6b304a49e3a77266fa0d6f615d2ea9a24692c25616970fd9b0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse const* value) {size_t n=0;
  n+=4;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>522u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDNumResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/GetRAPIDStringRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSTRINGREQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSTRINGREQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSTRINGREQUEST_CODEC_IDENTITY "UWVMROS1_56514142e9e064aa43d09078b49ed8ee0366189727c35a9bc130b35007da4c9c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetRAPIDStringResponse.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_value_text;
/** UTF-8 byte storage for abb_robot_msgs/GetRAPIDStringResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetRAPIDStringResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_value_text value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSTRINGRESPONSE_MAX_WIRE_BYTES 1034u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSTRINGRESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSTRINGRESPONSE_CODEC_IDENTITY "UWVMROS1_d1188827af3c307a721f8128b6f14fa6020f3187afce8a4162ce6562cdd1c854"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse const* value) {
  if(!value)return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse const* value) {size_t n=0;
  n+=4+value->value.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>1034u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDStringResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/GetRAPIDSymbolRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSYMBOLREQUEST_MAX_WIRE_BYTES 204u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSYMBOLREQUEST_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSYMBOLREQUEST_CODEC_IDENTITY "UWVMROS1_31302ad912a8ff39b56a429bc9dae22cbfc2c2101b8cd05162d00d9206aecf5b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>204u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetRAPIDSymbolResponse.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_value_text;
/** UTF-8 byte storage for abb_robot_msgs/GetRAPIDSymbolResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetRAPIDSymbolResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_value_text value;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSYMBOLRESPONSE_MAX_WIRE_BYTES 1034u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSYMBOLRESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETRAPIDSYMBOLRESPONSE_CODEC_IDENTITY "UWVMROS1_182dc465ea5fb9543791b6fb76520313cee1ed472ab0296c8ef7a9ed3d4d110f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse const* value) {
  if(!value)return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse const* value) {size_t n=0;
  n+=4+value->value.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>1034u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRAPIDSymbolResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/GetRobotControllerDescriptionRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETROBOTCONTROLLERDESCRIPTIONREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETROBOTCONTROLLERDESCRIPTIONREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETROBOTCONTROLLERDESCRIPTIONREQUEST_CODEC_IDENTITY "UWVMROS1_bc5f87f4d0f33cbc640c8188fbb15c9b0f43b3067f2406700d6f5bd2475089b6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest* value) {
  return roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetRobotControllerDescriptionResponse.description; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_description_text;
/** UTF-8 byte storage for abb_robot_msgs/GetRobotControllerDescriptionResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetRobotControllerDescriptionResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse {
  /** Declared field description; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_description_text description;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETROBOTCONTROLLERDESCRIPTIONRESPONSE_MAX_WIRE_BYTES 4618u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETROBOTCONTROLLERDESCRIPTIONRESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETROBOTCONTROLLERDESCRIPTIONRESPONSE_CODEC_IDENTITY "UWVMROS1_7db41283fbe12a010e526b87d77a5bb7faecdf85595ef2dc13f14bfed56b6e39"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse const* value) {
  if(!value)return 0;
  if(value->description.size>4096u||!uwvm_bounded_utf8(value->description.data,value->description.size))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse const* value) {size_t n=0;
  n+=4+value->description.size;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->description.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->description.data,value->description.size);*offset+=value->description.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->description.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->description.size>4096u||value->description.size>n-*offset)return 0;uwvm_bounded_copy(value->description.data,p+*offset,value->description.size);*offset+=value->description.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>4618u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetRobotControllerDescriptionResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/GetSpeedRatioRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETSPEEDRATIOREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETSPEEDRATIOREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETSPEEDRATIOREQUEST_CODEC_IDENTITY "UWVMROS1_82230aebf6fa923e09bcd413ecb3b4803a6f1cfd3e8334e0ce0c496b73008373"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_valid(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest* value) {
  return roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_encode(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/GetSpeedRatioResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_message_text;
/** Complete ROS1 abb_robot_msgs/GetSpeedRatioResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse {
  /** Declared field speed_ratio; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t speed_ratio;
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETSPEEDRATIORESPONSE_MAX_WIRE_BYTES 519u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETSPEEDRATIORESPONSE_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_GETSPEEDRATIORESPONSE_CODEC_IDENTITY "UWVMROS1_7784360ddafa1b8ad9626fde0afa2b2335f6a294507036c3aee953df99e91940"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_valid(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse const* value) {size_t n=0;
  n+=1;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed_ratio,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed_ratio,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_encode(roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>519u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_GetSpeedRatioResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/MechanicalUnitState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_name_text;
/** Complete ROS1 abb_robot_msgs/MechanicalUnitState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_name_text name;
  /** Declared field activated; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t activated;
} roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_MECHANICALUNITSTATE_MAX_WIRE_BYTES 69u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_MECHANICALUNITSTATE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_MECHANICALUNITSTATE_CODEC_IDENTITY "UWVMROS1_872cc6e8bdf32426bb9e24322c951452aab6ebcbc1f2998f4fb19c34abece8fa"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_valid(roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  if(value->activated>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_wire_size(roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_write_fields(roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->activated,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->activated,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_encode(roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState* value) {
  roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>69u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/RAPIDTaskState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_name_text;
/** Complete ROS1 abb_robot_msgs/RAPIDTaskState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_name_text name;
  /** Declared field activated; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t activated;
  /** Declared field execution_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t execution_state;
  /** Declared field motion_task; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t motion_task;
} roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_MAX_WIRE_BYTES 71u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_CODEC_IDENTITY "UWVMROS1_ab1245cc352efa91cdcc8de801392857e25a4101a141439554b761525b31b947"
/** Original scalar constant EXECUTION_STATE_UNKNOWN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_EXECUTION_STATE_UNKNOWN UINT32_C(1)
/** Original scalar constant EXECUTION_STATE_READY=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_EXECUTION_STATE_READY UINT32_C(2)
/** Original scalar constant EXECUTION_STATE_STOPPED=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_EXECUTION_STATE_STOPPED UINT32_C(3)
/** Original scalar constant EXECUTION_STATE_STARTED=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_EXECUTION_STATE_STARTED UINT32_C(4)
/** Original scalar constant EXECUTION_STATE_UNINITIALIZED=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_RAPIDTASKSTATE_EXECUTION_STATE_UNINITIALIZED UINT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_valid(roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  if(value->activated>1)return 0;
  if(value->motion_task>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_wire_size(roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_write_fields(roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->activated,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->execution_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->motion_task,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->activated,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->execution_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->motion_task,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_encode(roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState* value) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>71u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/ServiceResponses value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_ServiceResponses {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_abb_robot_msgs_ServiceResponses;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_CODEC_IDENTITY "UWVMROS1_f0a3c4b160bf0ca52dbbb4e26d2c8a7d3c6a40ac70787ba59cf181945e4276d4"
/** Original scalar constant SUCCESS=""; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_SUCCESS ""
/** Original scalar constant RC_SUCCESS=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_SUCCESS UINT32_C(1)
/** Original scalar constant FAILED="Service failed (e.g. timeout or resource not found, see the DEBUG log for details)"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_FAILED "\123\145\162\166\151\143\145\040\146\141\151\154\145\144\040\050\145\056\147\056\040\164\151\155\145\157\165\164\040\157\162\040\162\145\163\157\165\162\143\145\040\156\157\164\040\146\157\165\156\144\054\040\163\145\145\040\164\150\145\040\104\105\102\125\107\040\154\157\147\040\146\157\162\040\144\145\164\141\151\154\163\051"
/** Original scalar constant RC_FAILED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_FAILED UINT32_C(2)
/** Original scalar constant SERVER_IS_BUSY="Server is busy"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_SERVER_IS_BUSY "\123\145\162\166\145\162\040\151\163\040\142\165\163\171"
/** Original scalar constant RC_SERVER_IS_BUSY=1001; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_SERVER_IS_BUSY UINT32_C(1001)
/** Original scalar constant EMPTY_FILENAME="Filename is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_EMPTY_FILENAME "\106\151\154\145\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_FILENAME=2001; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_EMPTY_FILENAME UINT32_C(2001)
/** Original scalar constant EMPTY_SIGNAL_NAME="Signal name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_EMPTY_SIGNAL_NAME "\123\151\147\156\141\154\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_SIGNAL_NAME=2002; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_EMPTY_SIGNAL_NAME UINT32_C(2002)
/** Original scalar constant EMPTY_RAPID_TASK_NAME="RAPID task name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_EMPTY_RAPID_TASK_NAME "\122\101\120\111\104\040\164\141\163\153\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_RAPID_TASK_NAME=2003; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_EMPTY_RAPID_TASK_NAME UINT32_C(2003)
/** Original scalar constant EMPTY_RAPID_MODULE_NAME="RAPID module name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_EMPTY_RAPID_MODULE_NAME "\122\101\120\111\104\040\155\157\144\165\154\145\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_RAPID_MODULE_NAME=2004; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_EMPTY_RAPID_MODULE_NAME UINT32_C(2004)
/** Original scalar constant EMPTY_RAPID_SYMBOL_NAME="RAPID symbol name is empty"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_EMPTY_RAPID_SYMBOL_NAME "\122\101\120\111\104\040\163\171\155\142\157\154\040\156\141\155\145\040\151\163\040\145\155\160\164\171"
/** Original scalar constant RC_EMPTY_RAPID_SYMBOL_NAME=2005; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_EMPTY_RAPID_SYMBOL_NAME UINT32_C(2005)
/** Original scalar constant NOT_IN_AUTO_MODE="Robot controller is not in AUTO mode"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_NOT_IN_AUTO_MODE "\122\157\142\157\164\040\143\157\156\164\162\157\154\154\145\162\040\151\163\040\156\157\164\040\151\156\040\101\125\124\117\040\155\157\144\145"
/** Original scalar constant RC_NOT_IN_AUTO_MODE=3001; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_NOT_IN_AUTO_MODE UINT32_C(3001)
/** Original scalar constant MOTORS_ARE_OFF="Motors are off"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_MOTORS_ARE_OFF "\115\157\164\157\162\163\040\141\162\145\040\157\146\146"
/** Original scalar constant RC_MOTORS_ARE_OFF=3002; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_MOTORS_ARE_OFF UINT32_C(3002)
/** Original scalar constant MOTORS_ARE_ON="Motors are on"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_MOTORS_ARE_ON "\115\157\164\157\162\163\040\141\162\145\040\157\156"
/** Original scalar constant RC_MOTORS_ARE_ON=3003; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_MOTORS_ARE_ON UINT32_C(3003)
/** Original scalar constant RAPID_NOT_STOPPED="RAPID has not been stopped"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RAPID_NOT_STOPPED "\122\101\120\111\104\040\150\141\163\040\156\157\164\040\142\145\145\156\040\163\164\157\160\160\145\144"
/** Original scalar constant RC_RAPID_NOT_STOPPED=3004; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_RAPID_NOT_STOPPED UINT32_C(3004)
/** Original scalar constant RAPID_NOT_RUNNING="RAPID is not running"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RAPID_NOT_RUNNING "\122\101\120\111\104\040\151\163\040\156\157\164\040\162\165\156\156\151\156\147"
/** Original scalar constant RC_RAPID_NOT_RUNNING=3005; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_RAPID_NOT_RUNNING UINT32_C(3005)
/** Original scalar constant SM_RUNTIME_STATES_MISSING="No runtime states received for any StateMachine Add-In instance"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_SM_RUNTIME_STATES_MISSING "\116\157\040\162\165\156\164\151\155\145\040\163\164\141\164\145\163\040\162\145\143\145\151\166\145\144\040\146\157\162\040\141\156\171\040\123\164\141\164\145\115\141\143\150\151\156\145\040\101\144\144\055\111\156\040\151\156\163\164\141\156\143\145"
/** Original scalar constant RC_SM_RUNTIME_STATES_MISSING=4001; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_SM_RUNTIME_STATES_MISSING UINT32_C(4001)
/** Original scalar constant SM_UNKNOWN_RAPID_TASK="RAPID task not found among known StateMachine Add-In instances"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_SM_UNKNOWN_RAPID_TASK "\122\101\120\111\104\040\164\141\163\153\040\156\157\164\040\146\157\165\156\144\040\141\155\157\156\147\040\153\156\157\167\156\040\123\164\141\164\145\115\141\143\150\151\156\145\040\101\144\144\055\111\156\040\151\156\163\164\141\156\143\145\163"
/** Original scalar constant RC_SM_UNKNOWN_RAPID_TASK=4002; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_SM_UNKNOWN_RAPID_TASK UINT32_C(4002)
/** Original scalar constant SM_UNINITIALIZED="StateMachine Add-In instance has not been initialized"; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_SM_UNINITIALIZED "\123\164\141\164\145\115\141\143\150\151\156\145\040\101\144\144\055\111\156\040\151\156\163\164\141\156\143\145\040\150\141\163\040\156\157\164\040\142\145\145\156\040\151\156\151\164\151\141\154\151\172\145\144"
/** Original scalar constant RC_SM_UNINITIALIZED=4003; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SERVICERESPONSES_RC_SM_UNINITIALIZED UINT32_C(4003)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_valid(roswasmc_bounded_1_abb_robot_msgs_ServiceResponses const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_wire_size(roswasmc_bounded_1_abb_robot_msgs_ServiceResponses const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_write_fields(roswasmc_bounded_1_abb_robot_msgs_ServiceResponses const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_ServiceResponses* value) {
  return roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_encode(roswasmc_bounded_1_abb_robot_msgs_ServiceResponses const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_ServiceResponses* value) {
  roswasmc_bounded_1_abb_robot_msgs_ServiceResponses next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_ServiceResponses_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetFileContentsRequest.filename; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_filename_text;
/** UTF-8 byte storage for abb_robot_msgs/SetFileContentsRequest.contents; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_contents_text;
/** Complete ROS1 abb_robot_msgs/SetFileContentsRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest {
  /** Declared field filename; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_filename_text filename;
  /** Declared field contents; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_contents_text contents;
} roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETFILECONTENTSREQUEST_MAX_WIRE_BYTES 4616u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETFILECONTENTSREQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETFILECONTENTSREQUEST_CODEC_IDENTITY "UWVMROS1_f764448d9754e9ebc5e5fab64f0c8ebb01c534ec7b95751ed7d58e8ce0bd3012"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest const* value) {
  if(!value)return 0;
  if(value->filename.size>512u||!uwvm_bounded_utf8(value->filename.data,value->filename.size))return 0;
  if(value->contents.size>4096u||!uwvm_bounded_utf8(value->contents.data,value->contents.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest const* value) {size_t n=0;
  n+=4+value->filename.size;
  n+=4+value->contents.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->filename.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->filename.data,value->filename.size);*offset+=value->filename.size;
  uwvm_bounded_put(p+*offset,value->contents.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->contents.data,value->contents.size);*offset+=value->contents.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->filename.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->filename.size>512u||value->filename.size>n-*offset)return 0;uwvm_bounded_copy(value->filename.data,p+*offset,value->filename.size);*offset+=value->filename.size;
  if(*offset>n||n-*offset<4)return 0;value->contents.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->contents.size>4096u||value->contents.size>n-*offset)return 0;uwvm_bounded_copy(value->contents.data,p+*offset,value->contents.size);*offset+=value->contents.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>4616u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetFileContentsRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetFileContentsResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetFileContentsResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETFILECONTENTSRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETFILECONTENTSRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETFILECONTENTSRESPONSE_CODEC_IDENTITY "UWVMROS1_3de2a08b8e158e0612898880d6d9c98f1995828eb53bd5639b47f730c6cbefd0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetFileContentsResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetIOSignalRequest.signal; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_signal_text;
/** UTF-8 byte storage for abb_robot_msgs/SetIOSignalRequest.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_value_text;
/** Complete ROS1 abb_robot_msgs/SetIOSignalRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest {
  /** Declared field signal; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_signal_text signal;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_value_text value;
} roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETIOSIGNALREQUEST_MAX_WIRE_BYTES 1032u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETIOSIGNALREQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETIOSIGNALREQUEST_CODEC_IDENTITY "UWVMROS1_57a0197e2a5de7efd2fc91d945d699be24559baec6651179a1256f1d9d35bbc9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest const* value) {
  if(!value)return 0;
  if(value->signal.size>512u||!uwvm_bounded_utf8(value->signal.data,value->signal.size))return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest const* value) {size_t n=0;
  n+=4+value->signal.size;
  n+=4+value->value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->signal.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->signal.data,value->signal.size);*offset+=value->signal.size;
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->signal.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->signal.size>512u||value->signal.size>n-*offset)return 0;uwvm_bounded_copy(value->signal.data,p+*offset,value->signal.size);*offset+=value->signal.size;
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>1032u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetIOSignalRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetIOSignalResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetIOSignalResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETIOSIGNALRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETIOSIGNALRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETIOSIGNALRESPONSE_CODEC_IDENTITY "UWVMROS1_becc0f844ce6a7fabd8a683175edd8033ba5c943c2de152991755ee112b460e5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetIOSignalResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/SetRAPIDBoolRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDBOOLREQUEST_MAX_WIRE_BYTES 205u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDBOOLREQUEST_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDBOOLREQUEST_CODEC_IDENTITY "UWVMROS1_d8e5420678fe5feeffa20ad9ab8431678ec346f764c94871c4496c62e9dd8e8d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  if(value->value>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>205u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetRAPIDBoolResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetRAPIDBoolResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDBOOLRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDBOOLRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDBOOLRESPONSE_CODEC_IDENTITY "UWVMROS1_da2ae1231cb0c1ec452fb2733d5c99dcae78ad0bfd10771241708931b271bf13"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDBoolResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/SetRAPIDDnumRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  double value;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDDNUMREQUEST_MAX_WIRE_BYTES 212u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDDNUMREQUEST_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDDNUMREQUEST_CODEC_IDENTITY "UWVMROS1_0f17e3068f86bca6effb99f3bc850234b0163028f372340c450fce31657b9cf9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->value,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->value,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>212u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetRAPIDDnumResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetRAPIDDnumResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDDNUMRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDDNUMRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDDNUMRESPONSE_CODEC_IDENTITY "UWVMROS1_1905aed9683e03494baa54627ddbaf4aed0b92357823c0c042f9b7c6a970c8ed"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDDnumResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/SetRAPIDNumRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDNUMREQUEST_MAX_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDNUMREQUEST_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDNUMREQUEST_CODEC_IDENTITY "UWVMROS1_9982d02cb0b6ee5813f5fb38072d665226b7885aeb90517af881f4fc990f392f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>208u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetRAPIDNumResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetRAPIDNumResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDNUMRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDNUMRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDNUMRESPONSE_CODEC_IDENTITY "UWVMROS1_af1181242b4afdb6d387d39c24d0bc343e872fb9ab21c0ec1e451785786a4e64"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDNumResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetRAPIDStringRequest.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_value_text;
/** Complete ROS1 abb_robot_msgs/SetRAPIDStringRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_value_text value;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSTRINGREQUEST_MAX_WIRE_BYTES 720u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSTRINGREQUEST_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSTRINGREQUEST_CODEC_IDENTITY "UWVMROS1_e06839324f8abfa8780b683386f87bab1ec04f1e6463e63c0599f5c2f8a75f0c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  n+=4+value->value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>720u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetRAPIDStringResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetRAPIDStringResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSTRINGRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSTRINGRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSTRINGRESPONSE_CODEC_IDENTITY "UWVMROS1_e175bdff5a150722b8c50cf2e1c0f28a99289a0b148339efdee4b034ea3b96b4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDStringResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetRAPIDSymbolRequest.value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_value_text;
/** Complete ROS1 abb_robot_msgs/SetRAPIDSymbolRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest {
  /** Declared field path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath path;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_value_text value;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSYMBOLREQUEST_MAX_WIRE_BYTES 720u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSYMBOLREQUEST_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSYMBOLREQUEST_CODEC_IDENTITY "UWVMROS1_0654a5a8e5ec42f4feab54b086a008e02a74d44df65d2fcf60f007736e3fec7d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_valid(&value->path))return 0;
  if(value->value.size>512u||!uwvm_bounded_utf8(value->value.data,value->value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_wire_size(&value->path);
  n+=4+value->value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_write_fields(&value->path,p,offset);
  uwvm_bounded_put(p+*offset,value->value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->value.data,value->value.size);*offset+=value->value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest* value) {
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDSymbolPath_read_fields(p,n,offset,&value->path))return 0;
  if(*offset>n||n-*offset<4)return 0;value->value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->value.size>512u||value->value.size>n-*offset)return 0;uwvm_bounded_copy(value->value.data,p+*offset,value->value.size);*offset+=value->value.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>720u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetRAPIDSymbolResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetRAPIDSymbolResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSYMBOLRESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSYMBOLRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETRAPIDSYMBOLRESPONSE_CODEC_IDENTITY "UWVMROS1_a4ca0a8821337d1f16ba1f2ad2b232aceaba419308ca03b27f43dac427a32846"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetRAPIDSymbolResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/SetSpeedRatioRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest {
  /** Declared field speed_ratio; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t speed_ratio;
} roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETSPEEDRATIOREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETSPEEDRATIOREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETSPEEDRATIOREQUEST_CODEC_IDENTITY "UWVMROS1_a6ac368d243d488901b03d5e4303600d5f0eaf62150801103e5f5f9c0863fab5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_valid(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed_ratio,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed_ratio,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_encode(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/SetSpeedRatioResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_message_text;
/** Complete ROS1 abb_robot_msgs/SetSpeedRatioResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETSPEEDRATIORESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETSPEEDRATIORESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SETSPEEDRATIORESPONSE_CODEC_IDENTITY "UWVMROS1_9775dc1029993d1dfdf5f3080e6ae4262177ba96cdc0fbe9851dc4fea16c70d8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_valid(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_encode(roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SetSpeedRatioResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState data[16];} roswasmc_bounded_1_abb_robot_msgs_SystemState_rapid_tasks_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState data[16];} roswasmc_bounded_1_abb_robot_msgs_SystemState_mechanical_units_sequence;
/** Complete ROS1 abb_robot_msgs/SystemState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_SystemState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field motors_on; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t motors_on;
  /** Declared field auto_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t auto_mode;
  /** Declared field rapid_running; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rapid_running;
  /** Declared field rapid_tasks; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SystemState_rapid_tasks_sequence rapid_tasks;
  /** Declared field mechanical_units; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_SystemState_mechanical_units_sequence mechanical_units;
} roswasmc_bounded_1_abb_robot_msgs_SystemState;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SYSTEMSTATE_MAX_WIRE_BYTES 2331u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SYSTEMSTATE_MIN_WIRE_BYTES 27u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_SYSTEMSTATE_CODEC_IDENTITY "UWVMROS1_b57aa1fdf098aa78c51a96bd2978e02f97cbae67148c0028557d2a3e1e8630f2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SystemState_valid(roswasmc_bounded_1_abb_robot_msgs_SystemState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->motors_on>1)return 0;
  if(value->auto_mode>1)return 0;
  if(value->rapid_running>1)return 0;
  if(value->rapid_tasks.size>16u)return 0;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_valid(&value->rapid_tasks.data[i]))return 0;
  }}
  if(value->mechanical_units.size>16u)return 0;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  if(!roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_valid(&value->mechanical_units.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_SystemState_wire_size(roswasmc_bounded_1_abb_robot_msgs_SystemState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=1;
  n+=1;
  n+=1;
  n+=4;
  if(value->rapid_tasks.size>16u)return 0;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  n+=roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_wire_size(&value->rapid_tasks.data[i]);
  }}
  n+=4;
  if(value->mechanical_units.size>16u)return 0;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  n+=roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_wire_size(&value->mechanical_units.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_SystemState_write_fields(roswasmc_bounded_1_abb_robot_msgs_SystemState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->motors_on,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->auto_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rapid_running,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->rapid_tasks.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_write_fields(&value->rapid_tasks.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->mechanical_units.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_write_fields(&value->mechanical_units.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_SystemState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_SystemState* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->motors_on,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->auto_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rapid_running,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->rapid_tasks.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->rapid_tasks.size>16u)return 0;
  {size_t i;for(i=0;i<value->rapid_tasks.size;++i){
  if(!roswasmc_bounded_1_abb_robot_msgs_RAPIDTaskState_read_fields(p,n,offset,&value->rapid_tasks.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->mechanical_units.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->mechanical_units.size>16u)return 0;
  {size_t i;for(i=0;i<value->mechanical_units.size;++i){
  if(!roswasmc_bounded_1_abb_robot_msgs_MechanicalUnitState_read_fields(p,n,offset,&value->mechanical_units.data[i]))return 0;
  }}
  return roswasmc_bounded_1_abb_robot_msgs_SystemState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SystemState_encode(roswasmc_bounded_1_abb_robot_msgs_SystemState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_SystemState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_SystemState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_SystemState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_SystemState_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_SystemState* value) {
  roswasmc_bounded_1_abb_robot_msgs_SystemState next={0};size_t offset=0;if(!value||(length&&!input)||length<27u||length>2331u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_SystemState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 abb_robot_msgs/TriggerWithResultCodeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_TRIGGERWITHRESULTCODEREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_TRIGGERWITHRESULTCODEREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_TRIGGERWITHRESULTCODEREQUEST_CODEC_IDENTITY "UWVMROS1_999f63504788e522f7d0108a895cf35cd101e9a50856e480ae9edabf791bcf94"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_valid(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_wire_size(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_write_fields(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest* value) {
  return roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_encode(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest* value) {
  roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for abb_robot_msgs/TriggerWithResultCodeResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_message_text;
/** Complete ROS1 abb_robot_msgs/TriggerWithResultCodeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_message_text message;
} roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse;
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_TRIGGERWITHRESULTCODERESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_TRIGGERWITHRESULTCODERESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_ABB_ROBOT_MSGS_TRIGGERWITHRESULTCODERESPONSE_CODEC_IDENTITY "UWVMROS1_4ef5c987592e849766b82b7fd99a65b7a1e17605f3e3341c99803181887967ff"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_valid(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_wire_size(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse const* value) {size_t n=0;
  n+=2;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_write_fields(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->result_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_encode(roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse* value) {
  roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_abb_robot_msgs_TriggerWithResultCodeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 duration value; no field projects away native data. */
typedef struct roswasmc_bounded_1_duration {
  /** Declared field sec; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sec;
  /** Declared field nsec; canonical capacity is part of CODEC_IDENTITY. */
  int32_t nsec;
} roswasmc_bounded_1_duration;
#define ROSWASMC_BOUNDED_1_DURATION_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_DURATION_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_DURATION_CODEC_IDENTITY "UWVMROS1_8dbec6cdfef79495208abb358c5e8283ec8d9d471f984424afcda997d142cca8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_duration_valid(roswasmc_bounded_1_duration const* value) {
  if(!value)return 0;
  if(value->nsec<0||value->nsec>=1000000000)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_duration_wire_size(roswasmc_bounded_1_duration const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_duration_write_fields(roswasmc_bounded_1_duration const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->sec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nsec,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_duration_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_duration* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->nsec,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_duration_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_duration_encode(roswasmc_bounded_1_duration const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_duration_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_duration_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_duration_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_duration_decode(void const* input,size_t length,roswasmc_bounded_1_duration* value) {
  roswasmc_bounded_1_duration next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_duration_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_positions_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_velocities_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_accelerations_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_effort_sequence;
/** Complete ROS1 trajectory_msgs/JointTrajectoryPoint value; no field projects away native data. */
typedef struct roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint {
  /** Declared field positions; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_positions_sequence positions;
  /** Declared field velocities; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_velocities_sequence velocities;
  /** Declared field accelerations; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_accelerations_sequence accelerations;
  /** Declared field effort; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_effort_sequence effort;
  /** Declared field time_from_start; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_duration time_from_start;
} roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint;
#define ROSWASMC_BOUNDED_1_TRAJECTORY_MSGS_JOINTTRAJECTORYPOINT_MAX_WIRE_BYTES 408u
#define ROSWASMC_BOUNDED_1_TRAJECTORY_MSGS_JOINTTRAJECTORYPOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_TRAJECTORY_MSGS_JOINTTRAJECTORYPOINT_CODEC_IDENTITY "UWVMROS1_9773572fff8616235c4d7e23fccbe01f28dcfa62c7c80323a65c8c305feba3c5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint const* value) {
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
  if(!roswasmc_bounded_1_duration_valid(&value->time_from_start))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_wire_size(roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint const* value) {size_t n=0;
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
  n+=roswasmc_bounded_1_duration_wire_size(&value->time_from_start);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_write_fields(roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint const* value,unsigned char* p,size_t* offset) {
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
  roswasmc_bounded_1_duration_write_fields(&value->time_from_start,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint* value) {
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
  if(!roswasmc_bounded_1_duration_read_fields(p,n,offset,&value->time_from_start))return 0;
  return roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_encode(roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_decode(void const* input,size_t length,roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint* value) {
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>408u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for trajectory_msgs/JointTrajectory.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_trajectory_msgs_JointTrajectory_joint_names_text;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_trajectory_msgs_JointTrajectory_joint_names_text data[12];} roswasmc_bounded_1_trajectory_msgs_JointTrajectory_joint_names_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint data[8];} roswasmc_bounded_1_trajectory_msgs_JointTrajectory_points_sequence;
/** Complete ROS1 trajectory_msgs/JointTrajectory value; no field projects away native data. */
typedef struct roswasmc_bounded_1_trajectory_msgs_JointTrajectory {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectory_joint_names_sequence joint_names;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectory_points_sequence points;
} roswasmc_bounded_1_trajectory_msgs_JointTrajectory;
#define ROSWASMC_BOUNDED_1_TRAJECTORY_MSGS_JOINTTRAJECTORY_MAX_WIRE_BYTES 4168u
#define ROSWASMC_BOUNDED_1_TRAJECTORY_MSGS_JOINTTRAJECTORY_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_TRAJECTORY_MSGS_JOINTTRAJECTORY_CODEC_IDENTITY "UWVMROS1_079e6c432f0a858d6bd47c87aaf353a8b0676706809e0f984705b1a688de282b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_trajectory_msgs_JointTrajectory_valid(roswasmc_bounded_1_trajectory_msgs_JointTrajectory const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>64u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(value->points.size>8u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(&value->points.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_trajectory_msgs_JointTrajectory_wire_size(roswasmc_bounded_1_trajectory_msgs_JointTrajectory const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=4;
  if(value->points.size>8u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  n+=roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_wire_size(&value->points.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_trajectory_msgs_JointTrajectory_write_fields(roswasmc_bounded_1_trajectory_msgs_JointTrajectory const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  uwvm_bounded_put(p+*offset,value->points.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->points.size;++i){
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_write_fields(&value->points.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_trajectory_msgs_JointTrajectory_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_trajectory_msgs_JointTrajectory* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>64u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>8u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_read_fields(p,n,offset,&value->points.data[i]))return 0;
  }}
  return roswasmc_bounded_1_trajectory_msgs_JointTrajectory_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_trajectory_msgs_JointTrajectory_encode(roswasmc_bounded_1_trajectory_msgs_JointTrajectory const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_trajectory_msgs_JointTrajectory_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_trajectory_msgs_JointTrajectory_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_trajectory_msgs_JointTrajectory_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_trajectory_msgs_JointTrajectory_decode(void const* input,size_t length,roswasmc_bounded_1_trajectory_msgs_JointTrajectory* value) {
  roswasmc_bounded_1_trajectory_msgs_JointTrajectory next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>4168u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectory_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/JointTolerance.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_control_msgs_JointTolerance_name_text;
/** Complete ROS1 control_msgs/JointTolerance value; no field projects away native data. */
typedef struct roswasmc_bounded_1_control_msgs_JointTolerance {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_control_msgs_JointTolerance_name_text name;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  double position;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  double velocity;
  /** Declared field acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double acceleration;
} roswasmc_bounded_1_control_msgs_JointTolerance;
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_JOINTTOLERANCE_MAX_WIRE_BYTES 92u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_JOINTTOLERANCE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_JOINTTOLERANCE_CODEC_IDENTITY "UWVMROS1_c9f71f98726d4a6621dcc8db96a394a24941a79d9041c8ad7a07724ee64d3c88"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_control_msgs_JointTolerance_valid(roswasmc_bounded_1_control_msgs_JointTolerance const* value) {
  if(!value)return 0;
  if(value->name.size>64u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_control_msgs_JointTolerance_wire_size(roswasmc_bounded_1_control_msgs_JointTolerance const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_control_msgs_JointTolerance_write_fields(roswasmc_bounded_1_control_msgs_JointTolerance const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_control_msgs_JointTolerance_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_control_msgs_JointTolerance* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>64u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->acceleration,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_control_msgs_JointTolerance_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_control_msgs_JointTolerance_encode(roswasmc_bounded_1_control_msgs_JointTolerance const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_control_msgs_JointTolerance_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_control_msgs_JointTolerance_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_control_msgs_JointTolerance_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_control_msgs_JointTolerance_decode(void const* input,size_t length,roswasmc_bounded_1_control_msgs_JointTolerance* value) {
  roswasmc_bounded_1_control_msgs_JointTolerance next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>92u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_control_msgs_JointTolerance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_control_msgs_JointTolerance data[12];} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_path_tolerance_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_control_msgs_JointTolerance data[12];} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_goal_tolerance_sequence;
/** Complete ROS1 control_msgs/FollowJointTrajectoryGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal {
  /** Declared field trajectory; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectory trajectory;
  /** Declared field path_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_path_tolerance_sequence path_tolerance;
  /** Declared field goal_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_goal_tolerance_sequence goal_tolerance;
  /** Declared field goal_time_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_duration goal_time_tolerance;
} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal;
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYGOAL_MAX_WIRE_BYTES 6392u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYGOAL_MIN_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYGOAL_CODEC_IDENTITY "UWVMROS1_e9fda6f9e732450f2cf1af046ceb67815e6e1a22105eda2fee38f8f470252d40"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_valid(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectory_valid(&value->trajectory))return 0;
  if(value->path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_1_control_msgs_JointTolerance_valid(&value->path_tolerance.data[i]))return 0;
  }}
  if(value->goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_1_control_msgs_JointTolerance_valid(&value->goal_tolerance.data[i]))return 0;
  }}
  if(!roswasmc_bounded_1_duration_valid(&value->goal_time_tolerance))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_wire_size(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal const* value) {size_t n=0;
  n+=roswasmc_bounded_1_trajectory_msgs_JointTrajectory_wire_size(&value->trajectory);
  n+=4;
  if(value->path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  n+=roswasmc_bounded_1_control_msgs_JointTolerance_wire_size(&value->path_tolerance.data[i]);
  }}
  n+=4;
  if(value->goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  n+=roswasmc_bounded_1_control_msgs_JointTolerance_wire_size(&value->goal_tolerance.data[i]);
  }}
  n+=roswasmc_bounded_1_duration_wire_size(&value->goal_time_tolerance);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_write_fields(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_trajectory_msgs_JointTrajectory_write_fields(&value->trajectory,p,offset);
  uwvm_bounded_put(p+*offset,value->path_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  roswasmc_bounded_1_control_msgs_JointTolerance_write_fields(&value->path_tolerance.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->goal_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  roswasmc_bounded_1_control_msgs_JointTolerance_write_fields(&value->goal_tolerance.data[i],p,offset);
  }}
  roswasmc_bounded_1_duration_write_fields(&value->goal_time_tolerance,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal* value) {
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectory_read_fields(p,n,offset,&value->trajectory))return 0;
  if(*offset>n||n-*offset<4)return 0;value->path_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->path_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_1_control_msgs_JointTolerance_read_fields(p,n,offset,&value->path_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->goal_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->goal_tolerance.size>12u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_1_control_msgs_JointTolerance_read_fields(p,n,offset,&value->goal_tolerance.data[i]))return 0;
  }}
  if(!roswasmc_bounded_1_duration_read_fields(p,n,offset,&value->goal_time_tolerance))return 0;
  return roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_encode(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_decode(void const* input,size_t length,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal* value) {
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<40u||length>6392u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/FollowJointTrajectoryResult.error_string; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_error_string_text;
/** Complete ROS1 control_msgs/FollowJointTrajectoryResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult {
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  int32_t error_code;
  /** Declared field error_string; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_error_string_text error_string;
} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult;
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_MAX_WIRE_BYTES 4104u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_CODEC_IDENTITY "UWVMROS1_71da832be83c08804668aa63576768bbe831f21f5f270cdc8d851b30d2fc4281"
/** Original scalar constant SUCCESSFUL=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_SUCCESSFUL INT32_C(0)
/** Original scalar constant INVALID_GOAL=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_INVALID_GOAL -INT32_C(1)
/** Original scalar constant INVALID_JOINTS=-2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_INVALID_JOINTS -INT32_C(2)
/** Original scalar constant OLD_HEADER_TIMESTAMP=-3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_OLD_HEADER_TIMESTAMP -INT32_C(3)
/** Original scalar constant PATH_TOLERANCE_VIOLATED=-4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_PATH_TOLERANCE_VIOLATED -INT32_C(4)
/** Original scalar constant GOAL_TOLERANCE_VIOLATED=-5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYRESULT_GOAL_TOLERANCE_VIOLATED -INT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_valid(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult const* value) {
  if(!value)return 0;
  if(value->error_string.size>4096u||!uwvm_bounded_utf8(value->error_string.data,value->error_string.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_wire_size(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult const* value) {size_t n=0;
  n+=4;
  n+=4+value->error_string.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_write_fields(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->error_string.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_string.data,value->error_string.size);*offset+=value->error_string.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->error_string.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_string.size>4096u||value->error_string.size>n-*offset)return 0;uwvm_bounded_copy(value->error_string.data,p+*offset,value->error_string.size);*offset+=value->error_string.size;
  return roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_encode(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_decode(void const* input,size_t length,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult* value) {
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>4104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/FollowJointTrajectoryFeedback.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_joint_names_text;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_joint_names_text data[12];} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_joint_names_sequence;
/** Complete ROS1 control_msgs/FollowJointTrajectoryFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_joint_names_sequence joint_names;
  /** Declared field desired; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint desired;
  /** Declared field actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint actual;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint error;
} roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback;
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYFEEDBACK_MAX_WIRE_BYTES 2124u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYFEEDBACK_MIN_WIRE_BYTES 92u
#define ROSWASMC_BOUNDED_1_CONTROL_MSGS_FOLLOWJOINTTRAJECTORYFEEDBACK_CODEC_IDENTITY "UWVMROS1_a1ad5a1f8a00b60f97d06209737993bd395d83d1a008705d65193b63933b62a8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_valid(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>64u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(&value->desired))return 0;
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(&value->actual))return 0;
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_valid(&value->error))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_wire_size(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_wire_size(&value->desired);
  n+=roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_wire_size(&value->actual);
  n+=roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_wire_size(&value->error);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_write_fields(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_write_fields(&value->desired,p,offset);
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_write_fields(&value->actual,p,offset);
  roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_write_fields(&value->error,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>64u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_read_fields(p,n,offset,&value->desired))return 0;
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_read_fields(p,n,offset,&value->actual))return 0;
  if(!roswasmc_bounded_1_trajectory_msgs_JointTrajectoryPoint_read_fields(p,n,offset,&value->error))return 0;
  return roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_encode(roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback* value) {
  roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<92u||length>2124u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_control_msgs_FollowJointTrajectoryFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for sensor_msgs/JointState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_1_sensor_msgs_JointState_name_text;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_sensor_msgs_JointState_name_text data[12];} roswasmc_bounded_1_sensor_msgs_JointState_name_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_1_sensor_msgs_JointState_position_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_1_sensor_msgs_JointState_velocity_sequence;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[12];} roswasmc_bounded_1_sensor_msgs_JointState_effort_sequence;
/** Complete ROS1 sensor_msgs/JointState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_sensor_msgs_JointState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_JointState_name_sequence name;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_JointState_position_sequence position;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_JointState_velocity_sequence velocity;
  /** Declared field effort; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_JointState_effort_sequence effort;
} roswasmc_bounded_1_sensor_msgs_JointState;
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_JOINTSTATE_MAX_WIRE_BYTES 1200u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_JOINTSTATE_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_JOINTSTATE_CODEC_IDENTITY "UWVMROS1_8d1bb9c4903dd4b070a5ab157b3590815722d6b574d610c9b9fb5ead8be7a057"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_sensor_msgs_JointState_valid(roswasmc_bounded_1_sensor_msgs_JointState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
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
static inline size_t roswasmc_bounded_1_sensor_msgs_JointState_wire_size(roswasmc_bounded_1_sensor_msgs_JointState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
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
static inline void roswasmc_bounded_1_sensor_msgs_JointState_write_fields(roswasmc_bounded_1_sensor_msgs_JointState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
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
static inline int roswasmc_bounded_1_sensor_msgs_JointState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_sensor_msgs_JointState* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
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
  return roswasmc_bounded_1_sensor_msgs_JointState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_JointState_encode(roswasmc_bounded_1_sensor_msgs_JointState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_sensor_msgs_JointState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_sensor_msgs_JointState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_sensor_msgs_JointState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_JointState_decode(void const* input,size_t length,roswasmc_bounded_1_sensor_msgs_JointState* value) {
  roswasmc_bounded_1_sensor_msgs_JointState next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>1200u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_sensor_msgs_JointState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
