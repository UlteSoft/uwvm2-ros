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
/** Complete ROS2 clearpath_motor_msgs/action/LynxCalibrate_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_GOAL_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_GOAL_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_GOAL_CODEC_IDENTITY "UWVMROS1_61b01a6234b41f96464de9449b3e8d1b955decf6afa12aa3ba94062c98cec681"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_valid(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal* value) {
  return roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_encode(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2];} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_offset_sequence;
/** Complete ROS2 clearpath_motor_msgs/action/LynxCalibrate_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result {
  /** Declared field offset; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_offset_sequence offset;
} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_RESULT_MAX_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_RESULT_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_RESULT_CODEC_IDENTITY "UWVMROS1_686450363b8b15eba1e9e9e9ff05c9aacfb95bb0d113eaa7c443e4c70d55d41f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_valid(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result const* value) {
  if(!value)return 0;
  if(value->offset.size>2u)return 0;
  {size_t i;for(i=0;i<value->offset.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->offset.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result const* value) {size_t n=0;
  n+=4;
  if(value->offset.size>2u)return 0;
  {size_t i;for(i=0;i<value->offset.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->offset.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->offset.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->offset.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result* value) {
  if(*offset>n||n-*offset<4)return 0;value->offset.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->offset.size>2u)return 0;
  {size_t i;for(i=0;i<value->offset.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->offset.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_encode(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>12u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint16_t data[2];} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_iteration_sequence;
/** Complete ROS2 clearpath_motor_msgs/action/LynxCalibrate_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback {
  /** Declared field iteration; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_iteration_sequence iteration;
} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_FEEDBACK_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_FEEDBACK_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXCALIBRATE_FEEDBACK_CODEC_IDENTITY "UWVMROS1_ebe7badd3f72982079911d8aa72c4ddd27ff0a12d2f948e5970649317a592b53"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_valid(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback const* value) {
  if(!value)return 0;
  if(value->iteration.size>2u)return 0;
  {size_t i;for(i=0;i<value->iteration.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback const* value) {size_t n=0;
  n+=4;
  if(value->iteration.size>2u)return 0;
  {size_t i;for(i=0;i<value->iteration.size;++i){
  n+=2;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->iteration.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->iteration.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->iteration.data[i],2);*offset+=2;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback* value) {
  if(*offset>n||n-*offset<4)return 0;value->iteration.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->iteration.size>2u)return 0;
  {size_t i;for(i=0;i<value->iteration.size;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->iteration.data[i],&bits,2);}*offset+=2;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_encode(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxCalibrate_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_motor_msgs/action/LynxUpdate_Goal.file; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_file_text;
/** Complete ROS2 clearpath_motor_msgs/action/LynxUpdate_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal {
  /** Declared field file; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_file_text file;
} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_GOAL_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_GOAL_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_GOAL_CODEC_IDENTITY "UWVMROS1_f0362f8a90391e8ffda2e8f3b798769b644091058361882c4e9457196f007565"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_valid(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal const* value) {
  if(!value)return 0;
  if(value->file.size>256u||!uwvm_bounded_utf8(value->file.data,value->file.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal const* value) {size_t n=0;
  n+=4+value->file.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->file.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->file.data,value->file.size);*offset+=value->file.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal* value) {
  if(*offset>n||n-*offset<4)return 0;value->file.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->file.size>256u||value->file.size>n-*offset)return 0;uwvm_bounded_copy(value->file.data,p+*offset,value->file.size);*offset+=value->file.size;
  return roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_encode(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2];} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_success_sequence;
/** Complete ROS2 clearpath_motor_msgs/action/LynxUpdate_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_success_sequence success;
} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_RESULT_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_RESULT_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_RESULT_CODEC_IDENTITY "UWVMROS1_126fe871dbcb036e485d20e8ccdc8e9fb7be01757f539bfba895aabf44d341d8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_valid(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result const* value) {
  if(!value)return 0;
  if(value->success.size>2u)return 0;
  {size_t i;for(i=0;i<value->success.size;++i){
  if(value->success.data[i]>1)return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result const* value) {size_t n=0;
  n+=4;
  if(value->success.size>2u)return 0;
  {size_t i;for(i=0;i<value->success.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->success.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->success.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->success.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result* value) {
  if(*offset>n||n-*offset<4)return 0;value->success.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->success.size>2u)return 0;
  {size_t i;for(i=0;i<value->success.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_encode(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2];} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_progress_sequence;
/** Complete ROS2 clearpath_motor_msgs/action/LynxUpdate_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback {
  /** Declared field progress; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_progress_sequence progress;
} roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_FEEDBACK_MAX_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_FEEDBACK_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_ACTION_LYNXUPDATE_FEEDBACK_CODEC_IDENTITY "UWVMROS1_2179ec9dee627234a1a31e9681ff214cc45ca837349cd8e407e019a354121b19"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_valid(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback const* value) {
  if(!value)return 0;
  if(value->progress.size>2u)return 0;
  {size_t i;for(i=0;i<value->progress.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->progress.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback const* value) {size_t n=0;
  n+=4;
  if(value->progress.size>2u)return 0;
  {size_t i;for(i=0;i<value->progress.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->progress.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->progress.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->progress.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback* value) {
  if(*offset>n||n-*offset<4)return 0;value->progress.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->progress.size>2u)return 0;
  {size_t i;for(i=0;i<value->progress.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->progress.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_encode(roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>12u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_action_LynxUpdate_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** UTF-8 byte storage for clearpath_control_msgs/msg/DeviceInfo.device_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_device_id_text;
/** UTF-8 byte storage for clearpath_control_msgs/msg/DeviceInfo.device_class; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_device_class_text;
/** UTF-8 byte storage for clearpath_control_msgs/msg/DeviceInfo.model; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_model_text;
/** UTF-8 byte storage for clearpath_control_msgs/msg/DeviceInfo.manufacturer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_manufacturer_text;
/** UTF-8 byte storage for clearpath_control_msgs/msg/DeviceInfo.firmware_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_firmware_version_text;
/** UTF-8 byte storage for clearpath_control_msgs/msg/DeviceInfo.hardware_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_hardware_version_text;
/** UTF-8 byte storage for clearpath_control_msgs/msg/DeviceInfo.serial_number; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_serial_number_text;
/** Complete ROS2 clearpath_control_msgs/msg/DeviceInfo value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo {
  /** Declared field device_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_device_id_text device_id;
  /** Declared field device_class; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_device_class_text device_class;
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_model_text model;
  /** Declared field manufacturer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_manufacturer_text manufacturer;
  /** Declared field firmware_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_firmware_version_text firmware_version;
  /** Declared field hardware_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_hardware_version_text hardware_version;
  /** Declared field serial_number; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_serial_number_text serial_number;
} roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo;
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_MSG_DEVICEINFO_MAX_WIRE_BYTES 1820u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_MSG_DEVICEINFO_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_MSG_DEVICEINFO_CODEC_IDENTITY "UWVMROS1_9111f5d4b5324c36eb85f01e6fdbc3c589479ff5b64a8fa7b9d467aa14675c3e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_valid(roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo const* value) {
  if(!value)return 0;
  if(value->device_id.size>256u||!uwvm_bounded_utf8(value->device_id.data,value->device_id.size))return 0;
  if(value->device_class.size>256u||!uwvm_bounded_utf8(value->device_class.data,value->device_class.size))return 0;
  if(value->model.size>256u||!uwvm_bounded_utf8(value->model.data,value->model.size))return 0;
  if(value->manufacturer.size>256u||!uwvm_bounded_utf8(value->manufacturer.data,value->manufacturer.size))return 0;
  if(value->firmware_version.size>256u||!uwvm_bounded_utf8(value->firmware_version.data,value->firmware_version.size))return 0;
  if(value->hardware_version.size>256u||!uwvm_bounded_utf8(value->hardware_version.data,value->hardware_version.size))return 0;
  if(value->serial_number.size>256u||!uwvm_bounded_utf8(value->serial_number.data,value->serial_number.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_wire_size(roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo const* value) {size_t n=0;
  n+=4+value->device_id.size;
  n+=4+value->device_class.size;
  n+=4+value->model.size;
  n+=4+value->manufacturer.size;
  n+=4+value->firmware_version.size;
  n+=4+value->hardware_version.size;
  n+=4+value->serial_number.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_write_fields(roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->device_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->device_id.data,value->device_id.size);*offset+=value->device_id.size;
  uwvm_bounded_put(p+*offset,value->device_class.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->device_class.data,value->device_class.size);*offset+=value->device_class.size;
  uwvm_bounded_put(p+*offset,value->model.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->model.data,value->model.size);*offset+=value->model.size;
  uwvm_bounded_put(p+*offset,value->manufacturer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->manufacturer.data,value->manufacturer.size);*offset+=value->manufacturer.size;
  uwvm_bounded_put(p+*offset,value->firmware_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->firmware_version.data,value->firmware_version.size);*offset+=value->firmware_version.size;
  uwvm_bounded_put(p+*offset,value->hardware_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->hardware_version.data,value->hardware_version.size);*offset+=value->hardware_version.size;
  uwvm_bounded_put(p+*offset,value->serial_number.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->serial_number.data,value->serial_number.size);*offset+=value->serial_number.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo* value) {
  if(*offset>n||n-*offset<4)return 0;value->device_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->device_id.size>256u||value->device_id.size>n-*offset)return 0;uwvm_bounded_copy(value->device_id.data,p+*offset,value->device_id.size);*offset+=value->device_id.size;
  if(*offset>n||n-*offset<4)return 0;value->device_class.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->device_class.size>256u||value->device_class.size>n-*offset)return 0;uwvm_bounded_copy(value->device_class.data,p+*offset,value->device_class.size);*offset+=value->device_class.size;
  if(*offset>n||n-*offset<4)return 0;value->model.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->model.size>256u||value->model.size>n-*offset)return 0;uwvm_bounded_copy(value->model.data,p+*offset,value->model.size);*offset+=value->model.size;
  if(*offset>n||n-*offset<4)return 0;value->manufacturer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->manufacturer.size>256u||value->manufacturer.size>n-*offset)return 0;uwvm_bounded_copy(value->manufacturer.data,p+*offset,value->manufacturer.size);*offset+=value->manufacturer.size;
  if(*offset>n||n-*offset<4)return 0;value->firmware_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->firmware_version.size>256u||value->firmware_version.size>n-*offset)return 0;uwvm_bounded_copy(value->firmware_version.data,p+*offset,value->firmware_version.size);*offset+=value->firmware_version.size;
  if(*offset>n||n-*offset<4)return 0;value->hardware_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->hardware_version.size>256u||value->hardware_version.size>n-*offset)return 0;uwvm_bounded_copy(value->hardware_version.data,p+*offset,value->hardware_version.size);*offset+=value->hardware_version.size;
  if(*offset>n||n-*offset<4)return 0;value->serial_number.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->serial_number.size>256u||value->serial_number.size>n-*offset)return 0;uwvm_bounded_copy(value->serial_number.data,p+*offset,value->serial_number.size);*offset+=value->serial_number.size;
  return roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_encode(roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo* value) {
  roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>1820u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_control_msgs_msg_DeviceInfo_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_control_msgs/msg/StreamControl.stream; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_stream_text;
/** Complete ROS2 clearpath_control_msgs/msg/StreamControl value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl {
  /** Declared field stream; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_stream_text stream;
  /** Declared field enabled; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t enabled;
} roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl;
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_MSG_STREAMCONTROL_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_MSG_STREAMCONTROL_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_MSG_STREAMCONTROL_CODEC_IDENTITY "UWVMROS1_26208e266c9185bfb0e715407ab0fe313f2d47e6bc9f54594a102c3a09d34734"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_valid(roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl const* value) {
  if(!value)return 0;
  if(value->stream.size>256u||!uwvm_bounded_utf8(value->stream.data,value->stream.size))return 0;
  if(value->enabled>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_wire_size(roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl const* value) {size_t n=0;
  n+=4+value->stream.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_write_fields(roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->stream.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->stream.data,value->stream.size);*offset+=value->stream.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->enabled,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl* value) {
  if(*offset>n||n-*offset<4)return 0;value->stream.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->stream.size>256u||value->stream.size>n-*offset)return 0;uwvm_bounded_copy(value->stream.data,p+*offset,value->stream.size);*offset+=value->stream.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->enabled,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_encode(roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl* value) {
  roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_control_msgs_msg_StreamControl_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_motor_msgs/msg/LynxDebug.joint_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_joint_name_text;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_accumulators_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_currents_sequence;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxDebug value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug {
  /** Declared field can_id; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t can_id;
  /** Declared field joint_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_joint_name_text joint_name;
  /** Declared field fault_frequency; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t fault_frequency;
  /** Declared field accumulator_flags; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t accumulator_flags;
  /** Declared field accumulators; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_accumulators_sequence accumulators;
  /** Declared field currents; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_currents_sequence currents;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_MAX_WIRE_BYTES 291u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_MIN_WIRE_BYTES 19u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_CODEC_IDENTITY "UWVMROS1_0341ae040f1e8a1f980bde84498c4a4f8b9c55c3806839a73727c1579ab4416e"
/** Original scalar constant ACCUMULATOR_FLAG_ABOVE_RATED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_ABOVE_RATED UINT32_C(0)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_1=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_1 UINT32_C(1)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_2=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_2 UINT32_C(2)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_3=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_3 UINT32_C(3)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_4=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_4 UINT32_C(4)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_5=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_5 UINT32_C(5)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_6=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_6 UINT32_C(6)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_7=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_7 UINT32_C(7)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_8=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_8 UINT32_C(8)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_1_9=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_1_9 UINT32_C(9)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_0=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_0 UINT32_C(10)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_1=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_1 UINT32_C(11)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_2=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_2 UINT32_C(12)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_3=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_3 UINT32_C(13)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_4=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_4 UINT32_C(14)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_5=15; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_5 UINT32_C(15)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_6=16; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_6 UINT32_C(16)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_7=17; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_7 UINT32_C(17)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_8=18; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_8 UINT32_C(18)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_2_9=19; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_2_9 UINT32_C(19)
/** Original scalar constant ACCUMULATOR_FLAG_CONTINUOUS_3_0=20; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_FLAG_CONTINUOUS_3_0 UINT32_C(20)
/** Original scalar constant ACCUMULATOR_ABOVE_RATED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_ABOVE_RATED UINT32_C(0)
/** Original scalar constant ACCUMULATOR_CONTINUOUS_1_1=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_CONTINUOUS_1_1 UINT32_C(1)
/** Original scalar constant ACCUMULATOR_CONTINUOUS_1_5=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_CONTINUOUS_1_5 UINT32_C(2)
/** Original scalar constant ACCUMULATOR_CONTINUOUS_2_0=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_CONTINUOUS_2_0 UINT32_C(3)
/** Original scalar constant ACCUMULATOR_CONTINUOUS_2_5=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_CONTINUOUS_2_5 UINT32_C(4)
/** Original scalar constant ACCUMULATOR_CONTINUOUS_3_0=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_ACCUMULATOR_CONTINUOUS_3_0 UINT32_C(5)
/** Original scalar constant CURRENT_ID_FB=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_CURRENT_ID_FB UINT32_C(0)
/** Original scalar constant CURRENT_ID_FF=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_CURRENT_ID_FF UINT32_C(1)
/** Original scalar constant CURRENT_ID_REF=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_CURRENT_ID_REF UINT32_C(2)
/** Original scalar constant CURRENT_IQ_FB=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_CURRENT_IQ_FB UINT32_C(3)
/** Original scalar constant CURRENT_IQ_FF=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_CURRENT_IQ_FF UINT32_C(4)
/** Original scalar constant CURRENT_IQ_REF=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXDEBUG_CURRENT_IQ_REF UINT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug const* value) {
  if(!value)return 0;
  if(value->joint_name.size>256u||!uwvm_bounded_utf8(value->joint_name.data,value->joint_name.size))return 0;
  if(value->accumulators.size>2u)return 0;
  {size_t i;for(i=0;i<value->accumulators.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->accumulators.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->currents.size>2u)return 0;
  {size_t i;for(i=0;i<value->currents.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->currents.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug const* value) {size_t n=0;
  n+=1;
  n+=4+value->joint_name.size;
  n+=2;
  n+=4;
  n+=4;
  if(value->accumulators.size>2u)return 0;
  {size_t i;for(i=0;i<value->accumulators.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->currents.size>2u)return 0;
  {size_t i;for(i=0;i<value->currents.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->can_id,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->joint_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_name.data,value->joint_name.size);*offset+=value->joint_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->fault_frequency,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->accumulator_flags,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->accumulators.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->accumulators.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->accumulators.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->currents.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->currents.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->currents.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->can_id,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->joint_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_name.size>256u||value->joint_name.size>n-*offset)return 0;uwvm_bounded_copy(value->joint_name.data,p+*offset,value->joint_name.size);*offset+=value->joint_name.size;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->fault_frequency,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->accumulator_flags,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->accumulators.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->accumulators.size>2u)return 0;
  {size_t i;for(i=0;i<value->accumulators.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->accumulators.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->currents.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->currents.size>2u)return 0;
  {size_t i;for(i=0;i<value->currents.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->currents.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug next={0};size_t offset=0;if(!value||(length&&!input)||length<19u||length>291u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_motor_msgs/msg/LynxFeedback.joint_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_joint_name_text;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback {
  /** Declared field can_id; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t can_id;
  /** Declared field joint_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_joint_name_text joint_name;
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  float current;
  /** Declared field voltage; canonical capacity is part of CODEC_IDENTITY. */
  float voltage;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  float velocity;
  /** Declared field travel; canonical capacity is part of CODEC_IDENTITY. */
  float travel;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXFEEDBACK_MAX_WIRE_BYTES 277u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXFEEDBACK_MIN_WIRE_BYTES 21u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXFEEDBACK_CODEC_IDENTITY "UWVMROS1_5d92c7085019ebf00ba93788b5b2f4055ad40b2c6714575a66fb75cf314429eb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback const* value) {
  if(!value)return 0;
  if(value->joint_name.size>256u||!uwvm_bounded_utf8(value->joint_name.data,value->joint_name.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->voltage,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->travel,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback const* value) {size_t n=0;
  n+=1;
  n+=4+value->joint_name.size;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->can_id,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->joint_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_name.data,value->joint_name.size);*offset+=value->joint_name.size;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->voltage,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->travel,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->can_id,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->joint_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_name.size>256u||value->joint_name.size>n-*offset)return 0;uwvm_bounded_copy(value->joint_name.data,p+*offset,value->joint_name.size);*offset+=value->joint_name.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->voltage,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->velocity,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->travel,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<21u||length>277u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_motor_msgs/msg/LynxMotorProtection.joint_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_joint_name_text;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMotorProtection value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection {
  /** Declared field can_id; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t can_id;
  /** Declared field joint_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_joint_name_text joint_name;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t state;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMOTORPROTECTION_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMOTORPROTECTION_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMOTORPROTECTION_CODEC_IDENTITY "UWVMROS1_f824abfa97f6f24934e04094d66170167f19a14fb933f325cbef4ed6fca16630"
/** Original scalar constant NORMAL=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMOTORPROTECTION_NORMAL UINT32_C(0)
/** Original scalar constant THROTTLED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMOTORPROTECTION_THROTTLED UINT32_C(1)
/** Original scalar constant OVERHEATED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMOTORPROTECTION_OVERHEATED UINT32_C(2)
/** Original scalar constant ERROR=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMOTORPROTECTION_ERROR UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection const* value) {
  if(!value)return 0;
  if(value->joint_name.size>256u||!uwvm_bounded_utf8(value->joint_name.data,value->joint_name.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection const* value) {size_t n=0;
  n+=1;
  n+=4+value->joint_name.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->can_id,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->joint_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_name.data,value->joint_name.size);*offset+=value->joint_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->state,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->can_id,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->joint_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_name.size>256u||value->joint_name.size>n-*offset)return 0;uwvm_bounded_copy(value->joint_name.data,p+*offset,value->joint_name.size);*offset+=value->joint_name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->state,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug data[4];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_drivers_sequence;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMultiDebug value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field drivers; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_drivers_sequence drivers;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTIDEBUG_MAX_WIRE_BYTES 1308u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTIDEBUG_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTIDEBUG_CODEC_IDENTITY "UWVMROS1_056abd2153e7f1a6df223bf5daba581e6c218bb7ef365444fc5c3171dfc62e95"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_valid(&value->drivers.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  n+=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_wire_size(&value->drivers.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->drivers.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_write_fields(&value->drivers.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->drivers.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxDebug_read_fields(p,n,offset,&value->drivers.data[i]))return 0;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>1308u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiDebug_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback data[4];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_drivers_sequence;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMultiFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field drivers; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_drivers_sequence drivers;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTIFEEDBACK_MAX_WIRE_BYTES 1252u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTIFEEDBACK_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTIFEEDBACK_CODEC_IDENTITY "UWVMROS1_2c83cade6bec4d766fd2cff97338642b3d2065d41e9968ccf5054c6f231242c7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_valid(&value->drivers.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  n+=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_wire_size(&value->drivers.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->drivers.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_write_fields(&value->drivers.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->drivers.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxFeedback_read_fields(p,n,offset,&value->drivers.data[i]))return 0;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>1252u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_motor_msgs/msg/LynxStatus.joint_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_joint_name_text;
/** UTF-8 byte storage for clearpath_motor_msgs/msg/LynxStatus.firmware_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_firmware_version_text;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus {
  /** Declared field can_id; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t can_id;
  /** Declared field joint_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_joint_name_text joint_name;
  /** Declared field firmware_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_firmware_version_text firmware_version;
  /** Declared field motor_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float motor_temperature;
  /** Declared field mcu_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float mcu_temperature;
  /** Declared field pcb_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float pcb_temperature;
  /** Declared field stopped; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t stopped;
  /** Declared field status_flags; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t status_flags;
  /** Declared field warning_flags; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t warning_flags;
  /** Declared field error_flags; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error_flags;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_MAX_WIRE_BYTES 546u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_MIN_WIRE_BYTES 34u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_CODEC_IDENTITY "UWVMROS1_b68a7ab9bc5312c59d2ef86e49313468b631e8cacdf52468374b67f4eced5c6d"
/** Original scalar constant STATUS_FLAG_ADC_CALIBRATED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_STATUS_FLAG_ADC_CALIBRATED UINT32_C(0)
/** Original scalar constant STATUS_FLAG_FOC_ENABLED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_STATUS_FLAG_FOC_ENABLED UINT32_C(1)
/** Original scalar constant STATUS_FLAG_CALIBRATION_REQUESTED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_STATUS_FLAG_CALIBRATION_REQUESTED UINT32_C(2)
/** Original scalar constant STATUS_FLAG_CALIBRATION_CANCELLED=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_STATUS_FLAG_CALIBRATION_CANCELLED UINT32_C(3)
/** Original scalar constant STATUS_FLAG_ESTOPPED=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_STATUS_FLAG_ESTOPPED UINT32_C(4)
/** Original scalar constant WARNING_FLAG_MOTOR_THERMISTOR=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_MOTOR_THERMISTOR UINT32_C(0)
/** Original scalar constant WARNING_FLAG_PCB_THERMISTOR=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_PCB_THERMISTOR UINT32_C(1)
/** Original scalar constant WARNING_FLAG_PHASE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_PHASE UINT32_C(2)
/** Original scalar constant WARNING_FLAG_PHASE_A=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_PHASE_A UINT32_C(3)
/** Original scalar constant WARNING_FLAG_PHASE_B=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_PHASE_B UINT32_C(4)
/** Original scalar constant WARNING_FLAG_PHASE_C=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_PHASE_C UINT32_C(5)
/** Original scalar constant WARNING_FLAG_ENCODER_INDEX=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_ENCODER_INDEX UINT32_C(6)
/** Original scalar constant WARNING_FLAG_ENCODER_OUTPUT_A=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_ENCODER_OUTPUT_A UINT32_C(7)
/** Original scalar constant WARNING_FLAG_ENCODER_OUTPUT_B=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_WARNING_FLAG_ENCODER_OUTPUT_B UINT32_C(8)
/** Original scalar constant ERROR_FLAG_NOT_CALIBRATED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_NOT_CALIBRATED UINT32_C(0)
/** Original scalar constant ERROR_FLAG_MOTOR_FAULT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_MOTOR_FAULT UINT32_C(1)
/** Original scalar constant ERROR_FLAG_MOTOR_STALLING=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_MOTOR_STALLING UINT32_C(2)
/** Original scalar constant ERROR_FLAG_MOTOR_THERMISTOR=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_MOTOR_THERMISTOR UINT32_C(3)
/** Original scalar constant ERROR_FLAG_PCB_THERMISTOR=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_PCB_THERMISTOR UINT32_C(4)
/** Original scalar constant ERROR_FLAG_PHASE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_PHASE UINT32_C(5)
/** Original scalar constant ERROR_FLAG_PHASE_A=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_PHASE_A UINT32_C(6)
/** Original scalar constant ERROR_FLAG_PHASE_B=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_PHASE_B UINT32_C(7)
/** Original scalar constant ERROR_FLAG_PHASE_C=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_PHASE_C UINT32_C(8)
/** Original scalar constant ERROR_FLAG_ENCODER_POWER=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_ENCODER_POWER UINT32_C(9)
/** Original scalar constant ERROR_FLAG_ENCODER_INDEX=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_ENCODER_INDEX UINT32_C(10)
/** Original scalar constant ERROR_FLAG_ENCODER_OUTPUT_A=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_ENCODER_OUTPUT_A UINT32_C(11)
/** Original scalar constant ERROR_FLAG_ENCODER_OUTPUT_B=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSTATUS_ERROR_FLAG_ENCODER_OUTPUT_B UINT32_C(12)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus const* value) {
  if(!value)return 0;
  if(value->joint_name.size>256u||!uwvm_bounded_utf8(value->joint_name.data,value->joint_name.size))return 0;
  if(value->firmware_version.size>256u||!uwvm_bounded_utf8(value->firmware_version.data,value->firmware_version.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->motor_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mcu_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pcb_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->stopped>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus const* value) {size_t n=0;
  n+=1;
  n+=4+value->joint_name.size;
  n+=4+value->firmware_version.size;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->can_id,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->joint_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_name.data,value->joint_name.size);*offset+=value->joint_name.size;
  uwvm_bounded_put(p+*offset,value->firmware_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->firmware_version.data,value->firmware_version.size);*offset+=value->firmware_version.size;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->motor_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mcu_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pcb_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->stopped,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->status_flags,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->warning_flags,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_flags,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->can_id,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->joint_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_name.size>256u||value->joint_name.size>n-*offset)return 0;uwvm_bounded_copy(value->joint_name.data,p+*offset,value->joint_name.size);*offset+=value->joint_name.size;
  if(*offset>n||n-*offset<4)return 0;value->firmware_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->firmware_version.size>256u||value->firmware_version.size>n-*offset)return 0;uwvm_bounded_copy(value->firmware_version.data,p+*offset,value->firmware_version.size);*offset+=value->firmware_version.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->motor_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mcu_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pcb_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stopped,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->status_flags,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->warning_flags,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error_flags,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<34u||length>546u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus data[4];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_drivers_sequence;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxMultiStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field drivers; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_drivers_sequence drivers;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTISTATUS_MAX_WIRE_BYTES 2328u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTISTATUS_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXMULTISTATUS_CODEC_IDENTITY "UWVMROS1_595cd0847b4332e14cc067b916166e9a6e1d1e12a7a68cdb7e1cd414a9ea9ce1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_valid(&value->drivers.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  n+=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_wire_size(&value->drivers.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->drivers.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_write_fields(&value->drivers.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->drivers.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxStatus_read_fields(p,n,offset,&value->drivers.data[i]))return 0;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>2328u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMultiStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection data[2];} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_motor_states_sequence;
/** Complete ROS2 clearpath_motor_msgs/msg/LynxSystemProtection value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field system_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t system_state;
  /** Declared field motor_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_motor_states_sequence motor_states;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSYSTEMPROTECTION_MAX_WIRE_BYTES 669u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSYSTEMPROTECTION_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_LYNXSYSTEMPROTECTION_CODEC_IDENTITY "UWVMROS1_4d662de03c34bb633a122399be1a1aaea6187550c83231bf6d8a06b95fa82d9f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->motor_states.size>2u)return 0;
  {size_t i;for(i=0;i<value->motor_states.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_valid(&value->motor_states.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=1;
  n+=4;
  if(value->motor_states.size>2u)return 0;
  {size_t i;for(i=0;i<value->motor_states.size;++i){
  n+=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_wire_size(&value->motor_states.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->system_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->motor_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_states.size;++i){
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_write_fields(&value->motor_states.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->system_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->motor_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_states.size>2u)return 0;
  {size_t i;for(i=0;i<value->motor_states.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxMotorProtection_read_fields(p,n,offset,&value->motor_states.data[i]))return 0;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>669u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_LynxSystemProtection_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_motor_msgs/msg/PumaFeedback.device_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_device_name_text;
/** Complete ROS2 clearpath_motor_msgs/msg/PumaFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback {
  /** Declared field device_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t device_number;
  /** Declared field device_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_device_name_text device_name;
  /** Declared field duty_cycle; canonical capacity is part of CODEC_IDENTITY. */
  float duty_cycle;
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  float current;
  /** Declared field travel; canonical capacity is part of CODEC_IDENTITY. */
  double travel;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  double speed;
  /** Declared field setpoint; canonical capacity is part of CODEC_IDENTITY. */
  double setpoint;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAFEEDBACK_MAX_WIRE_BYTES 293u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAFEEDBACK_MIN_WIRE_BYTES 37u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAFEEDBACK_CODEC_IDENTITY "UWVMROS1_c020162b07a55ac5f41df6021c0d3053d7302863f4e544a738b6cc5bc4dc9659"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback const* value) {
  if(!value)return 0;
  if(value->device_name.size>256u||!uwvm_bounded_utf8(value->device_name.data,value->device_name.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->duty_cycle,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->travel,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->setpoint,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback const* value) {size_t n=0;
  n+=1;
  n+=4+value->device_name.size;
  n+=4;
  n+=4;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->device_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->device_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->device_name.data,value->device_name.size);*offset+=value->device_name.size;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->duty_cycle,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->travel,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->setpoint,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->device_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->device_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->device_name.size>256u||value->device_name.size>n-*offset)return 0;uwvm_bounded_copy(value->device_name.data,p+*offset,value->device_name.size);*offset+=value->device_name.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->duty_cycle,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->travel,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->speed,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->setpoint,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<37u||length>293u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback data[4];} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_drivers_feedback_sequence;
/** Complete ROS2 clearpath_motor_msgs/msg/PumaMultiFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field drivers_feedback; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_drivers_feedback_sequence drivers_feedback;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAMULTIFEEDBACK_MAX_WIRE_BYTES 1316u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAMULTIFEEDBACK_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAMULTIFEEDBACK_CODEC_IDENTITY "UWVMROS1_638309564580ad23d585799dcd46156e1e4261ebf9febfbb91754c250907c947"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->drivers_feedback.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers_feedback.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_valid(&value->drivers_feedback.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->drivers_feedback.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers_feedback.size;++i){
  n+=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_wire_size(&value->drivers_feedback.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->drivers_feedback.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->drivers_feedback.size;++i){
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_write_fields(&value->drivers_feedback.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->drivers_feedback.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->drivers_feedback.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers_feedback.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaFeedback_read_fields(p,n,offset,&value->drivers_feedback.data[i]))return 0;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>1316u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_motor_msgs/msg/PumaStatus.device_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_device_name_text;
/** Complete ROS2 clearpath_motor_msgs/msg/PumaStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus {
  /** Declared field device_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t device_number;
  /** Declared field device_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_device_name_text device_name;
  /** Declared field bus_voltage; canonical capacity is part of CODEC_IDENTITY. */
  float bus_voltage;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  float temperature;
  /** Declared field output_voltage; canonical capacity is part of CODEC_IDENTITY. */
  float output_voltage;
  /** Declared field analog_input; canonical capacity is part of CODEC_IDENTITY. */
  float analog_input;
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field fault; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t fault;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_MAX_WIRE_BYTES 279u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_MIN_WIRE_BYTES 23u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_CODEC_IDENTITY "UWVMROS1_a027229db2c8500f707d97064548b27e77e24ae4840ad9722d2dc74726099da8"
/** Original scalar constant MODE_VOLTAGE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_MODE_VOLTAGE UINT32_C(0)
/** Original scalar constant MODE_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_MODE_CURRENT UINT32_C(1)
/** Original scalar constant MODE_SPEED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_MODE_SPEED UINT32_C(2)
/** Original scalar constant MODE_POSITION=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_MODE_POSITION UINT32_C(3)
/** Original scalar constant MODE_VCOMP=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_MODE_VCOMP UINT32_C(4)
/** Original scalar constant FAULT_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_FAULT_CURRENT UINT32_C(1)
/** Original scalar constant FAULT_TEMPERATURE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_FAULT_TEMPERATURE UINT32_C(2)
/** Original scalar constant FAULT_BUS_VOLTAGE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_FAULT_BUS_VOLTAGE UINT32_C(4)
/** Original scalar constant FAULT_BRIDGE_DRIVER=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMASTATUS_FAULT_BRIDGE_DRIVER UINT32_C(8)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus const* value) {
  if(!value)return 0;
  if(value->device_name.size>256u||!uwvm_bounded_utf8(value->device_name.data,value->device_name.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bus_voltage,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->output_voltage,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->analog_input,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus const* value) {size_t n=0;
  n+=1;
  n+=4+value->device_name.size;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->device_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->device_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->device_name.data,value->device_name.size);*offset+=value->device_name.size;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bus_voltage,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->output_voltage,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->analog_input,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->fault,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->device_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->device_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->device_name.size>256u||value->device_name.size>n-*offset)return 0;uwvm_bounded_copy(value->device_name.data,p+*offset,value->device_name.size);*offset+=value->device_name.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bus_voltage,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->output_voltage,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->analog_input,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->fault,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<23u||length>279u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus data[4];} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_drivers_sequence;
/** Complete ROS2 clearpath_motor_msgs/msg/PumaMultiStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field drivers; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_drivers_sequence drivers;
} roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus;
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAMULTISTATUS_MAX_WIRE_BYTES 1260u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAMULTISTATUS_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_CLEARPATH_MOTOR_MSGS_MSG_PUMAMULTISTATUS_CODEC_IDENTITY "UWVMROS1_30a0210fea4fffe07371c898c365ef9696501a7541935dd80770671a7163c8f2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_valid(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_valid(&value->drivers.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_wire_size(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  n+=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_wire_size(&value->drivers.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_write_fields(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->drivers.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_write_fields(&value->drivers.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->drivers.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->drivers.size>4u)return 0;
  {size_t i;for(i=0;i<value->drivers.size;++i){
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaStatus_read_fields(p,n,offset,&value->drivers.data[i]))return 0;
  }}
  return roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_encode(roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus* value) {
  roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>1260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_motor_msgs_msg_PumaMultiStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_platform_msgs/msg/DisplayStatus.string1; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[49];} roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_string1_text;
/** UTF-8 byte storage for clearpath_platform_msgs/msg/DisplayStatus.string2; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[49];} roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_string2_text;
/** Complete ROS2 clearpath_platform_msgs/msg/DisplayStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus {
  /** Declared field string1; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_string1_text string1;
  /** Declared field string2; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_string2_text string2;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DISPLAYSTATUS_MAX_WIRE_BYTES 106u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DISPLAYSTATUS_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DISPLAYSTATUS_CODEC_IDENTITY "UWVMROS1_e4585117414581491304a26d528b44c770799f622c3f1cde037078f5cb2462c2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus const* value) {
  if(!value)return 0;
  if(value->string1.size>49u||!uwvm_bounded_utf8(value->string1.data,value->string1.size))return 0;
  if(value->string2.size>49u||!uwvm_bounded_utf8(value->string2.data,value->string2.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus const* value) {size_t n=0;
  n+=4+value->string1.size;
  n+=4+value->string2.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->string1.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->string1.data,value->string1.size);*offset+=value->string1.size;
  uwvm_bounded_put(p+*offset,value->string2.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->string2.data,value->string2.size);*offset+=value->string2.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus* value) {
  if(*offset>n||n-*offset<4)return 0;value->string1.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->string1.size>49u||value->string1.size>n-*offset)return 0;uwvm_bounded_copy(value->string1.data,p+*offset,value->string1.size);*offset+=value->string1.size;
  if(*offset>n||n-*offset<4)return 0;value->string2.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->string2.size>49u||value->string2.size>n-*offset)return 0;uwvm_bounded_copy(value->string2.data,p+*offset,value->string2.size);*offset+=value->string2.size;
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>106u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_DisplayStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 clearpath_platform_msgs/msg/Drive value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t mode;
  /** Declared field drivers; canonical capacity is part of CODEC_IDENTITY. */
  float drivers[2];
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_MAX_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_CODEC_IDENTITY "UWVMROS1_be8664128ff89a066550f1a2496c30396f39bafd1d96483c545048a328138785"
/** Original scalar constant MODE_VELOCITY=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_MODE_VELOCITY INT32_C(0)
/** Original scalar constant MODE_PWM=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_MODE_PWM INT32_C(1)
/** Original scalar constant MODE_NONE=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_MODE_NONE -INT32_C(1)
/** Original scalar constant LEFT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_LEFT INT32_C(0)
/** Original scalar constant RIGHT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVE_RIGHT INT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->drivers[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive const* value) {size_t n=0;
  n+=1;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->drivers[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->drivers[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>9u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Drive_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 clearpath_platform_msgs/msg/DriveFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback {
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  float current;
  /** Declared field duty_cycle; canonical capacity is part of CODEC_IDENTITY. */
  float duty_cycle;
  /** Declared field bridge_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float bridge_temperature;
  /** Declared field motor_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float motor_temperature;
  /** Declared field measured_velocity; canonical capacity is part of CODEC_IDENTITY. */
  float measured_velocity;
  /** Declared field measured_travel; canonical capacity is part of CODEC_IDENTITY. */
  float measured_travel;
  /** Declared field driver_fault; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t driver_fault;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVEFEEDBACK_MAX_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVEFEEDBACK_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_DRIVEFEEDBACK_CODEC_IDENTITY "UWVMROS1_008f51d4c8341723ffec72651f93c00db0f9feceb037ee7b6b460ffcafd3f287"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->duty_cycle,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bridge_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->motor_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_velocity,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_travel,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->driver_fault>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->duty_cycle,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bridge_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->motor_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_velocity,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_travel,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->driver_fault,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->duty_cycle,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bridge_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->motor_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->measured_velocity,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->measured_travel,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->driver_fault,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_fans_sequence;
/** Complete ROS2 clearpath_platform_msgs/msg/Fans value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans {
  /** Declared field fans; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_fans_sequence fans;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CODEC_IDENTITY "UWVMROS1_a7d96f2242d992baf96f31bd8f93088547f92c72912c241c2d237f7714800cc8"
/** Original scalar constant CC01_FAN1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN1 UINT32_C(0)
/** Original scalar constant CC01_FAN2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN2 UINT32_C(1)
/** Original scalar constant CC01_FAN3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN3 UINT32_C(2)
/** Original scalar constant CC01_FAN4=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN4 UINT32_C(3)
/** Original scalar constant CC01_FAN5=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN5 UINT32_C(4)
/** Original scalar constant CC01_FAN6=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN6 UINT32_C(5)
/** Original scalar constant CC01_FAN7=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN7 UINT32_C(6)
/** Original scalar constant CC01_FAN8=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CC01_FAN8 UINT32_C(7)
/** Original scalar constant R100_EQUIPMENT_BAY_INTAKE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_R100_EQUIPMENT_BAY_INTAKE UINT32_C(0)
/** Original scalar constant R100_EQUIPMENT_BAY_EXHAUST=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_R100_EQUIPMENT_BAY_EXHAUST UINT32_C(1)
/** Original scalar constant R100_CHARGER_BAY_INTAKE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_R100_CHARGER_BAY_INTAKE UINT32_C(2)
/** Original scalar constant R100_CHARGER_BAY_EXHAUST=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_R100_CHARGER_BAY_EXHAUST UINT32_C(3)
/** Original scalar constant R100_USER_BAY_INTAKE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_R100_USER_BAY_INTAKE UINT32_C(4)
/** Original scalar constant R100_USER_BAY_EXHAUST=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_R100_USER_BAY_EXHAUST UINT32_C(5)
/** Original scalar constant FAN_OFF=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_FAN_OFF UINT32_C(0)
/** Original scalar constant FAN_ON_HIGH=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_FAN_ON_HIGH UINT32_C(1)
/** Original scalar constant FAN_ON_LOW=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_FAN_ON_LOW UINT32_C(2)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans const* value) {
  if(!value)return 0;
  if(value->fans.size>2u)return 0;
  {size_t i;for(i=0;i<value->fans.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans const* value) {size_t n=0;
  n+=4;
  if(value->fans.size>2u)return 0;
  {size_t i;for(i=0;i<value->fans.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->fans.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->fans.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->fans.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans* value) {
  if(*offset>n||n-*offset<4)return 0;value->fans.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->fans.size>2u)return 0;
  {size_t i;for(i=0;i<value->fans.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->fans.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Fans_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 clearpath_platform_msgs/msg/Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field drivers; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback drivers[2];
  /** Declared field commanded_mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t commanded_mode;
  /** Declared field actual_mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t actual_mode;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FEEDBACK_MAX_WIRE_BYTES 192u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FEEDBACK_MIN_WIRE_BYTES 64u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FEEDBACK_CODEC_IDENTITY "UWVMROS1_15fdfd2e12df38bab09bf083beb61ce23d3c1082e794c34d51eec4c473e29442"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  {size_t i;for(i=0;i<2u;++i){
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_valid(&value->drivers[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  {size_t i;for(i=0;i<2u;++i){
  n+=roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_wire_size(&value->drivers[i]);
  }}
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  {size_t i;for(i=0;i<2u;++i){
  roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_write_fields(&value->drivers[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->commanded_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->actual_mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  {size_t i;for(i=0;i<2u;++i){
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_DriveFeedback_read_fields(p,n,offset,&value->drivers[i]))return 0;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->commanded_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->actual_mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<64u||length>192u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 clearpath_platform_msgs/msg/RGB value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB {
  /** Declared field red; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t red;
  /** Declared field green; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t green;
  /** Declared field blue; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t blue;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_RGB_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_RGB_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_RGB_CODEC_IDENTITY "UWVMROS1_42db8731293054ef44002afc54c16dac45544354932d5353cf749fb376cef1a1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->red,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->green,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->blue,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->red,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->green,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->blue,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_lights_sequence;
/** Complete ROS2 clearpath_platform_msgs/msg/Lights value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights {
  /** Declared field lights; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_lights_sequence lights;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_MAX_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_CODEC_IDENTITY "UWVMROS1_3f975a3ec1fca4f8add9523d4f9f99963be520c0e2982e644551b71cbd4b8a27"
/** Original scalar constant D100_LIGHTS_REAR_LEFT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D100_LIGHTS_REAR_LEFT UINT32_C(0)
/** Original scalar constant D100_LIGHTS_FRONT_LEFT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D100_LIGHTS_FRONT_LEFT UINT32_C(1)
/** Original scalar constant D100_LIGHTS_FRONT_RIGHT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D100_LIGHTS_FRONT_RIGHT UINT32_C(2)
/** Original scalar constant D100_LIGHTS_REAR_RIGHT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D100_LIGHTS_REAR_RIGHT UINT32_C(3)
/** Original scalar constant D150_LIGHTS_REAR_LEFT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D150_LIGHTS_REAR_LEFT UINT32_C(0)
/** Original scalar constant D150_LIGHTS_FRONT_LEFT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D150_LIGHTS_FRONT_LEFT UINT32_C(1)
/** Original scalar constant D150_LIGHTS_FRONT_RIGHT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D150_LIGHTS_FRONT_RIGHT UINT32_C(2)
/** Original scalar constant D150_LIGHTS_REAR_RIGHT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_D150_LIGHTS_REAR_RIGHT UINT32_C(3)
/** Original scalar constant R100_LIGHTS_FRONT_PORT_UPPER=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_FRONT_PORT_UPPER UINT32_C(0)
/** Original scalar constant R100_LIGHTS_FRONT_PORT_LOWER=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_FRONT_PORT_LOWER UINT32_C(1)
/** Original scalar constant R100_LIGHTS_FRONT_STARBOARD_UPPER=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_FRONT_STARBOARD_UPPER UINT32_C(2)
/** Original scalar constant R100_LIGHTS_FRONT_STARBOARD_LOWER=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_FRONT_STARBOARD_LOWER UINT32_C(3)
/** Original scalar constant R100_LIGHTS_REAR_PORT_UPPER=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_REAR_PORT_UPPER UINT32_C(4)
/** Original scalar constant R100_LIGHTS_REAR_PORT_LOWER=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_REAR_PORT_LOWER UINT32_C(5)
/** Original scalar constant R100_LIGHTS_REAR_STARBOARD_UPPER=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_REAR_STARBOARD_UPPER UINT32_C(6)
/** Original scalar constant R100_LIGHTS_REAR_STARBOARD_LOWER=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_R100_LIGHTS_REAR_STARBOARD_LOWER UINT32_C(7)
/** Original scalar constant W200_LIGHTS_FRONT_LEFT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_W200_LIGHTS_FRONT_LEFT UINT32_C(0)
/** Original scalar constant W200_LIGHTS_FRONT_RIGHT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_W200_LIGHTS_FRONT_RIGHT UINT32_C(1)
/** Original scalar constant W200_LIGHTS_REAR_LEFT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_W200_LIGHTS_REAR_LEFT UINT32_C(2)
/** Original scalar constant W200_LIGHTS_REAR_RIGHT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_W200_LIGHTS_REAR_RIGHT UINT32_C(3)
/** Original scalar constant A300_LIGHTS_FRONT_RIGHT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_A300_LIGHTS_FRONT_RIGHT UINT32_C(0)
/** Original scalar constant A300_LIGHTS_FRONT_LEFT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_A300_LIGHTS_FRONT_LEFT UINT32_C(1)
/** Original scalar constant A300_LIGHTS_REAR_LEFT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_A300_LIGHTS_REAR_LEFT UINT32_C(2)
/** Original scalar constant A300_LIGHTS_REAR_RIGHT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_A300_LIGHTS_REAR_RIGHT UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights const* value) {
  if(!value)return 0;
  if(value->lights.size>2u)return 0;
  {size_t i;for(i=0;i<value->lights.size;++i){
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_valid(&value->lights.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights const* value) {size_t n=0;
  n+=4;
  if(value->lights.size>2u)return 0;
  {size_t i;for(i=0;i<value->lights.size;++i){
  n+=roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_wire_size(&value->lights.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->lights.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->lights.size;++i){
  roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_write_fields(&value->lights.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights* value) {
  if(*offset>n||n-*offset<4)return 0;value->lights.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->lights.size>2u)return 0;
  {size_t i;for(i=0;i<value->lights.size;++i){
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_RGB_read_fields(p,n,offset,&value->lights.data[i]))return 0;
  }}
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>10u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Lights_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_rails_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint32_t data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_outputs_sequence;
/** Complete ROS2 clearpath_platform_msgs/msg/PinoutCommand value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand {
  /** Declared field rails; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_rails_sequence rails;
  /** Declared field outputs; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_outputs_sequence outputs;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_MAX_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CODEC_IDENTITY "UWVMROS1_b26c2baf1e64cefa889c4ac6bd6b7f4647d9247eca0626b83cd726547a4e80ff"
/** Original scalar constant CC01_USER_PWR_CTRL=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_USER_PWR_CTRL UINT32_C(0)
/** Original scalar constant D100_AUX_OUT1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_D100_AUX_OUT1 UINT32_C(0)
/** Original scalar constant D100_AUX_OUT2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_D100_AUX_OUT2 UINT32_C(1)
/** Original scalar constant D100_AUX_OUT3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_D100_AUX_OUT3 UINT32_C(2)
/** Original scalar constant D150_AUX_OUT1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_D150_AUX_OUT1 UINT32_C(0)
/** Original scalar constant D150_AUX_OUT2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_D150_AUX_OUT2 UINT32_C(1)
/** Original scalar constant D150_AUX_OUT3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_D150_AUX_OUT3 UINT32_C(2)
/** Original scalar constant CC01_AUX_OUT1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_AUX_OUT1 UINT32_C(0)
/** Original scalar constant CC01_AUX_OUT2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_AUX_OUT2 UINT32_C(1)
/** Original scalar constant CC01_AUX_OUT3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_AUX_OUT3 UINT32_C(2)
/** Original scalar constant CC01_GPO0=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_GPO0 UINT32_C(3)
/** Original scalar constant CC01_GPO1=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_GPO1 UINT32_C(4)
/** Original scalar constant CC01_GPO2=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_GPO2 UINT32_C(5)
/** Original scalar constant CC01_GPO3=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTCOMMAND_CC01_GPO3 UINT32_C(6)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand const* value) {
  if(!value)return 0;
  if(value->rails.size>2u)return 0;
  {size_t i;for(i=0;i<value->rails.size;++i){
  if(value->rails.data[i]>1)return 0;
  }}
  if(value->outputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand const* value) {size_t n=0;
  n+=4;
  if(value->rails.size>2u)return 0;
  {size_t i;for(i=0;i<value->rails.size;++i){
  n+=1;
  }}
  n+=4;
  if(value->outputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->rails.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->rails.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->rails.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,value->outputs.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->outputs.data[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand* value) {
  if(*offset>n||n-*offset<4)return 0;value->rails.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->rails.size>2u)return 0;
  {size_t i;for(i=0;i<value->rails.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rails.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;value->outputs.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->outputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->outputs.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>18u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutCommand_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_rails_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_inputs_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_outputs_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint32_t data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_output_periods_sequence;
/** Complete ROS2 clearpath_platform_msgs/msg/PinoutState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field rails; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_rails_sequence rails;
  /** Declared field inputs; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_inputs_sequence inputs;
  /** Declared field outputs; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_outputs_sequence outputs;
  /** Declared field output_periods; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_output_periods_sequence output_periods;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_MAX_WIRE_BYTES 170u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CODEC_IDENTITY "UWVMROS1_a936c6d91440709d50fa9b0be8fb567a9ce793e84f57e5379f6a160c8cbf73e7"
/** Original scalar constant CC01_USER_PWR_CTRL=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_USER_PWR_CTRL UINT32_C(0)
/** Original scalar constant D100_AUX_IN1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D100_AUX_IN1 UINT32_C(0)
/** Original scalar constant D100_AUX_IN2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D100_AUX_IN2 UINT32_C(1)
/** Original scalar constant D100_AUX_IN3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D100_AUX_IN3 UINT32_C(2)
/** Original scalar constant D150_AUX_IN1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D150_AUX_IN1 UINT32_C(0)
/** Original scalar constant D150_AUX_IN2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D150_AUX_IN2 UINT32_C(1)
/** Original scalar constant D150_AUX_IN3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D150_AUX_IN3 UINT32_C(2)
/** Original scalar constant CC01_AUX_IN1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_AUX_IN1 UINT32_C(0)
/** Original scalar constant CC01_AUX_IN2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_AUX_IN2 UINT32_C(1)
/** Original scalar constant CC01_AUX_IN3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_AUX_IN3 UINT32_C(2)
/** Original scalar constant CC01_GPI0=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPI0 UINT32_C(3)
/** Original scalar constant CC01_GPI1=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPI1 UINT32_C(4)
/** Original scalar constant CC01_GPI2=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPI2 UINT32_C(5)
/** Original scalar constant CC01_GPI3=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPI3 UINT32_C(6)
/** Original scalar constant D100_AUX_OUT1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D100_AUX_OUT1 UINT32_C(0)
/** Original scalar constant D100_AUX_OUT2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D100_AUX_OUT2 UINT32_C(1)
/** Original scalar constant D100_AUX_OUT3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D100_AUX_OUT3 UINT32_C(2)
/** Original scalar constant D150_AUX_OUT1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D150_AUX_OUT1 UINT32_C(0)
/** Original scalar constant D150_AUX_OUT2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D150_AUX_OUT2 UINT32_C(1)
/** Original scalar constant D150_AUX_OUT3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_D150_AUX_OUT3 UINT32_C(2)
/** Original scalar constant CC01_AUX_OUT1=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_AUX_OUT1 UINT32_C(0)
/** Original scalar constant CC01_AUX_OUT2=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_AUX_OUT2 UINT32_C(1)
/** Original scalar constant CC01_AUX_OUT3=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_AUX_OUT3 UINT32_C(2)
/** Original scalar constant CC01_GPO0=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPO0 UINT32_C(3)
/** Original scalar constant CC01_GPO1=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPO1 UINT32_C(4)
/** Original scalar constant CC01_GPO2=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPO2 UINT32_C(5)
/** Original scalar constant CC01_GPO3=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_PINOUTSTATE_CC01_GPO3 UINT32_C(6)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->rails.size>2u)return 0;
  {size_t i;for(i=0;i<value->rails.size;++i){
  if(value->rails.data[i]>1)return 0;
  }}
  if(value->inputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->inputs.size;++i){
  if(value->inputs.data[i]>1)return 0;
  }}
  if(value->outputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  if(value->outputs.data[i]>1)return 0;
  }}
  if(value->output_periods.size>2u)return 0;
  {size_t i;for(i=0;i<value->output_periods.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->rails.size>2u)return 0;
  {size_t i;for(i=0;i<value->rails.size;++i){
  n+=1;
  }}
  n+=4;
  if(value->inputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->inputs.size;++i){
  n+=1;
  }}
  n+=4;
  if(value->outputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  n+=1;
  }}
  n+=4;
  if(value->output_periods.size>2u)return 0;
  {size_t i;for(i=0;i<value->output_periods.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->rails.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->rails.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->rails.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,value->inputs.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->inputs.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->inputs.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,value->outputs.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->outputs.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,value->output_periods.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->output_periods.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->output_periods.data[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->rails.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->rails.size>2u)return 0;
  {size_t i;for(i=0;i<value->rails.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rails.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;value->inputs.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->inputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->inputs.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->inputs.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;value->outputs.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->outputs.size>2u)return 0;
  {size_t i;for(i=0;i<value->outputs.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->outputs.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;value->output_periods.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->output_periods.size>2u)return 0;
  {size_t i;for(i=0;i<value->output_periods.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->output_periods.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>170u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_PinoutState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_measured_voltages_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_measured_currents_sequence;
/** Complete ROS2 clearpath_platform_msgs/msg/Power value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_Power {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field shore_power_connected; canonical capacity is part of CODEC_IDENTITY. */
  int8_t shore_power_connected;
  /** Declared field battery_connected; canonical capacity is part of CODEC_IDENTITY. */
  int8_t battery_connected;
  /** Declared field power_12v_user_nominal; canonical capacity is part of CODEC_IDENTITY. */
  int8_t power_12v_user_nominal;
  /** Declared field charger_connected; canonical capacity is part of CODEC_IDENTITY. */
  int8_t charger_connected;
  /** Declared field charging_complete; canonical capacity is part of CODEC_IDENTITY. */
  int8_t charging_complete;
  /** Declared field measured_voltages; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_measured_voltages_sequence measured_voltages;
  /** Declared field measured_currents; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_measured_currents_sequence measured_currents;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Power;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_MAX_WIRE_BYTES 169u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CODEC_IDENTITY "UWVMROS1_3fc241d6b81da2f80bfce22933b81786b5e582715dc66cd238fa40b239e7aecc"
/** Original scalar constant NOT_APPLICABLE=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_NOT_APPLICABLE -INT32_C(1)
/** Original scalar constant J100_MEASURED_BATTERY=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_J100_MEASURED_BATTERY UINT32_C(0)
/** Original scalar constant J100_MEASURED_5V=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_J100_MEASURED_5V UINT32_C(1)
/** Original scalar constant J100_MEASURED_12V=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_J100_MEASURED_12V UINT32_C(2)
/** Original scalar constant D100_MEASURED_BATTERY=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D100_MEASURED_BATTERY UINT32_C(0)
/** Original scalar constant D100_MEASURED_5V=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D100_MEASURED_5V UINT32_C(1)
/** Original scalar constant D100_MEASURED_12V=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D100_MEASURED_12V UINT32_C(2)
/** Original scalar constant D150_MEASURED_BATTERY=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D150_MEASURED_BATTERY UINT32_C(0)
/** Original scalar constant D150_MEASURED_5V=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D150_MEASURED_5V UINT32_C(1)
/** Original scalar constant D150_MEASURED_12V=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D150_MEASURED_12V UINT32_C(2)
/** Original scalar constant W200_MEASURED_BATTERY=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_MEASURED_BATTERY UINT32_C(0)
/** Original scalar constant W200_MEASURED_12V=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_MEASURED_12V UINT32_C(1)
/** Original scalar constant W200_MEASURED_24V=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_MEASURED_24V UINT32_C(2)
/** Original scalar constant W200_MEASURED_48V=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_MEASURED_48V UINT32_C(3)
/** Original scalar constant R100_MEASURED_BATTERY=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_MEASURED_BATTERY UINT32_C(0)
/** Original scalar constant R100_MEASURED_5V=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_MEASURED_5V UINT32_C(1)
/** Original scalar constant R100_MEASURED_12V=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_MEASURED_12V UINT32_C(2)
/** Original scalar constant R100_MEASURED_INVERTER=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_MEASURED_INVERTER UINT32_C(3)
/** Original scalar constant R100_MEASURED_FRONT_AXLE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_MEASURED_FRONT_AXLE UINT32_C(4)
/** Original scalar constant R100_MEASURED_REAR_AXLE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_MEASURED_REAR_AXLE UINT32_C(5)
/** Original scalar constant R100_MEASURED_LIGHT=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_MEASURED_LIGHT UINT32_C(6)
/** Original scalar constant A200_BATTERY_VOLTAGE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_BATTERY_VOLTAGE UINT32_C(0)
/** Original scalar constant A200_LEFT_DRIVER_VOLTAGE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_LEFT_DRIVER_VOLTAGE UINT32_C(1)
/** Original scalar constant A200_RIGHT_DRIVER_VOLTAGE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_RIGHT_DRIVER_VOLTAGE UINT32_C(2)
/** Original scalar constant A200_VOLTAGES_SIZE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_VOLTAGES_SIZE UINT32_C(3)
/** Original scalar constant CC01_BATTERY_VOLTAGE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BATTERY_VOLTAGE UINT32_C(0)
/** Original scalar constant CC01_USER_BATTERY_VOLTAGE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_USER_BATTERY_VOLTAGE UINT32_C(1)
/** Original scalar constant CC01_USER_24V_VOLTAGE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_USER_24V_VOLTAGE UINT32_C(2)
/** Original scalar constant CC01_USER_12VB_VOLTAGE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_USER_12VB_VOLTAGE UINT32_C(3)
/** Original scalar constant CC01_SYSTEM_12V_VOLTAGE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_SYSTEM_12V_VOLTAGE UINT32_C(4)
/** Original scalar constant CC01_EXPANSION_VOLTAGE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_EXPANSION_VOLTAGE UINT32_C(5)
/** Original scalar constant CC01_BREAKOUT_24V_AUX_VOLTAGE=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_24V_AUX_VOLTAGE UINT32_C(6)
/** Original scalar constant CC01_BREAKOUT_12V_AUX_VOLTAGE=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_12V_AUX_VOLTAGE UINT32_C(7)
/** Original scalar constant CC01_BREAKOUT_USER_12VA_VOLTAGE=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_12VA_VOLTAGE UINT32_C(8)
/** Original scalar constant CC01_BREAKOUT_LYNX1_VOLTAGE=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_LYNX1_VOLTAGE UINT32_C(9)
/** Original scalar constant CC01_BREAKOUT_LYNX2_VOLTAGE=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_LYNX2_VOLTAGE UINT32_C(10)
/** Original scalar constant CC01_BREAKOUT_LYNX3_VOLTAGE=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_LYNX3_VOLTAGE UINT32_C(11)
/** Original scalar constant CC01_BREAKOUT_LYNX4_VOLTAGE=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_LYNX4_VOLTAGE UINT32_C(12)
/** Original scalar constant J100_TOTAL_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_J100_TOTAL_CURRENT UINT32_C(0)
/** Original scalar constant J100_COMPUTER_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_J100_COMPUTER_CURRENT UINT32_C(1)
/** Original scalar constant J100_DRIVE_CURRENT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_J100_DRIVE_CURRENT UINT32_C(2)
/** Original scalar constant J100_USER_CURRENT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_J100_USER_CURRENT UINT32_C(3)
/** Original scalar constant D100_TOTAL_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D100_TOTAL_CURRENT UINT32_C(0)
/** Original scalar constant D100_COMPUTER_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D100_COMPUTER_CURRENT UINT32_C(1)
/** Original scalar constant D150_TOTAL_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D150_TOTAL_CURRENT UINT32_C(0)
/** Original scalar constant D150_COMPUTER_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_D150_COMPUTER_CURRENT UINT32_C(1)
/** Original scalar constant W200_TOTAL_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_TOTAL_CURRENT UINT32_C(0)
/** Original scalar constant W200_COMPUTER_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_COMPUTER_CURRENT UINT32_C(1)
/** Original scalar constant W200_12V_CURRENT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_12V_CURRENT UINT32_C(2)
/** Original scalar constant W200_24V_CURRENT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_W200_24V_CURRENT UINT32_C(3)
/** Original scalar constant R100_TOTAL_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_R100_TOTAL_CURRENT UINT32_C(0)
/** Original scalar constant A200_MCU_AND_USER_PORT_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_MCU_AND_USER_PORT_CURRENT UINT32_C(0)
/** Original scalar constant A200_LEFT_DRIVER_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_LEFT_DRIVER_CURRENT UINT32_C(1)
/** Original scalar constant A200_RIGHT_DRIVER_CURRENT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_RIGHT_DRIVER_CURRENT UINT32_C(2)
/** Original scalar constant A200_CURRENTS_SIZE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_A200_CURRENTS_SIZE UINT32_C(3)
/** Original scalar constant CC01_BATTERY_PACK_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BATTERY_PACK_CURRENT UINT32_C(0)
/** Original scalar constant CC01_USER_BATTERY_CURRENT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_USER_BATTERY_CURRENT UINT32_C(1)
/** Original scalar constant CC01_AUX_HES_CURRENT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_AUX_HES_CURRENT UINT32_C(2)
/** Original scalar constant CC01_SYSTEM_12V_CURRENT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_SYSTEM_12V_CURRENT UINT32_C(3)
/** Original scalar constant CC01_24V_AUX_CURRENT=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_24V_AUX_CURRENT UINT32_C(4)
/** Original scalar constant CC01_12V_AUX_CURRENT=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_12V_AUX_CURRENT UINT32_C(5)
/** Original scalar constant CC01_BREAKOUT_USER_24V_CURRENT=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_24V_CURRENT UINT32_C(6)
/** Original scalar constant CC01_BREAKOUT_USER_12VA_CURRENT=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_12VA_CURRENT UINT32_C(7)
/** Original scalar constant CC01_BREAKOUT_USER_12VB_CURRENT=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_12VB_CURRENT UINT32_C(8)
/** Original scalar constant CC01_BREAKOUT_USER_LYNX1_CURRENT=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_LYNX1_CURRENT UINT32_C(9)
/** Original scalar constant CC01_BREAKOUT_USER_LYNX2_CURRENT=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_LYNX2_CURRENT UINT32_C(10)
/** Original scalar constant CC01_BREAKOUT_USER_LYNX3_CURRENT=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_LYNX3_CURRENT UINT32_C(11)
/** Original scalar constant CC01_BREAKOUT_USER_LYNX4_CURRENT=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CC01_BREAKOUT_USER_LYNX4_CURRENT UINT32_C(12)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Power const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->measured_voltages.size>2u)return 0;
  {size_t i;for(i=0;i<value->measured_voltages.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_voltages.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->measured_currents.size>2u)return 0;
  {size_t i;for(i=0;i<value->measured_currents.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_currents.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Power const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=4;
  if(value->measured_voltages.size>2u)return 0;
  {size_t i;for(i=0;i<value->measured_voltages.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->measured_currents.size>2u)return 0;
  {size_t i;for(i=0;i<value->measured_currents.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Power const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->shore_power_connected,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->battery_connected,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->power_12v_user_nominal,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->charger_connected,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->charging_complete,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->measured_voltages.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->measured_voltages.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_voltages.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->measured_currents.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->measured_currents.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->measured_currents.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_Power* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->shore_power_connected,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->battery_connected,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->power_12v_user_nominal,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->charger_connected,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->charging_complete,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->measured_voltages.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->measured_voltages.size>2u)return 0;
  {size_t i;for(i=0;i<value->measured_voltages.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->measured_voltages.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->measured_currents.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->measured_currents.size>2u)return 0;
  {size_t i;for(i=0;i<value->measured_currents.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->measured_currents.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_Power const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_Power* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Power next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>169u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Power_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_platform_msgs/msg/Status.hardware_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_hardware_id_text;
/** UTF-8 byte storage for clearpath_platform_msgs/msg/Status.firmware_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_firmware_version_text;
/** Complete ROS2 clearpath_platform_msgs/msg/Status value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_Status {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field hardware_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_hardware_id_text hardware_id;
  /** Declared field firmware_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_firmware_version_text firmware_version;
  /** Declared field mcu_uptime; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration mcu_uptime;
  /** Declared field connection_uptime; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration connection_uptime;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Status;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_MAX_WIRE_BYTES 676u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_CODEC_IDENTITY "UWVMROS1_1bd99cc11ea9efcb51eeba8fc1d599960c16e958a8adf33a56d47fe27be87782"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->hardware_id.size>256u||!uwvm_bounded_utf8(value->hardware_id.data,value->hardware_id.size))return 0;
  if(value->firmware_version.size>256u||!uwvm_bounded_utf8(value->firmware_version.data,value->firmware_version.size))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->mcu_uptime))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->connection_uptime))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4+value->hardware_id.size;
  n+=4+value->firmware_version.size;
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->mcu_uptime);
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->connection_uptime);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->hardware_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->hardware_id.data,value->hardware_id.size);*offset+=value->hardware_id.size;
  uwvm_bounded_put(p+*offset,value->firmware_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->firmware_version.data,value->firmware_version.size);*offset+=value->firmware_version.size;
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->mcu_uptime,p,offset);
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->connection_uptime,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_Status* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->hardware_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->hardware_id.size>256u||value->hardware_id.size>n-*offset)return 0;uwvm_bounded_copy(value->hardware_id.data,p+*offset,value->hardware_id.size);*offset+=value->hardware_id.size;
  if(*offset>n||n-*offset<4)return 0;value->firmware_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->firmware_version.size>256u||value->firmware_version.size>n-*offset)return 0;uwvm_bounded_copy(value->firmware_version.data,p+*offset,value->firmware_version.size);*offset+=value->firmware_version.size;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->mcu_uptime))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->connection_uptime))return 0;
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_Status* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Status next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>676u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 clearpath_platform_msgs/msg/StopStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field stop_power_status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t stop_power_status;
  /** Declared field external_stop_present; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t external_stop_present;
  /** Declared field needs_reset; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t needs_reset;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STOPSTATUS_MAX_WIRE_BYTES 143u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STOPSTATUS_MIN_WIRE_BYTES 15u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STOPSTATUS_CODEC_IDENTITY "UWVMROS1_101f96c8d48e7a945a2f94108e7b48393fc9d4bd75507dd709b95180ff4794d2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->stop_power_status>1)return 0;
  if(value->external_stop_present>1)return 0;
  if(value->needs_reset>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->stop_power_status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->external_stop_present,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->needs_reset,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stop_power_status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->external_stop_present,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->needs_reset,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<15u||length>143u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_StopStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2];} roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_temperatures_sequence;
/** Complete ROS2 clearpath_platform_msgs/msg/Temperature value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field temperatures; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_temperatures_sequence temperatures;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_MAX_WIRE_BYTES 152u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CODEC_IDENTITY "UWVMROS1_b4ae4d6ace9045ce96b0931f08d3b2c7ec358ab9c6da7022afd9ea489f9b0466"
/** Original scalar constant J100_MCU=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_J100_MCU UINT32_C(0)
/** Original scalar constant J100_PCB=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_J100_PCB UINT32_C(1)
/** Original scalar constant D100_MCU=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_D100_MCU UINT32_C(0)
/** Original scalar constant D100_PCB=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_D100_PCB UINT32_C(1)
/** Original scalar constant D150_MCU=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_D150_MCU UINT32_C(0)
/** Original scalar constant D150_PCB=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_D150_PCB UINT32_C(1)
/** Original scalar constant W200_MCU=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_W200_MCU UINT32_C(0)
/** Original scalar constant R100_MCU=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_R100_MCU UINT32_C(0)
/** Original scalar constant R100_PCB=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_R100_PCB UINT32_C(1)
/** Original scalar constant CC01_MCU=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_MCU UINT32_C(0)
/** Original scalar constant CC01_FAN1=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN1 UINT32_C(1)
/** Original scalar constant CC01_FAN2=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN2 UINT32_C(2)
/** Original scalar constant CC01_FAN3=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN3 UINT32_C(3)
/** Original scalar constant CC01_FAN4=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN4 UINT32_C(4)
/** Original scalar constant CC01_FAN5=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN5 UINT32_C(5)
/** Original scalar constant CC01_FAN6=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN6 UINT32_C(6)
/** Original scalar constant CC01_FAN7=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN7 UINT32_C(7)
/** Original scalar constant CC01_FAN8=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_FAN8 UINT32_C(8)
/** Original scalar constant CC01_5V_INDUCTOR=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_5V_INDUCTOR UINT32_C(9)
/** Original scalar constant CC01_MAIN_GND_LUG=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_MAIN_GND_LUG UINT32_C(10)
/** Original scalar constant CC01_24V_DCDC=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_24V_DCDC UINT32_C(11)
/** Original scalar constant CC01_12V_DCDC=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_12V_DCDC UINT32_C(12)
/** Original scalar constant CC01_EXT_1=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_EXT_1 UINT32_C(13)
/** Original scalar constant CC01_EXT_2=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_EXT_2 UINT32_C(14)
/** Original scalar constant CC01_EXT_3=15; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_EXT_3 UINT32_C(15)
/** Original scalar constant CC01_EXT_4=16; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_EXT_4 UINT32_C(16)
/** Original scalar constant CC01_BREAKOUT_1=17; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_BREAKOUT_1 UINT32_C(17)
/** Original scalar constant CC01_BREAKOUT_2=18; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_TEMPERATURE_CC01_BREAKOUT_2 UINT32_C(18)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->temperatures.size>2u)return 0;
  {size_t i;for(i=0;i<value->temperatures.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperatures.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->temperatures.size>2u)return 0;
  {size_t i;for(i=0;i<value->temperatures.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->temperatures.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->temperatures.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperatures.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->temperatures.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->temperatures.size>2u)return 0;
  {size_t i;for(i=0;i<value->temperatures.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->temperatures.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_encode(roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>152u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_msg_Temperature_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 clearpath_control_msgs/srv/PowerControl_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request {
  /** Declared field power_on; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t power_on;
} roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request;
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_SRV_POWERCONTROL_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_SRV_POWERCONTROL_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_SRV_POWERCONTROL_REQUEST_CODEC_IDENTITY "UWVMROS1_1ad49d42d8895b3bd219799ab9cc080cf09d23aa4bca4c1172d19bc18fa416c4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_valid(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request const* value) {
  if(!value)return 0;
  if(value->power_on>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_wire_size(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_write_fields(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->power_on,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->power_on,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_encode(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request* value) {
  roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_control_msgs/srv/PowerControl_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_message_text;
/** Complete ROS2 clearpath_control_msgs/srv/PowerControl_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_message_text message;
} roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response;
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_SRV_POWERCONTROL_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_SRV_POWERCONTROL_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_CLEARPATH_CONTROL_MSGS_SRV_POWERCONTROL_RESPONSE_CODEC_IDENTITY "UWVMROS1_6d74d07d6a8ff1c41ed4ca13939d54e9e642f423dc2ff6598665238ebbf10a8b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_valid(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_wire_size(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_write_fields(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_encode(roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response* value) {
  roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_control_msgs_srv_PowerControl_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_platform_msgs/srv/ConfigureMcu_Request.robot_namespace; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_robot_namespace_text;
/** Complete ROS2 clearpath_platform_msgs/srv/ConfigureMcu_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request {
  /** Declared field domain_id; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t domain_id;
  /** Declared field robot_namespace; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_robot_namespace_text robot_namespace;
} roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_CONFIGUREMCU_REQUEST_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_CONFIGUREMCU_REQUEST_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_CONFIGUREMCU_REQUEST_CODEC_IDENTITY "UWVMROS1_9c48451b208c4de7a51c5075587dfbe710e7ec6b249c7e1eef519022154023fa"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_valid(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request const* value) {
  if(!value)return 0;
  if(value->robot_namespace.size>256u||!uwvm_bounded_utf8(value->robot_namespace.data,value->robot_namespace.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request const* value) {size_t n=0;
  n+=1;
  n+=4+value->robot_namespace.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->domain_id,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->robot_namespace.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->robot_namespace.data,value->robot_namespace.size);*offset+=value->robot_namespace.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->domain_id,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->robot_namespace.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->robot_namespace.size>256u||value->robot_namespace.size>n-*offset)return 0;uwvm_bounded_copy(value->robot_namespace.data,p+*offset,value->robot_namespace.size);*offset+=value->robot_namespace.size;
  return roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_encode(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_platform_msgs/srv/ConfigureMcu_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_message_text;
/** Complete ROS2 clearpath_platform_msgs/srv/ConfigureMcu_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_message_text message;
} roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_CONFIGUREMCU_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_CONFIGUREMCU_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_CONFIGUREMCU_RESPONSE_CODEC_IDENTITY "UWVMROS1_552e715f290b5b9aa5dc926519cf2ab5120d6b2ae5eca9902efcc3f8c9e459ad"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_valid(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_encode(roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_ConfigureMcu_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 clearpath_platform_msgs/srv/SetPinout_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t state;
  /** Declared field toggle_period; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t toggle_period;
} roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_SETPINOUT_REQUEST_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_SETPINOUT_REQUEST_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_SETPINOUT_REQUEST_CODEC_IDENTITY "UWVMROS1_3a3a2f8f16279c2e7a057353cd1f7f11e1a65e887dd3ce0a4b7f584708afb238"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_valid(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request const* value) {
  if(!value)return 0;
  if(value->state>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request const* value) {size_t n=0;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->toggle_period,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->toggle_period,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_encode(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for clearpath_platform_msgs/srv/SetPinout_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_message_text;
/** Complete ROS2 clearpath_platform_msgs/srv/SetPinout_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_message_text message;
} roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_SETPINOUT_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_SETPINOUT_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_SRV_SETPINOUT_RESPONSE_CODEC_IDENTITY "UWVMROS1_b643ef5de2dfb725906ef8f11d79601400953211b47220be7a6fcc05d1be8ee3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_valid(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_encode(roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_decode(void const* input,size_t length,roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response* value) {
  roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_clearpath_platform_msgs_srv_SetPinout_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
