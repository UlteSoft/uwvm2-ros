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
/** Complete ROS2 franka_msgs/action/ErrorRecovery_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_GOAL_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_GOAL_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_GOAL_CODEC_IDENTITY "UWVMROS1_2caee3e8d5c04f12239a71f6e9a9259b818b9f1ad75103442bfc9d2343a3a070"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_valid(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_wire_size(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_write_fields(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal* value) {
  return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_encode(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal* value) {
  roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/ErrorRecovery_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_RESULT_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_RESULT_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_RESULT_CODEC_IDENTITY "UWVMROS1_9112b16c42034737b96214f23249d499ab6a9f0aee529cfec34c67cdc76809e9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_valid(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_wire_size(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_write_fields(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result* value) {
  return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_encode(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result* value) {
  roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/ErrorRecovery_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_FEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_FEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_ERRORRECOVERY_FEEDBACK_CODEC_IDENTITY "UWVMROS1_cc817bcbe5790860e83c71e50a8c3610cf3965e34036b3099712cdcdb426127f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_valid(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_wire_size(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_write_fields(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback* value) {
  return roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_encode(roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback* value) {
  roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_ErrorRecovery_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/msg/GraspEpsilon value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon {
  /** Declared field inner; canonical capacity is part of CODEC_IDENTITY. */
  double inner;
  /** Declared field outer; canonical capacity is part of CODEC_IDENTITY. */
  double outer;
} roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_GRASPEPSILON_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_GRASPEPSILON_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_GRASPEPSILON_CODEC_IDENTITY "UWVMROS1_21787ae5929dc83b9f644f4914e22da9ddd3e4f7ad3d8c6cda6bbcdfeba20c38"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_default(roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon* value) {
  roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.inner=0x1.47ae147ae147bp-8;
  next.outer=0x1.47ae147ae147bp-8;
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_valid(roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->inner,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->outer,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_wire_size(roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon const* value) {size_t n=0;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_write_fields(roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->inner,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->outer,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->inner,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->outer,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_encode(roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon* value) {
  roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/Grasp_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Grasp_Goal {
  /** Declared field width; canonical capacity is part of CODEC_IDENTITY. */
  double width;
  /** Declared field epsilon; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon epsilon;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  double speed;
  /** Declared field force; canonical capacity is part of CODEC_IDENTITY. */
  double force;
} roswasmc_bounded_2_franka_msgs_action_Grasp_Goal;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_GOAL_MAX_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_GOAL_MIN_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_GOAL_CODEC_IDENTITY "UWVMROS1_b7ef701e9fd93b5da9894499234d3647ecc4cea1936f637105c172f4579b6e1f"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_default(roswasmc_bounded_2_franka_msgs_action_Grasp_Goal* value) {
  roswasmc_bounded_2_franka_msgs_action_Grasp_Goal next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_default(&next.epsilon);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_valid(roswasmc_bounded_2_franka_msgs_action_Grasp_Goal const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(!roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_valid(&value->epsilon))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->force,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_wire_size(roswasmc_bounded_2_franka_msgs_action_Grasp_Goal const* value) {size_t n=0;
  n+=8;
  n+=roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_wire_size(&value->epsilon);
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_write_fields(roswasmc_bounded_2_franka_msgs_action_Grasp_Goal const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_write_fields(&value->epsilon,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->force,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Grasp_Goal* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->width,&bits,8);}*offset+=8;
  if(!roswasmc_bounded_2_franka_msgs_msg_GraspEpsilon_read_fields(p,n,offset,&value->epsilon))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->speed,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->force,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_encode(roswasmc_bounded_2_franka_msgs_action_Grasp_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Grasp_Goal* value) {
  roswasmc_bounded_2_franka_msgs_action_Grasp_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<40u||length>40u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Grasp_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/action/Grasp_Result.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_action_Grasp_Result_error_text;
/** Complete ROS2 franka_msgs/action/Grasp_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Grasp_Result {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_action_Grasp_Result_error_text error;
} roswasmc_bounded_2_franka_msgs_action_Grasp_Result;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_RESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_RESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_RESULT_CODEC_IDENTITY "UWVMROS1_9a4c3150c0c2d5eb8ff3dad403473a9546ee37cc3086a55a628a1a85b905b733"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Grasp_Result_valid(roswasmc_bounded_2_franka_msgs_action_Grasp_Result const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Grasp_Result_wire_size(roswasmc_bounded_2_franka_msgs_action_Grasp_Result const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Grasp_Result_write_fields(roswasmc_bounded_2_franka_msgs_action_Grasp_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Grasp_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Grasp_Result* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_action_Grasp_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Grasp_Result_encode(roswasmc_bounded_2_franka_msgs_action_Grasp_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Grasp_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Grasp_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Grasp_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Grasp_Result_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Grasp_Result* value) {
  roswasmc_bounded_2_franka_msgs_action_Grasp_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Grasp_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/Grasp_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback {
  /** Declared field current_width; canonical capacity is part of CODEC_IDENTITY. */
  double current_width;
} roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_FEEDBACK_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_FEEDBACK_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_GRASP_FEEDBACK_CODEC_IDENTITY "UWVMROS1_2e7697286c002e2d891e9c3167aa0dd1664acfc66c649fff6e8f9101c42703fe"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_valid(roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_width,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_wire_size(roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_write_fields(roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_width,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->current_width,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_encode(roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback* value) {
  roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Grasp_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/Homing_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Homing_Goal {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_msgs_action_Homing_Goal;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_GOAL_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_GOAL_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_GOAL_CODEC_IDENTITY "UWVMROS1_8008426ce3fa8c8d62133704f8738201b8154b324ba07e7b6f605cd3132a885d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Homing_Goal_valid(roswasmc_bounded_2_franka_msgs_action_Homing_Goal const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Homing_Goal_wire_size(roswasmc_bounded_2_franka_msgs_action_Homing_Goal const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Homing_Goal_write_fields(roswasmc_bounded_2_franka_msgs_action_Homing_Goal const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Homing_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Homing_Goal* value) {
  return roswasmc_bounded_2_franka_msgs_action_Homing_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Homing_Goal_encode(roswasmc_bounded_2_franka_msgs_action_Homing_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Homing_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Homing_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Homing_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Homing_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Homing_Goal* value) {
  roswasmc_bounded_2_franka_msgs_action_Homing_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Homing_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/action/Homing_Result.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_action_Homing_Result_error_text;
/** Complete ROS2 franka_msgs/action/Homing_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Homing_Result {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_action_Homing_Result_error_text error;
} roswasmc_bounded_2_franka_msgs_action_Homing_Result;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_RESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_RESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_RESULT_CODEC_IDENTITY "UWVMROS1_74a9677213ffa9ba3a26b3053956350b8f75d5d1769152e5e3d4d26370bcd77c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Homing_Result_valid(roswasmc_bounded_2_franka_msgs_action_Homing_Result const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Homing_Result_wire_size(roswasmc_bounded_2_franka_msgs_action_Homing_Result const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Homing_Result_write_fields(roswasmc_bounded_2_franka_msgs_action_Homing_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Homing_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Homing_Result* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_action_Homing_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Homing_Result_encode(roswasmc_bounded_2_franka_msgs_action_Homing_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Homing_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Homing_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Homing_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Homing_Result_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Homing_Result* value) {
  roswasmc_bounded_2_franka_msgs_action_Homing_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Homing_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/Homing_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Homing_Feedback {
  /** Declared field current_width; canonical capacity is part of CODEC_IDENTITY. */
  double current_width;
} roswasmc_bounded_2_franka_msgs_action_Homing_Feedback;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_FEEDBACK_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_FEEDBACK_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_HOMING_FEEDBACK_CODEC_IDENTITY "UWVMROS1_2260e092d369a0d2e4bf97ca2dfa78c02b6836b8167e71d63e76b5c0c40a6b46"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_valid(roswasmc_bounded_2_franka_msgs_action_Homing_Feedback const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_width,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_wire_size(roswasmc_bounded_2_franka_msgs_action_Homing_Feedback const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_write_fields(roswasmc_bounded_2_franka_msgs_action_Homing_Feedback const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_width,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Homing_Feedback* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->current_width,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_encode(roswasmc_bounded_2_franka_msgs_action_Homing_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Homing_Feedback* value) {
  roswasmc_bounded_2_franka_msgs_action_Homing_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Homing_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/Move_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Move_Goal {
  /** Declared field width; canonical capacity is part of CODEC_IDENTITY. */
  double width;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  double speed;
} roswasmc_bounded_2_franka_msgs_action_Move_Goal;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_GOAL_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_GOAL_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_GOAL_CODEC_IDENTITY "UWVMROS1_9f9e308ebd034344ee73b22a07d005944975c037943279504b2414bdd9ad206f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Move_Goal_valid(roswasmc_bounded_2_franka_msgs_action_Move_Goal const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Move_Goal_wire_size(roswasmc_bounded_2_franka_msgs_action_Move_Goal const* value) {size_t n=0;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Move_Goal_write_fields(roswasmc_bounded_2_franka_msgs_action_Move_Goal const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->width,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Move_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Move_Goal* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->width,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->speed,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_msgs_action_Move_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Move_Goal_encode(roswasmc_bounded_2_franka_msgs_action_Move_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Move_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Move_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Move_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Move_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Move_Goal* value) {
  roswasmc_bounded_2_franka_msgs_action_Move_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Move_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/action/Move_Result.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_action_Move_Result_error_text;
/** Complete ROS2 franka_msgs/action/Move_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Move_Result {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_action_Move_Result_error_text error;
} roswasmc_bounded_2_franka_msgs_action_Move_Result;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_RESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_RESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_RESULT_CODEC_IDENTITY "UWVMROS1_66aa505c035357274c68be773e1b502c7d77f195f3dca848e818811ce95dd266"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Move_Result_valid(roswasmc_bounded_2_franka_msgs_action_Move_Result const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Move_Result_wire_size(roswasmc_bounded_2_franka_msgs_action_Move_Result const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Move_Result_write_fields(roswasmc_bounded_2_franka_msgs_action_Move_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Move_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Move_Result* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_action_Move_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Move_Result_encode(roswasmc_bounded_2_franka_msgs_action_Move_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Move_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Move_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Move_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Move_Result_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Move_Result* value) {
  roswasmc_bounded_2_franka_msgs_action_Move_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Move_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/Move_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_Move_Feedback {
  /** Declared field current_width; canonical capacity is part of CODEC_IDENTITY. */
  double current_width;
} roswasmc_bounded_2_franka_msgs_action_Move_Feedback;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_FEEDBACK_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_FEEDBACK_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_MOVE_FEEDBACK_CODEC_IDENTITY "UWVMROS1_18d0e658f109492380c135e787adbdcc026b051e9a547fcea70453bbd9750596"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_Move_Feedback_valid(roswasmc_bounded_2_franka_msgs_action_Move_Feedback const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_width,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_Move_Feedback_wire_size(roswasmc_bounded_2_franka_msgs_action_Move_Feedback const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_Move_Feedback_write_fields(roswasmc_bounded_2_franka_msgs_action_Move_Feedback const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_width,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_Move_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_Move_Feedback* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->current_width,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_msgs_action_Move_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Move_Feedback_encode(roswasmc_bounded_2_franka_msgs_action_Move_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_Move_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_Move_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_Move_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_Move_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_Move_Feedback* value) {
  roswasmc_bounded_2_franka_msgs_action_Move_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_Move_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most7 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[7];} roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_goal_joint_configuration_sequence;
/** At most7 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[7];} roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_maximum_joint_velocities_sequence;
/** Complete ROS2 franka_msgs/action/PTPMotion_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal {
  /** Declared field goal_joint_configuration; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_goal_joint_configuration_sequence goal_joint_configuration;
  /** Declared field maximum_joint_velocities; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_maximum_joint_velocities_sequence maximum_joint_velocities;
  /** Declared field goal_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  double goal_tolerance;
} roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_GOAL_MAX_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_GOAL_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_GOAL_CODEC_IDENTITY "UWVMROS1_7e1b02257a7be37e7bccd0870ebced1e1560e7250eaeaa4547836e721f2ea2cc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_valid(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal const* value) {
  if(!value)return 0;
  if(value->goal_joint_configuration.size>7u)return 0;
  {size_t i;for(i=0;i<value->goal_joint_configuration.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->goal_joint_configuration.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->maximum_joint_velocities.size>7u)return 0;
  {size_t i;for(i=0;i<value->maximum_joint_velocities.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->maximum_joint_velocities.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->goal_tolerance,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_wire_size(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal const* value) {size_t n=0;
  n+=4;
  if(value->goal_joint_configuration.size>7u)return 0;
  {size_t i;for(i=0;i<value->goal_joint_configuration.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->maximum_joint_velocities.size>7u)return 0;
  {size_t i;for(i=0;i<value->maximum_joint_velocities.size;++i){
  n+=8;
  }}
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_write_fields(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->goal_joint_configuration.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->goal_joint_configuration.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->goal_joint_configuration.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->maximum_joint_velocities.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->maximum_joint_velocities.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->maximum_joint_velocities.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->goal_tolerance,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal* value) {
  if(*offset>n||n-*offset<4)return 0;value->goal_joint_configuration.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->goal_joint_configuration.size>7u)return 0;
  {size_t i;for(i=0;i<value->goal_joint_configuration.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->goal_joint_configuration.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->maximum_joint_velocities.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->maximum_joint_velocities.size>7u)return 0;
  {size_t i;for(i=0;i<value->maximum_joint_velocities.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->maximum_joint_velocities.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->goal_tolerance,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_encode(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal* value) {
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>128u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/msg/TargetStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_msg_TargetStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
} roswasmc_bounded_2_franka_msgs_msg_TargetStatus;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_TARGETSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_TARGETSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_TARGETSTATUS_CODEC_IDENTITY "UWVMROS1_df49eedc13cd8be2055361d8640c519bfa018ed2a421aa2b59b35c8828eac585"
/** Original scalar constant IDLE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_TARGETSTATUS_IDLE UINT32_C(0)
/** Original scalar constant EXECUTING=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_TARGETSTATUS_EXECUTING UINT32_C(1)
/** Original scalar constant TARGET_REACHED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_TARGETSTATUS_TARGET_REACHED UINT32_C(2)
/** Original scalar constant ABORTED=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_TARGETSTATUS_ABORTED UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_msg_TargetStatus_valid(roswasmc_bounded_2_franka_msgs_msg_TargetStatus const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_msg_TargetStatus_wire_size(roswasmc_bounded_2_franka_msgs_msg_TargetStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_msg_TargetStatus_write_fields(roswasmc_bounded_2_franka_msgs_msg_TargetStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_msg_TargetStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_msg_TargetStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_franka_msgs_msg_TargetStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_TargetStatus_encode(roswasmc_bounded_2_franka_msgs_msg_TargetStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_msg_TargetStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_msg_TargetStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_msg_TargetStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_TargetStatus_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_msg_TargetStatus* value) {
  roswasmc_bounded_2_franka_msgs_msg_TargetStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_msg_TargetStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/action/PTPMotion_Result.error_message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_error_message_text;
/** Complete ROS2 franka_msgs/action/PTPMotion_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result {
  /** Declared field target_status; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_msg_TargetStatus target_status;
  /** Declared field error_message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_error_message_text error_message;
} roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_RESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_RESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_RESULT_CODEC_IDENTITY "UWVMROS1_b61fabf13a3d8d910a35a5145e87b4dd80180b0678ef5580784f461e850fa4c1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_valid(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_TargetStatus_valid(&value->target_status))return 0;
  if(value->error_message.size>256u||!uwvm_bounded_utf8(value->error_message.data,value->error_message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_wire_size(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result const* value) {size_t n=0;
  n+=roswasmc_bounded_2_franka_msgs_msg_TargetStatus_wire_size(&value->target_status);
  n+=4+value->error_message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_write_fields(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_franka_msgs_msg_TargetStatus_write_fields(&value->target_status,p,offset);
  uwvm_bounded_put(p+*offset,value->error_message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_message.data,value->error_message.size);*offset+=value->error_message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result* value) {
  if(!roswasmc_bounded_2_franka_msgs_msg_TargetStatus_read_fields(p,n,offset,&value->target_status))return 0;
  if(*offset>n||n-*offset<4)return 0;value->error_message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_message.size>256u||value->error_message.size>n-*offset)return 0;uwvm_bounded_copy(value->error_message.data,p+*offset,value->error_message.size);*offset+=value->error_message.size;
  return roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_encode(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result* value) {
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/action/PTPMotion_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback {
  /** Declared field target_status; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_msg_TargetStatus target_status;
} roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_FEEDBACK_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_FEEDBACK_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_ACTION_PTPMOTION_FEEDBACK_CODEC_IDENTITY "UWVMROS1_c4dcfeb668595ebc341d76b5cbc606c1ae4c38b28a1b329f8436e61f506a0300"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_valid(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_TargetStatus_valid(&value->target_status))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_wire_size(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback const* value) {size_t n=0;
  n+=roswasmc_bounded_2_franka_msgs_msg_TargetStatus_wire_size(&value->target_status);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_write_fields(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_franka_msgs_msg_TargetStatus_write_fields(&value->target_status,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback* value) {
  if(!roswasmc_bounded_2_franka_msgs_msg_TargetStatus_read_fields(p,n,offset,&value->target_status))return 0;
  return roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_encode(roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback* value) {
  roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_action_PTPMotion_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS2 franka_msgs/msg/CollisionIndicators value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators {
  /** Declared field is_cartesian_linear_collision; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 is_cartesian_linear_collision;
  /** Declared field is_cartesian_angular_collision; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 is_cartesian_angular_collision;
  /** Declared field is_cartesian_linear_contact; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 is_cartesian_linear_contact;
  /** Declared field is_cartesian_angular_contact; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 is_cartesian_angular_contact;
  /** Declared field is_joint_collision; canonical capacity is part of CODEC_IDENTITY. */
  double is_joint_collision[7];
  /** Declared field is_joint_contact; canonical capacity is part of CODEC_IDENTITY. */
  double is_joint_contact[7];
} roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_COLLISIONINDICATORS_MAX_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_COLLISIONINDICATORS_MIN_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_COLLISIONINDICATORS_CODEC_IDENTITY "UWVMROS1_2bdfb3ece01b47b07fae84a998b23c56fc9482a0518969e43304b318d4ee0ce1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_valid(roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->is_cartesian_linear_collision))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->is_cartesian_angular_collision))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->is_cartesian_linear_contact))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->is_cartesian_angular_contact))return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->is_joint_collision[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->is_joint_contact[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_wire_size(roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->is_cartesian_linear_collision);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->is_cartesian_angular_collision);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->is_cartesian_linear_contact);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->is_cartesian_angular_contact);
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_write_fields(roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->is_cartesian_linear_collision,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->is_cartesian_angular_collision,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->is_cartesian_linear_contact,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->is_cartesian_angular_contact,p,offset);
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->is_joint_collision[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->is_joint_contact[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->is_cartesian_linear_collision))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->is_cartesian_angular_collision))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->is_cartesian_linear_contact))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->is_cartesian_angular_contact))return 0;
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->is_joint_collision[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->is_joint_contact[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_encode(roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators* value) {
  roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators next={0};size_t offset=0;if(!value||(length&&!input)||length<208u||length>208u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/msg/Elbow value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_msg_Elbow {
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  double position[2];
  /** Declared field desired_position; canonical capacity is part of CODEC_IDENTITY. */
  double desired_position[2];
  /** Declared field commanded_position; canonical capacity is part of CODEC_IDENTITY. */
  double commanded_position[2];
  /** Declared field commanded_velocity; canonical capacity is part of CODEC_IDENTITY. */
  double commanded_velocity[2];
  /** Declared field commanded_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double commanded_acceleration[2];
} roswasmc_bounded_2_franka_msgs_msg_Elbow;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_ELBOW_MAX_WIRE_BYTES 80u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_ELBOW_MIN_WIRE_BYTES 80u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_ELBOW_CODEC_IDENTITY "UWVMROS1_686cf2da29007046f9855ffeecac304c159245ec3da153990ab83be4ffce53b6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_msg_Elbow_valid(roswasmc_bounded_2_franka_msgs_msg_Elbow const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->desired_position[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->commanded_position[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->commanded_velocity[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->commanded_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_msg_Elbow_wire_size(roswasmc_bounded_2_franka_msgs_msg_Elbow const* value) {size_t n=0;
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_msg_Elbow_write_fields(roswasmc_bounded_2_franka_msgs_msg_Elbow const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->desired_position[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->commanded_position[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->commanded_velocity[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->commanded_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_msg_Elbow_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_msg_Elbow* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->desired_position[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->commanded_position[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->commanded_velocity[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->commanded_acceleration[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_msg_Elbow_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_Elbow_encode(roswasmc_bounded_2_franka_msgs_msg_Elbow const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_msg_Elbow_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_msg_Elbow_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_msg_Elbow_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_Elbow_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_msg_Elbow* value) {
  roswasmc_bounded_2_franka_msgs_msg_Elbow next={0};size_t offset=0;if(!value||(length&&!input)||length<80u||length>80u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_msg_Elbow_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/msg/Errors value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_msg_Errors {
  /** Declared field joint_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_position_limits_violation;
  /** Declared field cartesian_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_position_limits_violation;
  /** Declared field self_collision_avoidance_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t self_collision_avoidance_violation;
  /** Declared field joint_velocity_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_velocity_violation;
  /** Declared field cartesian_velocity_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_velocity_violation;
  /** Declared field force_control_safety_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t force_control_safety_violation;
  /** Declared field joint_reflex; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_reflex;
  /** Declared field cartesian_reflex; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_reflex;
  /** Declared field max_goal_pose_deviation_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t max_goal_pose_deviation_violation;
  /** Declared field max_path_pose_deviation_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t max_path_pose_deviation_violation;
  /** Declared field cartesian_velocity_profile_safety_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_velocity_profile_safety_violation;
  /** Declared field joint_position_motion_generator_start_pose_invalid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_position_motion_generator_start_pose_invalid;
  /** Declared field joint_motion_generator_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_position_limits_violation;
  /** Declared field joint_motion_generator_velocity_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_velocity_limits_violation;
  /** Declared field joint_motion_generator_velocity_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_velocity_discontinuity;
  /** Declared field joint_motion_generator_acceleration_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_motion_generator_acceleration_discontinuity;
  /** Declared field cartesian_position_motion_generator_start_pose_invalid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_position_motion_generator_start_pose_invalid;
  /** Declared field cartesian_motion_generator_elbow_limit_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_elbow_limit_violation;
  /** Declared field cartesian_motion_generator_velocity_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_velocity_limits_violation;
  /** Declared field cartesian_motion_generator_velocity_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_velocity_discontinuity;
  /** Declared field cartesian_motion_generator_acceleration_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_acceleration_discontinuity;
  /** Declared field cartesian_motion_generator_elbow_sign_inconsistent; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_elbow_sign_inconsistent;
  /** Declared field cartesian_motion_generator_start_elbow_invalid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_start_elbow_invalid;
  /** Declared field cartesian_motion_generator_joint_position_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_position_limits_violation;
  /** Declared field cartesian_motion_generator_joint_velocity_limits_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_velocity_limits_violation;
  /** Declared field cartesian_motion_generator_joint_velocity_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_velocity_discontinuity;
  /** Declared field cartesian_motion_generator_joint_acceleration_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_motion_generator_joint_acceleration_discontinuity;
  /** Declared field cartesian_position_motion_generator_invalid_frame; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t cartesian_position_motion_generator_invalid_frame;
  /** Declared field force_controller_desired_force_tolerance_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t force_controller_desired_force_tolerance_violation;
  /** Declared field controller_torque_discontinuity; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t controller_torque_discontinuity;
  /** Declared field start_elbow_sign_inconsistent; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t start_elbow_sign_inconsistent;
  /** Declared field communication_constraints_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t communication_constraints_violation;
  /** Declared field power_limit_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t power_limit_violation;
  /** Declared field joint_p2p_insufficient_torque_for_planning; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joint_p2p_insufficient_torque_for_planning;
  /** Declared field tau_j_range_violation; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t tau_j_range_violation;
  /** Declared field instability_detected; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t instability_detected;
} roswasmc_bounded_2_franka_msgs_msg_Errors;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_ERRORS_MAX_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_ERRORS_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_ERRORS_CODEC_IDENTITY "UWVMROS1_c7927af1abe7710bb686574ad6f2459f8425af1d6cdb3189160bb732b5a6bc8c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_msg_Errors_valid(roswasmc_bounded_2_franka_msgs_msg_Errors const* value) {
  if(!value)return 0;
  if(value->joint_position_limits_violation>1)return 0;
  if(value->cartesian_position_limits_violation>1)return 0;
  if(value->self_collision_avoidance_violation>1)return 0;
  if(value->joint_velocity_violation>1)return 0;
  if(value->cartesian_velocity_violation>1)return 0;
  if(value->force_control_safety_violation>1)return 0;
  if(value->joint_reflex>1)return 0;
  if(value->cartesian_reflex>1)return 0;
  if(value->max_goal_pose_deviation_violation>1)return 0;
  if(value->max_path_pose_deviation_violation>1)return 0;
  if(value->cartesian_velocity_profile_safety_violation>1)return 0;
  if(value->joint_position_motion_generator_start_pose_invalid>1)return 0;
  if(value->joint_motion_generator_position_limits_violation>1)return 0;
  if(value->joint_motion_generator_velocity_limits_violation>1)return 0;
  if(value->joint_motion_generator_velocity_discontinuity>1)return 0;
  if(value->joint_motion_generator_acceleration_discontinuity>1)return 0;
  if(value->cartesian_position_motion_generator_start_pose_invalid>1)return 0;
  if(value->cartesian_motion_generator_elbow_limit_violation>1)return 0;
  if(value->cartesian_motion_generator_velocity_limits_violation>1)return 0;
  if(value->cartesian_motion_generator_velocity_discontinuity>1)return 0;
  if(value->cartesian_motion_generator_acceleration_discontinuity>1)return 0;
  if(value->cartesian_motion_generator_elbow_sign_inconsistent>1)return 0;
  if(value->cartesian_motion_generator_start_elbow_invalid>1)return 0;
  if(value->cartesian_motion_generator_joint_position_limits_violation>1)return 0;
  if(value->cartesian_motion_generator_joint_velocity_limits_violation>1)return 0;
  if(value->cartesian_motion_generator_joint_velocity_discontinuity>1)return 0;
  if(value->cartesian_motion_generator_joint_acceleration_discontinuity>1)return 0;
  if(value->cartesian_position_motion_generator_invalid_frame>1)return 0;
  if(value->force_controller_desired_force_tolerance_violation>1)return 0;
  if(value->controller_torque_discontinuity>1)return 0;
  if(value->start_elbow_sign_inconsistent>1)return 0;
  if(value->communication_constraints_violation>1)return 0;
  if(value->power_limit_violation>1)return 0;
  if(value->joint_p2p_insufficient_torque_for_planning>1)return 0;
  if(value->tau_j_range_violation>1)return 0;
  if(value->instability_detected>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_msg_Errors_wire_size(roswasmc_bounded_2_franka_msgs_msg_Errors const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_franka_msgs_msg_Errors_write_fields(roswasmc_bounded_2_franka_msgs_msg_Errors const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->self_collision_avoidance_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_velocity_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_velocity_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->force_control_safety_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_reflex,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_reflex,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->max_goal_pose_deviation_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->max_path_pose_deviation_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_velocity_profile_safety_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_position_motion_generator_start_pose_invalid,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_velocity_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_velocity_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_motion_generator_acceleration_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_position_motion_generator_start_pose_invalid,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_elbow_limit_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_velocity_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_velocity_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_acceleration_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_elbow_sign_inconsistent,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_start_elbow_invalid,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_position_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_velocity_limits_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_velocity_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_motion_generator_joint_acceleration_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cartesian_position_motion_generator_invalid_frame,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->force_controller_desired_force_tolerance_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->controller_torque_discontinuity,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->start_elbow_sign_inconsistent,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->communication_constraints_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->power_limit_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joint_p2p_insufficient_torque_for_planning,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->tau_j_range_violation,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->instability_detected,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_msg_Errors_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_msg_Errors* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->self_collision_avoidance_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_velocity_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_velocity_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->force_control_safety_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_reflex,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_reflex,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->max_goal_pose_deviation_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->max_path_pose_deviation_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_velocity_profile_safety_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_position_motion_generator_start_pose_invalid,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_velocity_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_velocity_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_motion_generator_acceleration_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_position_motion_generator_start_pose_invalid,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_elbow_limit_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_velocity_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_velocity_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_acceleration_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_elbow_sign_inconsistent,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_start_elbow_invalid,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_position_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_velocity_limits_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_velocity_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_motion_generator_joint_acceleration_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->cartesian_position_motion_generator_invalid_frame,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->force_controller_desired_force_tolerance_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->controller_torque_discontinuity,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->start_elbow_sign_inconsistent,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->communication_constraints_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->power_limit_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joint_p2p_insufficient_torque_for_planning,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tau_j_range_violation,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->instability_detected,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_franka_msgs_msg_Errors_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_Errors_encode(roswasmc_bounded_2_franka_msgs_msg_Errors const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_msg_Errors_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_msg_Errors_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_msg_Errors_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_Errors_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_msg_Errors* value) {
  roswasmc_bounded_2_franka_msgs_msg_Errors next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>36u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_msg_Errors_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** UTF-8 byte storage for sensor_msgs/msg/JointState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text;
/** At most7 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text data[7];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_sequence;
/** At most7 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[7];} roswasmc_bounded_2_sensor_msgs_msg_JointState_position_sequence;
/** At most7 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[7];} roswasmc_bounded_2_sensor_msgs_msg_JointState_velocity_sequence;
/** At most7 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[7];} roswasmc_bounded_2_sensor_msgs_msg_JointState_effort_sequence;
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
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MAX_WIRE_BYTES 1248u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_CODEC_IDENTITY "UWVMROS1_912ba4261e2dfbec0e4a543ea8e5d308ef1d26877b4a7d71162a981ac64c40a3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(roswasmc_bounded_2_sensor_msgs_msg_JointState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->name.size>7u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(value->name.data[i].size>128u||!uwvm_bounded_utf8(value->name.data[i].data,value->name.data[i].size))return 0;
  }}
  if(value->position.size>7u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->velocity.size>7u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->effort.size>7u)return 0;
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
  if(value->name.size>7u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  n+=4+value->name.data[i].size;
  }}
  n+=4;
  if(value->position.size>7u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->velocity.size>7u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->effort.size>7u)return 0;
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
  if(value->name.size>7u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->name.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.data[i].size>128u||value->name.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->name.data[i].data,p+*offset,value->name.data[i].size);*offset+=value->name.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->position.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->position.size>7u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocity.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocity.size>7u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->effort.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->effort.size>7u)return 0;
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
  roswasmc_bounded_2_sensor_msgs_msg_JointState next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>1248u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Wrench value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Wrench {
  /** Declared field force; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 force;
  /** Declared field torque; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 torque;
} roswasmc_bounded_2_geometry_msgs_msg_Wrench;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_WRENCH_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_WRENCH_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_WRENCH_CODEC_IDENTITY "UWVMROS1_fdfdf2474c0129b0ce1f85972cc87fd80478ac7d2429d9de1ec58951a82a11b6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Wrench_valid(roswasmc_bounded_2_geometry_msgs_msg_Wrench const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->force))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->torque))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Wrench_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Wrench const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->force);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->torque);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Wrench_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Wrench const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->force,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->torque,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Wrench_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Wrench* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->force))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->torque))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_Wrench_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Wrench_encode(roswasmc_bounded_2_geometry_msgs_msg_Wrench const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Wrench_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Wrench_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Wrench_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Wrench_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Wrench* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Wrench next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Wrench_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/WrenchStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field wrench; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Wrench wrench;
} roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_WRENCHSTAMPED_MAX_WIRE_BYTES 188u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_WRENCHSTAMPED_MIN_WIRE_BYTES 60u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_WRENCHSTAMPED_CODEC_IDENTITY "UWVMROS1_e23600874a626ba9d52f043e14ed04beb3e316d47c3bca8bd87cfeed3f5d9d66"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_valid(roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Wrench_valid(&value->wrench))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_wire_size(roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Wrench_wire_size(&value->wrench);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_write_fields(roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Wrench_write_fields(&value->wrench,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Wrench_read_fields(p,n,offset,&value->wrench))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_encode(roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<60u||length>188u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Inertia value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Inertia {
  /** Declared field m; canonical capacity is part of CODEC_IDENTITY. */
  double m;
  /** Declared field com; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 com;
  /** Declared field ixx; canonical capacity is part of CODEC_IDENTITY. */
  double ixx;
  /** Declared field ixy; canonical capacity is part of CODEC_IDENTITY. */
  double ixy;
  /** Declared field ixz; canonical capacity is part of CODEC_IDENTITY. */
  double ixz;
  /** Declared field iyy; canonical capacity is part of CODEC_IDENTITY. */
  double iyy;
  /** Declared field iyz; canonical capacity is part of CODEC_IDENTITY. */
  double iyz;
  /** Declared field izz; canonical capacity is part of CODEC_IDENTITY. */
  double izz;
} roswasmc_bounded_2_geometry_msgs_msg_Inertia;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_INERTIA_MAX_WIRE_BYTES 80u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_INERTIA_MIN_WIRE_BYTES 80u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_INERTIA_CODEC_IDENTITY "UWVMROS1_4a1b7a6a2516724a9a12e5eb166a85cd73d33fc6075c8d517ff2a279fd29e12a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Inertia_valid(roswasmc_bounded_2_geometry_msgs_msg_Inertia const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->com))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixx,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixy,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixz,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyy,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyz,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->izz,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Inertia_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Inertia const* value) {size_t n=0;
  n+=8;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->com);
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Inertia_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Inertia const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->com,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixx,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixy,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixz,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyy,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyz,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->izz,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Inertia_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Inertia* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->m,&bits,8);}*offset+=8;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->com))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ixx,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ixy,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ixz,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->iyy,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->iyz,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->izz,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_geometry_msgs_msg_Inertia_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Inertia_encode(roswasmc_bounded_2_geometry_msgs_msg_Inertia const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Inertia_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Inertia_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Inertia_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Inertia_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Inertia* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Inertia next={0};size_t offset=0;if(!value||(length&&!input)||length<80u||length>80u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Inertia_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/InertiaStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field inertia; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Inertia inertia;
} roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_INERTIASTAMPED_MAX_WIRE_BYTES 220u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_INERTIASTAMPED_MIN_WIRE_BYTES 92u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_INERTIASTAMPED_CODEC_IDENTITY "UWVMROS1_19b0b442c5858ff1eee20e4658ecc3a2b76fdd64c4a21a1642fae5ab38198576"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_valid(roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Inertia_valid(&value->inertia))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_wire_size(roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Inertia_wire_size(&value->inertia);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_write_fields(roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Inertia_write_fields(&value->inertia,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Inertia_read_fields(p,n,offset,&value->inertia))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_encode(roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<92u||length>220u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Point value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Point {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  double x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  double y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  double z;
} roswasmc_bounded_2_geometry_msgs_msg_Point;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POINT_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POINT_CODEC_IDENTITY "UWVMROS1_80f43d054abe333673c940c6d0fd88e8e26cea4a1cff53ff789cccaa38240616"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Point_valid(roswasmc_bounded_2_geometry_msgs_msg_Point const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Point_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Point const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Point_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Point const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Point_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Point* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->x,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->y,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->z,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_geometry_msgs_msg_Point_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Point_encode(roswasmc_bounded_2_geometry_msgs_msg_Point const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Point_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Point_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Point_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Point_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Point* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Point next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Point_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS2 geometry_msgs/msg/Pose value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Pose {
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Point position;
  /** Declared field orientation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion orientation;
} roswasmc_bounded_2_geometry_msgs_msg_Pose;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSE_MAX_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSE_MIN_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSE_CODEC_IDENTITY "UWVMROS1_9468a10b8b2728fe71a370b4348ffe5e81b884ff957533f8700aa16e70b12777"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Pose_default(roswasmc_bounded_2_geometry_msgs_msg_Pose* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Pose next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&next.orientation);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Pose_valid(roswasmc_bounded_2_geometry_msgs_msg_Pose const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Point_valid(&value->position))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(&value->orientation))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Pose_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Pose const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Point_wire_size(&value->position);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Quaternion_wire_size(&value->orientation);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Pose_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Pose const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Point_write_fields(&value->position,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion_write_fields(&value->orientation,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Pose_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Pose* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Point_read_fields(p,n,offset,&value->position))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_read_fields(p,n,offset,&value->orientation))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_Pose_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Pose_encode(roswasmc_bounded_2_geometry_msgs_msg_Pose const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Pose_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Pose_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Pose_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Pose_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Pose* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Pose next={0};size_t offset=0;if(!value||(length&&!input)||length<56u||length>56u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Pose_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/PoseStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_PoseStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field pose; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Pose pose;
} roswasmc_bounded_2_geometry_msgs_msg_PoseStamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSESTAMPED_MAX_WIRE_BYTES 196u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSESTAMPED_MIN_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSESTAMPED_CODEC_IDENTITY "UWVMROS1_71abdc844f18f02f45a354967a5f1c644fb20dc14cbc61c64aa41b927e0712a2"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_Pose_default(&next.pose);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Pose_valid(&value->pose))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Pose_wire_size(&value->pose);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Pose_write_fields(&value->pose,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_PoseStamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Pose_read_fields(p,n,offset,&value->pose))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_encode(roswasmc_bounded_2_geometry_msgs_msg_PoseStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_PoseStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<68u||length>196u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS2 geometry_msgs/msg/TwistStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_TwistStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field twist; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Twist twist;
} roswasmc_bounded_2_geometry_msgs_msg_TwistStamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWISTSTAMPED_MAX_WIRE_BYTES 188u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWISTSTAMPED_MIN_WIRE_BYTES 60u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWISTSTAMPED_CODEC_IDENTITY "UWVMROS1_7448b885388ff3454de1a35b8c5c895ee58b791267b435df1e4cf7c1eee0f573"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_valid(roswasmc_bounded_2_geometry_msgs_msg_TwistStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value->twist))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_wire_size(roswasmc_bounded_2_geometry_msgs_msg_TwistStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(&value->twist);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_write_fields(roswasmc_bounded_2_geometry_msgs_msg_TwistStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Twist_write_fields(&value->twist,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_TwistStamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields(p,n,offset,&value->twist))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_encode(roswasmc_bounded_2_geometry_msgs_msg_TwistStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_TwistStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_TwistStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<60u||length>188u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Accel value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Accel {
  /** Declared field linear; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 linear;
  /** Declared field angular; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 angular;
} roswasmc_bounded_2_geometry_msgs_msg_Accel;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_ACCEL_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_ACCEL_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_ACCEL_CODEC_IDENTITY "UWVMROS1_87264cb87970315248345c43cc629bbc0c9f9b2a8c6c4bc089da81c7af552fde"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Accel_valid(roswasmc_bounded_2_geometry_msgs_msg_Accel const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->linear))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->angular))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Accel_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Accel const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->linear);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->angular);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Accel_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Accel const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->linear,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->angular,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Accel_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Accel* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->linear))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->angular))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_Accel_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Accel_encode(roswasmc_bounded_2_geometry_msgs_msg_Accel const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Accel_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Accel_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Accel_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Accel_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Accel* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Accel next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Accel_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/AccelStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_AccelStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field accel; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Accel accel;
} roswasmc_bounded_2_geometry_msgs_msg_AccelStamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_ACCELSTAMPED_MAX_WIRE_BYTES 188u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_ACCELSTAMPED_MIN_WIRE_BYTES 60u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_ACCELSTAMPED_CODEC_IDENTITY "UWVMROS1_c04d3ae310bd82f7c4c9f30c2ff185ab54989aa3adcc759c7d3eb19853819899"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_valid(roswasmc_bounded_2_geometry_msgs_msg_AccelStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Accel_valid(&value->accel))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_wire_size(roswasmc_bounded_2_geometry_msgs_msg_AccelStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Accel_wire_size(&value->accel);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_write_fields(roswasmc_bounded_2_geometry_msgs_msg_AccelStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Accel_write_fields(&value->accel,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_AccelStamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Accel_read_fields(p,n,offset,&value->accel))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_encode(roswasmc_bounded_2_geometry_msgs_msg_AccelStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_AccelStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_AccelStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<60u||length>188u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 geometry_msgs/msg/Vector3Stamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field vector; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 vector;
} roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_VECTOR3STAMPED_MAX_WIRE_BYTES 164u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_VECTOR3STAMPED_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_VECTOR3STAMPED_CODEC_IDENTITY "UWVMROS1_c6b67646d3fc6d29c195637ea353dafa7085b2bdb55a15a8b65009200b5ed31b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_valid(roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->vector))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_wire_size(roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->vector);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_write_fields(roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->vector,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->vector))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_encode(roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>164u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/msg/FrankaRobotState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field collision_indicators; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators collision_indicators;
  /** Declared field measured_joint_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState measured_joint_state;
  /** Declared field desired_joint_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState desired_joint_state;
  /** Declared field measured_joint_motor_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState measured_joint_motor_state;
  /** Declared field ddq_d; canonical capacity is part of CODEC_IDENTITY. */
  double ddq_d[7];
  /** Declared field dtau_j; canonical capacity is part of CODEC_IDENTITY. */
  double dtau_j[7];
  /** Declared field tau_ext_hat_filtered; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_JointState tau_ext_hat_filtered;
  /** Declared field elbow; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_msg_Elbow elbow;
  /** Declared field k_f_ext_hat_k; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped k_f_ext_hat_k;
  /** Declared field o_f_ext_hat_k; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped o_f_ext_hat_k;
  /** Declared field inertia_ee; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped inertia_ee;
  /** Declared field inertia_load; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped inertia_load;
  /** Declared field inertia_total; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped inertia_total;
  /** Declared field o_t_ee; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped o_t_ee;
  /** Declared field o_t_ee_d; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped o_t_ee_d;
  /** Declared field o_t_ee_c; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped o_t_ee_c;
  /** Declared field f_t_ee; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped f_t_ee;
  /** Declared field ee_t_k; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped ee_t_k;
  /** Declared field o_dp_ee_d; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_TwistStamped o_dp_ee_d;
  /** Declared field o_dp_ee_c; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_TwistStamped o_dp_ee_c;
  /** Declared field o_ddp_ee_c; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_AccelStamped o_ddp_ee_c;
  /** Declared field time; canonical capacity is part of CODEC_IDENTITY. */
  double time;
  /** Declared field control_command_success_rate; canonical capacity is part of CODEC_IDENTITY. */
  double control_command_success_rate;
  /** Declared field robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t robot_mode;
  /** Declared field current_errors; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_msg_Errors current_errors;
  /** Declared field last_motion_errors; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_msg_Errors last_motion_errors;
  /** Declared field accelerometer_top; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped accelerometer_top[6];
  /** Declared field accelerometer_bottom; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped accelerometer_bottom[6];
} roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_MAX_WIRE_BYTES 10169u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_MIN_WIRE_BYTES 1961u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_CODEC_IDENTITY "UWVMROS1_0fdc5c99a098f4c891956bb6cd81505995d5dda41a740f88ee456f48f6e07282"
/** Original scalar constant ROBOT_MODE_OTHER=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_ROBOT_MODE_OTHER UINT32_C(0)
/** Original scalar constant ROBOT_MODE_IDLE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_ROBOT_MODE_IDLE UINT32_C(1)
/** Original scalar constant ROBOT_MODE_MOVE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_ROBOT_MODE_MOVE UINT32_C(2)
/** Original scalar constant ROBOT_MODE_GUIDING=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_ROBOT_MODE_GUIDING UINT32_C(3)
/** Original scalar constant ROBOT_MODE_REFLEX=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_ROBOT_MODE_REFLEX UINT32_C(4)
/** Original scalar constant ROBOT_MODE_USER_STOPPED=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_ROBOT_MODE_USER_STOPPED UINT32_C(5)
/** Original scalar constant ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_MSG_FRANKAROBOTSTATE_ROBOT_MODE_AUTOMATIC_ERROR_RECOVERY UINT32_C(6)
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_default(roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState* value) {
  roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(&next.o_t_ee);
  (void)roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(&next.o_t_ee_d);
  (void)roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(&next.o_t_ee_c);
  (void)roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(&next.f_t_ee);
  (void)roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(&next.ee_t_k);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_valid(roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_valid(&value->collision_indicators))return 0;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value->measured_joint_state))return 0;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value->desired_joint_state))return 0;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value->measured_joint_motor_state))return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ddq_d[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dtau_j[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(&value->tau_ext_hat_filtered))return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_Elbow_valid(&value->elbow))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_valid(&value->k_f_ext_hat_k))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_valid(&value->o_f_ext_hat_k))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_valid(&value->inertia_ee))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_valid(&value->inertia_load))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_valid(&value->inertia_total))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(&value->o_t_ee))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(&value->o_t_ee_d))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(&value->o_t_ee_c))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(&value->f_t_ee))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(&value->ee_t_k))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_valid(&value->o_dp_ee_d))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_valid(&value->o_dp_ee_c))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_valid(&value->o_ddp_ee_c))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->time,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->control_command_success_rate,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(!roswasmc_bounded_2_franka_msgs_msg_Errors_valid(&value->current_errors))return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_Errors_valid(&value->last_motion_errors))return 0;
  {size_t i;for(i=0;i<6u;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_valid(&value->accelerometer_top[i]))return 0;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_valid(&value->accelerometer_bottom[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_wire_size(roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_wire_size(&value->collision_indicators);
  n+=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value->measured_joint_state);
  n+=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value->desired_joint_state);
  n+=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value->measured_joint_motor_state);
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  n+=roswasmc_bounded_2_sensor_msgs_msg_JointState_wire_size(&value->tau_ext_hat_filtered);
  n+=roswasmc_bounded_2_franka_msgs_msg_Elbow_wire_size(&value->elbow);
  n+=roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_wire_size(&value->k_f_ext_hat_k);
  n+=roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_wire_size(&value->o_f_ext_hat_k);
  n+=roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_wire_size(&value->inertia_ee);
  n+=roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_wire_size(&value->inertia_load);
  n+=roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_wire_size(&value->inertia_total);
  n+=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(&value->o_t_ee);
  n+=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(&value->o_t_ee_d);
  n+=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(&value->o_t_ee_c);
  n+=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(&value->f_t_ee);
  n+=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(&value->ee_t_k);
  n+=roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_wire_size(&value->o_dp_ee_d);
  n+=roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_wire_size(&value->o_dp_ee_c);
  n+=roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_wire_size(&value->o_ddp_ee_c);
  n+=8;
  n+=8;
  n+=1;
  n+=roswasmc_bounded_2_franka_msgs_msg_Errors_wire_size(&value->current_errors);
  n+=roswasmc_bounded_2_franka_msgs_msg_Errors_wire_size(&value->last_motion_errors);
  {size_t i;for(i=0;i<6u;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_wire_size(&value->accelerometer_top[i]);
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_wire_size(&value->accelerometer_bottom[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_write_fields(roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_write_fields(&value->collision_indicators,p,offset);
  roswasmc_bounded_2_sensor_msgs_msg_JointState_write_fields(&value->measured_joint_state,p,offset);
  roswasmc_bounded_2_sensor_msgs_msg_JointState_write_fields(&value->desired_joint_state,p,offset);
  roswasmc_bounded_2_sensor_msgs_msg_JointState_write_fields(&value->measured_joint_motor_state,p,offset);
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ddq_d[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->dtau_j[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  roswasmc_bounded_2_sensor_msgs_msg_JointState_write_fields(&value->tau_ext_hat_filtered,p,offset);
  roswasmc_bounded_2_franka_msgs_msg_Elbow_write_fields(&value->elbow,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_write_fields(&value->k_f_ext_hat_k,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_write_fields(&value->o_f_ext_hat_k,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_write_fields(&value->inertia_ee,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_write_fields(&value->inertia_load,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_write_fields(&value->inertia_total,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(&value->o_t_ee,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(&value->o_t_ee_d,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(&value->o_t_ee_c,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(&value->f_t_ee,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(&value->ee_t_k,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_write_fields(&value->o_dp_ee_d,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_write_fields(&value->o_dp_ee_c,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_write_fields(&value->o_ddp_ee_c,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->time,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->control_command_success_rate,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->robot_mode,1);*offset+=1;
  roswasmc_bounded_2_franka_msgs_msg_Errors_write_fields(&value->current_errors,p,offset);
  roswasmc_bounded_2_franka_msgs_msg_Errors_write_fields(&value->last_motion_errors,p,offset);
  {size_t i;for(i=0;i<6u;++i){
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_write_fields(&value->accelerometer_top[i],p,offset);
  }}
  {size_t i;for(i=0;i<6u;++i){
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_write_fields(&value->accelerometer_bottom[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_CollisionIndicators_read_fields(p,n,offset,&value->collision_indicators))return 0;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields(p,n,offset,&value->measured_joint_state))return 0;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields(p,n,offset,&value->desired_joint_state))return 0;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields(p,n,offset,&value->measured_joint_motor_state))return 0;
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ddq_d[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->dtau_j[i],&bits,8);}*offset+=8;
  }}
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields(p,n,offset,&value->tau_ext_hat_filtered))return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_Elbow_read_fields(p,n,offset,&value->elbow))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_read_fields(p,n,offset,&value->k_f_ext_hat_k))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_WrenchStamped_read_fields(p,n,offset,&value->o_f_ext_hat_k))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_read_fields(p,n,offset,&value->inertia_ee))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_read_fields(p,n,offset,&value->inertia_load))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_InertiaStamped_read_fields(p,n,offset,&value->inertia_total))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields(p,n,offset,&value->o_t_ee))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields(p,n,offset,&value->o_t_ee_d))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields(p,n,offset,&value->o_t_ee_c))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields(p,n,offset,&value->f_t_ee))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields(p,n,offset,&value->ee_t_k))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_read_fields(p,n,offset,&value->o_dp_ee_d))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistStamped_read_fields(p,n,offset,&value->o_dp_ee_c))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_AccelStamped_read_fields(p,n,offset,&value->o_ddp_ee_c))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->time,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->control_command_success_rate,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->robot_mode,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_franka_msgs_msg_Errors_read_fields(p,n,offset,&value->current_errors))return 0;
  if(!roswasmc_bounded_2_franka_msgs_msg_Errors_read_fields(p,n,offset,&value->last_motion_errors))return 0;
  {size_t i;for(i=0;i<6u;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_read_fields(p,n,offset,&value->accelerometer_top[i]))return 0;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_read_fields(p,n,offset,&value->accelerometer_bottom[i]))return 0;
  }}
  return roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_encode(roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState* value) {
  roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState next={0};size_t offset=0;if(!value||(length&&!input)||length<1961u||length>10169u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_msg_FrankaRobotState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/srv/SetCartesianStiffness_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request {
  /** Declared field cartesian_stiffness; canonical capacity is part of CODEC_IDENTITY. */
  double cartesian_stiffness[6];
} roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETCARTESIANSTIFFNESS_REQUEST_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETCARTESIANSTIFFNESS_REQUEST_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETCARTESIANSTIFFNESS_REQUEST_CODEC_IDENTITY "UWVMROS1_ae78b9887a5419225cfe0727bac83d8c53f789acd53bd47f024485b02aa98260"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_valid(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_stiffness[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request const* value) {size_t n=0;
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->cartesian_stiffness[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request* value) {
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->cartesian_stiffness[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_encode(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/srv/SetCartesianStiffness_Response.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_error_text;
/** Complete ROS2 franka_msgs/srv/SetCartesianStiffness_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_error_text error;
} roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETCARTESIANSTIFFNESS_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETCARTESIANSTIFFNESS_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETCARTESIANSTIFFNESS_RESPONSE_CODEC_IDENTITY "UWVMROS1_3194f7469bbf228e653f5a93336fd5de4a1b2d7c1fe9542f3fadc8ad4fd51afa"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_valid(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_encode(roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetCartesianStiffness_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/srv/SetForceTorqueCollisionBehavior_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request {
  /** Declared field lower_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_torque_thresholds_nominal[7];
  /** Declared field upper_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_torque_thresholds_nominal[7];
  /** Declared field lower_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_force_thresholds_nominal[6];
  /** Declared field upper_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_force_thresholds_nominal[6];
} roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFORCETORQUECOLLISIONBEHAVIOR_REQUEST_MAX_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFORCETORQUECOLLISIONBEHAVIOR_REQUEST_MIN_WIRE_BYTES 208u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFORCETORQUECOLLISIONBEHAVIOR_REQUEST_CODEC_IDENTITY "UWVMROS1_829642a9ea418a53f7252f7489bd7f5ed976bf3689443012daf216db895cf99c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_valid(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request const* value) {size_t n=0;
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request* value) {
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_encode(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<208u||length>208u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/srv/SetForceTorqueCollisionBehavior_Response.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_error_text;
/** Complete ROS2 franka_msgs/srv/SetForceTorqueCollisionBehavior_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_error_text error;
} roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFORCETORQUECOLLISIONBEHAVIOR_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFORCETORQUECOLLISIONBEHAVIOR_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFORCETORQUECOLLISIONBEHAVIOR_RESPONSE_CODEC_IDENTITY "UWVMROS1_7d5640c34446e4262a8c00f0b242b68c0095688f0208fd692f21d855003eb47a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_valid(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_encode(roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetForceTorqueCollisionBehavior_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/srv/SetFullCollisionBehavior_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request {
  /** Declared field lower_torque_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double lower_torque_thresholds_acceleration[7];
  /** Declared field upper_torque_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double upper_torque_thresholds_acceleration[7];
  /** Declared field lower_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_torque_thresholds_nominal[7];
  /** Declared field upper_torque_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_torque_thresholds_nominal[7];
  /** Declared field lower_force_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double lower_force_thresholds_acceleration[6];
  /** Declared field upper_force_thresholds_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double upper_force_thresholds_acceleration[6];
  /** Declared field lower_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double lower_force_thresholds_nominal[6];
  /** Declared field upper_force_thresholds_nominal; canonical capacity is part of CODEC_IDENTITY. */
  double upper_force_thresholds_nominal[6];
} roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFULLCOLLISIONBEHAVIOR_REQUEST_MAX_WIRE_BYTES 416u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFULLCOLLISIONBEHAVIOR_REQUEST_MIN_WIRE_BYTES 416u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFULLCOLLISIONBEHAVIOR_REQUEST_CODEC_IDENTITY "UWVMROS1_d6898555d931aee3a5b4f5f97830cb9982f2e378c59c40bcaa0201840d2f2824"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_valid(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_acceleration[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request const* value) {size_t n=0;
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_torque_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_acceleration[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<6u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_force_thresholds_nominal[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request* value) {
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_torque_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_torque_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_torque_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_force_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_force_thresholds_acceleration[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_force_thresholds_nominal[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_encode(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<416u||length>416u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/srv/SetFullCollisionBehavior_Response.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_error_text;
/** Complete ROS2 franka_msgs/srv/SetFullCollisionBehavior_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_error_text error;
} roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFULLCOLLISIONBEHAVIOR_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFULLCOLLISIONBEHAVIOR_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETFULLCOLLISIONBEHAVIOR_RESPONSE_CODEC_IDENTITY "UWVMROS1_76430e384e9ad8b3f3c937e3004e8dc15a51d28abecd108c698e60d39c54f300"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_valid(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_encode(roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetFullCollisionBehavior_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/srv/SetJointStiffness_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request {
  /** Declared field joint_stiffness; canonical capacity is part of CODEC_IDENTITY. */
  double joint_stiffness[7];
} roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETJOINTSTIFFNESS_REQUEST_MAX_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETJOINTSTIFFNESS_REQUEST_MIN_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETJOINTSTIFFNESS_REQUEST_CODEC_IDENTITY "UWVMROS1_65b5d6599d1b97a0356f599102c22e0ae8c33b85f14c5a49b6c097eb227d334c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_valid(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_stiffness[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request const* value) {size_t n=0;
  {size_t i;for(i=0;i<7u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<7u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_stiffness[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request* value) {
  {size_t i;for(i=0;i<7u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_stiffness[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_encode(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<56u||length>56u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/srv/SetJointStiffness_Response.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_error_text;
/** Complete ROS2 franka_msgs/srv/SetJointStiffness_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_error_text error;
} roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETJOINTSTIFFNESS_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETJOINTSTIFFNESS_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETJOINTSTIFFNESS_RESPONSE_CODEC_IDENTITY "UWVMROS1_1a857ab809079e0eec664250a9521ddc1cdf66e61aa2e0f22eae8442f5231c4d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_valid(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_encode(roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetJointStiffness_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/srv/SetLoad_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  double mass;
  /** Declared field center_of_mass; canonical capacity is part of CODEC_IDENTITY. */
  double center_of_mass[3];
  /** Declared field load_inertia; canonical capacity is part of CODEC_IDENTITY. */
  double load_inertia[9];
} roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETLOAD_REQUEST_MAX_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETLOAD_REQUEST_MIN_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETLOAD_REQUEST_CODEC_IDENTITY "UWVMROS1_a616a844864b7bbf4ee18ac481ddd8406bec47354b4519a0e17f8a305635b7f8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_valid(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->mass,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->center_of_mass[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->load_inertia[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request const* value) {size_t n=0;
  n+=8;
  {size_t i;for(i=0;i<3u;++i){
  n+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->mass,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {size_t i;for(i=0;i<3u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->center_of_mass[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->load_inertia[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->mass,&bits,8);}*offset+=8;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->center_of_mass[i],&bits,8);}*offset+=8;
  }}
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->load_inertia[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_encode(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<104u||length>104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetLoad_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/srv/SetLoad_Response.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_error_text;
/** Complete ROS2 franka_msgs/srv/SetLoad_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_error_text error;
} roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETLOAD_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETLOAD_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETLOAD_RESPONSE_CODEC_IDENTITY "UWVMROS1_991ff575719ef9442063a13c45a9660c672f5607e6e40cf6ab5c7a3173fa00bf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_valid(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_encode(roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetLoad_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/srv/SetStiffnessFrame_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request {
  /** Declared field transformation; canonical capacity is part of CODEC_IDENTITY. */
  double transformation[16];
} roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETSTIFFNESSFRAME_REQUEST_MAX_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETSTIFFNESSFRAME_REQUEST_MIN_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETSTIFFNESSFRAME_REQUEST_CODEC_IDENTITY "UWVMROS1_f8ee91daba1642286b63178c18bf2a005597e94951f11a185241193132c9f402"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_valid(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->transformation[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request const* value) {size_t n=0;
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->transformation[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request* value) {
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->transformation[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_encode(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<128u||length>128u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/srv/SetStiffnessFrame_Response.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_error_text;
/** Complete ROS2 franka_msgs/srv/SetStiffnessFrame_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_error_text error;
} roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETSTIFFNESSFRAME_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETSTIFFNESSFRAME_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETSTIFFNESSFRAME_RESPONSE_CODEC_IDENTITY "UWVMROS1_e32cb83b1f0f3cda363566525c0b1fe367190f2e66f68282b330f4517f6ea744"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_valid(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_encode(roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetStiffnessFrame_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_msgs/srv/SetTCPFrame_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request {
  /** Declared field transformation; canonical capacity is part of CODEC_IDENTITY. */
  double transformation[16];
} roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETTCPFRAME_REQUEST_MAX_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETTCPFRAME_REQUEST_MIN_WIRE_BYTES 128u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETTCPFRAME_REQUEST_CODEC_IDENTITY "UWVMROS1_2ef445bfa494a6bc50ee3337695562255c06890216c30ba17df95e691871441f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_valid(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->transformation[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request const* value) {size_t n=0;
  {size_t i;for(i=0;i<16u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<16u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->transformation[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request* value) {
  {size_t i;for(i=0;i<16u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->transformation[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_encode(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<128u||length>128u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_msgs/srv/SetTCPFrame_Response.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_error_text;
/** Complete ROS2 franka_msgs/srv/SetTCPFrame_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_error_text error;
} roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETTCPFRAME_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETTCPFRAME_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FRANKA_MSGS_SRV_SETTCPFRAME_RESPONSE_CODEC_IDENTITY "UWVMROS1_87e4f5556c292289fe17898a40f1f5dd6161e7cc87ca570c86e9f69080d1df38"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_valid(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_wire_size(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_write_fields(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_encode(roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response* value) {
  roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_msgs_srv_SetTCPFrame_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/action/MoveAbsolute_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal {
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  double position;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  double velocity;
  /** Declared field acceleration; canonical capacity is part of CODEC_IDENTITY. */
  double acceleration;
  /** Declared field deceleration; canonical capacity is part of CODEC_IDENTITY. */
  double deceleration;
} roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_GOAL_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_GOAL_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_GOAL_CODEC_IDENTITY "UWVMROS1_a0c2f9037fd6730641b19b6da6ce1c6ca03bbd517fdd791c159effc93290cf13"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_valid(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->deceleration,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_wire_size(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_write_fields(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->acceleration,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->deceleration,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->acceleration,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->deceleration,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_encode(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal* value) {
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/action/MoveAbsolute_Result.stop_by; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_stop_by_text;
/** UTF-8 byte storage for franka_spine_msgs/action/MoveAbsolute_Result.error; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_error_text;
/** Complete ROS2 franka_spine_msgs/action/MoveAbsolute_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field stop_by; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_stop_by_text stop_by;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_error_text error;
} roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_RESULT_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_RESULT_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_RESULT_CODEC_IDENTITY "UWVMROS1_86b5c823e6e6604595462a76cd259daef420b2dff169f6dbe00f742f079155d5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_valid(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->stop_by.size>256u||!uwvm_bounded_utf8(value->stop_by.data,value->stop_by.size))return 0;
  if(value->error.size>256u||!uwvm_bounded_utf8(value->error.data,value->error.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_wire_size(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result const* value) {size_t n=0;
  n+=1;
  n+=4+value->stop_by.size;
  n+=4+value->error.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_write_fields(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->stop_by.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->stop_by.data,value->stop_by.size);*offset+=value->stop_by.size;
  uwvm_bounded_put(p+*offset,value->error.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error.data,value->error.size);*offset+=value->error.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->stop_by.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->stop_by.size>256u||value->stop_by.size>n-*offset)return 0;uwvm_bounded_copy(value->stop_by.data,p+*offset,value->stop_by.size);*offset+=value->stop_by.size;
  if(*offset>n||n-*offset<4)return 0;value->error.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error.size>256u||value->error.size>n-*offset)return 0;uwvm_bounded_copy(value->error.data,p+*offset,value->error.size);*offset+=value->error.size;
  return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_encode(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result* value) {
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/action/MoveAbsolute_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback {
  /** Declared field current_position; canonical capacity is part of CODEC_IDENTITY. */
  double current_position;
} roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_FEEDBACK_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_FEEDBACK_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_ACTION_MOVEABSOLUTE_FEEDBACK_CODEC_IDENTITY "UWVMROS1_cf2f760313ab8c6cc01dd79271619926320021746451f376e56d18f63ffad929"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_valid(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_position,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_wire_size(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_write_fields(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current_position,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->current_position,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_encode(roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback* value) {
  roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_action_MoveAbsolute_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/msg/UserLimits value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits {
  /** Declared field lower_limit; canonical capacity is part of CODEC_IDENTITY. */
  double lower_limit;
  /** Declared field upper_limit; canonical capacity is part of CODEC_IDENTITY. */
  double upper_limit;
} roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_USERLIMITS_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_USERLIMITS_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_USERLIMITS_CODEC_IDENTITY "UWVMROS1_20256e37f0ae710b5cb9ab9438b5ff8144ce2ab6fc606c5335d0f5de5384e368"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_valid(roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_limit,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_limit,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_wire_size(roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits const* value) {size_t n=0;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_write_fields(roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lower_limit,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->upper_limit,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lower_limit,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->upper_limit,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_encode(roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits* value) {
  roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/msg/SpineParameters value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters {
  /** Declared field user_limits; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits user_limits;
} roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINEPARAMETERS_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINEPARAMETERS_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINEPARAMETERS_CODEC_IDENTITY "UWVMROS1_4d4c45cfccf278c99a189e92d438d0b11b2b693012cb0f76b65b6822eb816223"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_valid(roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_valid(&value->user_limits))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_wire_size(roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters const* value) {size_t n=0;
  n+=roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_wire_size(&value->user_limits);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_write_fields(roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_write_fields(&value->user_limits,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters* value) {
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_UserLimits_read_fields(p,n,offset,&value->user_limits))return 0;
  return roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_encode(roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters* value) {
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/msg/SpinePosition value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  double position;
  /** Declared field valid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t valid;
} roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINEPOSITION_MAX_WIRE_BYTES 149u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINEPOSITION_MIN_WIRE_BYTES 21u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINEPOSITION_CODEC_IDENTITY "UWVMROS1_71be15c40f39074dfdbe79a3587847ae512c5a3c8df23cc9415d5bfe59d05775"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_valid(roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->valid>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_wire_size(roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=8;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_write_fields(roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->valid,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->valid,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_encode(roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition* value) {
  roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition next={0};size_t offset=0;if(!value||(length&&!input)||length<21u||length>149u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpinePosition_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/msg/SpineState.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_state_text;
/** UTF-8 byte storage for franka_spine_msgs/msg/SpineState.error_code; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_error_code_text;
/** UTF-8 byte storage for franka_spine_msgs/msg/SpineState.error_description; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_error_description_text;
/** Complete ROS2 franka_spine_msgs/msg/SpineState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_msg_SpineState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_state_text state;
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_error_code_text error_code;
  /** Declared field error_description; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_error_description_text error_description;
  /** Declared field valid; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t valid;
} roswasmc_bounded_2_franka_spine_msgs_msg_SpineState;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINESTATE_MAX_WIRE_BYTES 921u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINESTATE_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_MSG_SPINESTATE_CODEC_IDENTITY "UWVMROS1_ad0b5dc5d79956da854582765c147d3d33edab569c4829e69cc7990edfb89441"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_valid(roswasmc_bounded_2_franka_spine_msgs_msg_SpineState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  if(value->error_code.size>256u||!uwvm_bounded_utf8(value->error_code.data,value->error_code.size))return 0;
  if(value->error_description.size>256u||!uwvm_bounded_utf8(value->error_description.data,value->error_description.size))return 0;
  if(value->valid>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_wire_size(roswasmc_bounded_2_franka_spine_msgs_msg_SpineState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4+value->state.size;
  n+=4+value->error_code.size;
  n+=4+value->error_description.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_write_fields(roswasmc_bounded_2_franka_spine_msgs_msg_SpineState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
  uwvm_bounded_put(p+*offset,value->error_code.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_code.data,value->error_code.size);*offset+=value->error_code.size;
  uwvm_bounded_put(p+*offset,value->error_description.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_description.data,value->error_description.size);*offset+=value->error_description.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->valid,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_msg_SpineState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  if(*offset>n||n-*offset<4)return 0;value->error_code.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_code.size>256u||value->error_code.size>n-*offset)return 0;uwvm_bounded_copy(value->error_code.data,p+*offset,value->error_code.size);*offset+=value->error_code.size;
  if(*offset>n||n-*offset<4)return 0;value->error_description.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_description.size>256u||value->error_description.size>n-*offset)return 0;uwvm_bounded_copy(value->error_description.data,p+*offset,value->error_description.size);*offset+=value->error_description.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->valid,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_encode(roswasmc_bounded_2_franka_spine_msgs_msg_SpineState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_msg_SpineState* value) {
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineState next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>921u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpineState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/FaultReset_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_FAULTRESET_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_FAULTRESET_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_FAULTRESET_REQUEST_CODEC_IDENTITY "UWVMROS1_0f836229e8e8a1713b9e9a8d7f0f43677ad93380e1f7350ff1597d25067b7f33"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_valid(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request* value) {
  return roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_encode(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/srv/FaultReset_Response.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_state_text;
/** UTF-8 byte storage for franka_spine_msgs/srv/FaultReset_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_message_text;
/** Complete ROS2 franka_spine_msgs/srv/FaultReset_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_state_text state;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_message_text message;
} roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_FAULTRESET_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_FAULTRESET_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_FAULTRESET_RESPONSE_CODEC_IDENTITY "UWVMROS1_1ebeded0dbe4a4197a52de3fa43dc57fa372bd4e6ef5e2ded2558ccbf691410c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_valid(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response const* value) {size_t n=0;
  n+=4+value->state.size;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_encode(roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_FaultReset_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/GetParameters_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPARAMETERS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPARAMETERS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPARAMETERS_REQUEST_CODEC_IDENTITY "UWVMROS1_80ef5d2ef6ff2df9d14f29417d1928429315c0b7be9ec59513b9b3943a970b14"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_valid(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request* value) {
  return roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_encode(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/srv/GetParameters_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_message_text;
/** Complete ROS2 franka_spine_msgs/srv/GetParameters_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response {
  /** Declared field parameters; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters parameters;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_message_text message;
} roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPARAMETERS_RESPONSE_MAX_WIRE_BYTES 277u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPARAMETERS_RESPONSE_MIN_WIRE_BYTES 21u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPARAMETERS_RESPONSE_CODEC_IDENTITY "UWVMROS1_8e648792534d7f4996179f31d353d2594cf63031edef4451fd20a3838dc896ed"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_valid(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_valid(&value->parameters))return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_wire_size(&value->parameters);
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_write_fields(&value->parameters,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response* value) {
  if(!roswasmc_bounded_2_franka_spine_msgs_msg_SpineParameters_read_fields(p,n,offset,&value->parameters))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_encode(roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<21u||length>277u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetParameters_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/GetPosition_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPOSITION_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPOSITION_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPOSITION_REQUEST_CODEC_IDENTITY "UWVMROS1_66b05d1f08331161cafed301b3ab95a60dbb16aeb1a930ee780db163cb37c6c2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_valid(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request* value) {
  return roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_encode(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/GetPosition_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response {
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  double position;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPOSITION_RESPONSE_MAX_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPOSITION_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETPOSITION_RESPONSE_CODEC_IDENTITY "UWVMROS1_18d9af57cc9f82c3c03d01e850da254e9431820b8291f03dafd2e8f74e849ab8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_valid(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response const* value) {size_t n=0;
  n+=8;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_encode(roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>9u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetPosition_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/GetSpineState_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETSPINESTATE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETSPINESTATE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETSPINESTATE_REQUEST_CODEC_IDENTITY "UWVMROS1_c4c456174d7b6647d09d986d37c041b942418941a5e01c447486727e919b8a67"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_valid(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request* value) {
  return roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_encode(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/srv/GetSpineState_Response.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_state_text;
/** UTF-8 byte storage for franka_spine_msgs/srv/GetSpineState_Response.error_code; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_error_code_text;
/** UTF-8 byte storage for franka_spine_msgs/srv/GetSpineState_Response.error_description; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_error_description_text;
/** Complete ROS2 franka_spine_msgs/srv/GetSpineState_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_state_text state;
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_error_code_text error_code;
  /** Declared field error_description; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_error_description_text error_description;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETSPINESTATE_RESPONSE_MAX_WIRE_BYTES 781u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETSPINESTATE_RESPONSE_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_GETSPINESTATE_RESPONSE_CODEC_IDENTITY "UWVMROS1_297d70e2c5c24dcea4bdf8c5f9e5c8c32256e8f35f1900d8afd914ab2acfdcd1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_valid(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  if(value->error_code.size>256u||!uwvm_bounded_utf8(value->error_code.data,value->error_code.size))return 0;
  if(value->error_description.size>256u||!uwvm_bounded_utf8(value->error_description.data,value->error_description.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response const* value) {size_t n=0;
  n+=4+value->state.size;
  n+=4+value->error_code.size;
  n+=4+value->error_description.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
  uwvm_bounded_put(p+*offset,value->error_code.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_code.data,value->error_code.size);*offset+=value->error_code.size;
  uwvm_bounded_put(p+*offset,value->error_description.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_description.data,value->error_description.size);*offset+=value->error_description.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  if(*offset>n||n-*offset<4)return 0;value->error_code.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_code.size>256u||value->error_code.size>n-*offset)return 0;uwvm_bounded_copy(value->error_code.data,p+*offset,value->error_code.size);*offset+=value->error_code.size;
  if(*offset>n||n-*offset<4)return 0;value->error_description.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_description.size>256u||value->error_description.size>n-*offset)return 0;uwvm_bounded_copy(value->error_description.data,p+*offset,value->error_description.size);*offset+=value->error_description.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_encode(roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>781u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_GetSpineState_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/Halt_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_HALT_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_HALT_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_HALT_REQUEST_CODEC_IDENTITY "UWVMROS1_50af7625abf7ec20a30ec49b01e8d32de2ee0d4710820c8de659df211be8c111"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_valid(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request* value) {
  return roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_encode(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/srv/Halt_Response.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_state_text;
/** UTF-8 byte storage for franka_spine_msgs/srv/Halt_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_message_text;
/** Complete ROS2 franka_spine_msgs/srv/Halt_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_state_text state;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_message_text message;
} roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_HALT_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_HALT_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_HALT_RESPONSE_CODEC_IDENTITY "UWVMROS1_150e1d90b485a6ddf79f771379a0e1d3f99a77f88ab1cf1f569d614a63ac3d77"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_valid(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response const* value) {size_t n=0;
  n+=4+value->state.size;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_encode(roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_Halt_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/SwitchOff_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHOFF_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHOFF_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHOFF_REQUEST_CODEC_IDENTITY "UWVMROS1_097f00adfdc8d1a49615792158a36052917c41385683b39de6686f591964fc16"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_valid(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request* value) {
  return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_encode(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/srv/SwitchOff_Response.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_state_text;
/** UTF-8 byte storage for franka_spine_msgs/srv/SwitchOff_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_message_text;
/** Complete ROS2 franka_spine_msgs/srv/SwitchOff_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_state_text state;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_message_text message;
} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHOFF_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHOFF_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHOFF_RESPONSE_CODEC_IDENTITY "UWVMROS1_d3ab8516e62428b2caf71673f398ccc7199b7b25aac4c37b42c15a79878e54ad"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_valid(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response const* value) {size_t n=0;
  n+=4+value->state.size;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_encode(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOff_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 franka_spine_msgs/srv/SwitchOn_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHON_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHON_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHON_REQUEST_CODEC_IDENTITY "UWVMROS1_840d12dbe4d4a8558e72c2facbccfc547de35536f749573b6de60ce9a9238a56"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_valid(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request* value) {
  return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_encode(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for franka_spine_msgs/srv/SwitchOn_Response.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_state_text;
/** UTF-8 byte storage for franka_spine_msgs/srv/SwitchOn_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_message_text;
/** Complete ROS2 franka_spine_msgs/srv/SwitchOn_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_state_text state;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_message_text message;
} roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response;
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHON_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHON_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_FRANKA_SPINE_MSGS_SRV_SWITCHON_RESPONSE_CODEC_IDENTITY "UWVMROS1_8c58d63385a5fc6f160585504b3c9c5ca5375c6187f7a494c3db696c5cc7a37d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_valid(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_wire_size(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response const* value) {size_t n=0;
  n+=4+value->state.size;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_write_fields(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_encode(roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_decode(void const* input,size_t length,roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response* value) {
  roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_franka_spine_msgs_srv_SwitchOn_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
