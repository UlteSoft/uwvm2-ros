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
/** UTF-8 byte storage for fanuc_msgs/msg/IOType.type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_fanuc_msgs_msg_IOType_type_text;
/** Complete ROS2 fanuc_msgs/msg/IOType value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_IOType {
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_type_text type;
} roswasmc_bounded_2_fanuc_msgs_msg_IOType;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_CODEC_IDENTITY "UWVMROS1_1815aa6b1bcab1efe21de0e31570ca650553b5c70f0793301f82c0ca7934c586"
/** Original scalar constant AI=AI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_AI "\101\111"
/** Original scalar constant AO=AO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_AO "\101\117"
/** Original scalar constant DI=DI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_DI "\104\111"
/** Original scalar constant DO=DO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_DO "\104\117"
/** Original scalar constant RI=RI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_RI "\122\111"
/** Original scalar constant RO=RO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_RO "\122\117"
/** Original scalar constant SI=SI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_SI "\123\111"
/** Original scalar constant SO=SO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_SO "\123\117"
/** Original scalar constant WI=WI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_WI "\127\111"
/** Original scalar constant WO=WO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_WO "\127\117"
/** Original scalar constant UI=UI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_UI "\125\111"
/** Original scalar constant UO=UO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_UO "\125\117"
/** Original scalar constant WSI=WSI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_WSI "\127\123\111"
/** Original scalar constant WSO=WSO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_WSO "\127\123\117"
/** Original scalar constant F=F; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_F "\106"
/** Original scalar constant M=M; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_M "\115"
/** Original scalar constant GI=GI; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_GI "\107\111"
/** Original scalar constant GO=GO; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOTYPE_GO "\107\117"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(roswasmc_bounded_2_fanuc_msgs_msg_IOType const* value) {
  if(!value)return 0;
  if(value->type.size>64u||!uwvm_bounded_utf8(value->type.data,value->type.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_IOType const* value) {size_t n=0;
  n+=4+value->type.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_IOType const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->type.data,value->type.size);*offset+=value->type.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_IOType* value) {
  if(*offset>n||n-*offset<4)return 0;value->type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->type.size>64u||value->type.size>n-*offset)return 0;uwvm_bounded_copy(value->type.data,p+*offset,value->type.size);*offset+=value->type.size;
  return roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_IOType_encode(roswasmc_bounded_2_fanuc_msgs_msg_IOType const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_IOType_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_IOType* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/AnalogIO value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIO_MAX_WIRE_BYTES 74u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIO_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIO_CODEC_IDENTITY "UWVMROS1_d62097527ba99bf6e803283350161cd1415173a22eb3cb20331a232366076d00"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_valid(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_encode(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>74u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO data[32];} roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_values_sequence;
/** Complete ROS2 fanuc_msgs/msg/AnalogIOCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd {
  /** Declared field values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_values_sequence values;
} roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIOCMD_MAX_WIRE_BYTES 2372u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIOCMD_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIOCMD_CODEC_IDENTITY "UWVMROS1_62f38a088ed23f8daf7163f26514ae1aabd7b1b64845ccdbc281cb7839d857ba"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_valid(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd const* value) {
  if(!value)return 0;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_valid(&value->values.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd const* value) {size_t n=0;
  n+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  n+=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_wire_size(&value->values.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->values.size;++i){
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_write_fields(&value->values.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd* value) {
  if(*offset>n||n-*offset<4)return 0;value->values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_read_fields(p,n,offset,&value->values.data[i]))return 0;
  }}
  return roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_encode(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>2372u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO data[32];} roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_values_sequence;
/** Complete ROS2 fanuc_msgs/msg/AnalogIOState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState {
  /** Declared field values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_values_sequence values;
} roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIOSTATE_MAX_WIRE_BYTES 2372u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIOSTATE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ANALOGIOSTATE_CODEC_IDENTITY "UWVMROS1_820b41fecbf6db8e2b2e67d60cd1167cf474b22ca6c489f4d2d64dd33eaac9a4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_valid(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState const* value) {
  if(!value)return 0;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_valid(&value->values.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState const* value) {size_t n=0;
  n+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  n+=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_wire_size(&value->values.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->values.size;++i){
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_write_fields(&value->values.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState* value) {
  if(*offset>n||n-*offset<4)return 0;value->values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIO_read_fields(p,n,offset,&value->values.data[i]))return 0;
  }}
  return roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_encode(roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>2372u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_AnalogIOState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/BoolIO value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_BoolIO {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_fanuc_msgs_msg_BoolIO;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_BOOLIO_MAX_WIRE_BYTES 71u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_BOOLIO_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_BOOLIO_CODEC_IDENTITY "UWVMROS1_9209ae1b8f5407a31beface0a3286f58bd823aabdc560c2a4bf4a9b1ca125aa7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_valid(roswasmc_bounded_2_fanuc_msgs_msg_BoolIO const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  if(value->value>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_BoolIO const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_BoolIO const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_BoolIO* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_encode(roswasmc_bounded_2_fanuc_msgs_msg_BoolIO const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_BoolIO* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_BoolIO next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>71u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/CollaborativeSpeedScaling value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling {
  /** Declared field collaborative_speed_scaling; canonical capacity is part of CODEC_IDENTITY. */
  double collaborative_speed_scaling;
} roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_COLLABORATIVESPEEDSCALING_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_COLLABORATIVESPEEDSCALING_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_COLLABORATIVESPEEDSCALING_CODEC_IDENTITY "UWVMROS1_693a3b581527d2eaf0f697f31e87ca2da428d1f51b821d42d6b06af387a2fb45"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_valid(roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->collaborative_speed_scaling,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->collaborative_speed_scaling,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->collaborative_speed_scaling,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_encode(roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_CollaborativeSpeedScaling_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/ConnectionStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus {
  /** Declared field is_connected; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_connected;
} roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_CONNECTIONSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_CONNECTIONSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_CONNECTIONSTATUS_CODEC_IDENTITY "UWVMROS1_8ce24fb87538922b1f3c0ae856eb223d716143064eb46a6cfce5fec01b45b078"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_valid(roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus const* value) {
  if(!value)return 0;
  if(value->is_connected>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_connected,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_connected,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_encode(roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_ConnectionStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/ForceSensor value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor {
  /** Declared field force_x; canonical capacity is part of CODEC_IDENTITY. */
  float force_x;
  /** Declared field force_y; canonical capacity is part of CODEC_IDENTITY. */
  float force_y;
  /** Declared field force_z; canonical capacity is part of CODEC_IDENTITY. */
  float force_z;
  /** Declared field moment_x; canonical capacity is part of CODEC_IDENTITY. */
  float moment_x;
  /** Declared field moment_y; canonical capacity is part of CODEC_IDENTITY. */
  float moment_y;
  /** Declared field moment_z; canonical capacity is part of CODEC_IDENTITY. */
  float moment_z;
  /** Declared field fs_type; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t fs_type;
} roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_FORCESENSOR_MAX_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_FORCESENSOR_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_FORCESENSOR_CODEC_IDENTITY "UWVMROS1_06c99b43d7347646179850f3ba79192da7ce71296667d93d88659efbc1004032"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_valid(roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->force_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->force_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->force_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->moment_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->moment_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->moment_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->force_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->force_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->force_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->moment_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->moment_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->moment_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->fs_type,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->force_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->force_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->force_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->moment_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->moment_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->moment_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->fs_type,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_encode(roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>28u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_ForceSensor_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/Frame value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_Frame {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field w; canonical capacity is part of CODEC_IDENTITY. */
  float w;
  /** Declared field p; canonical capacity is part of CODEC_IDENTITY. */
  float p;
  /** Declared field r; canonical capacity is part of CODEC_IDENTITY. */
  float r;
} roswasmc_bounded_2_fanuc_msgs_msg_Frame;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_FRAME_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_FRAME_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_FRAME_CODEC_IDENTITY "UWVMROS1_609d1b7e49315f7f0a0fe86fa997673acd0d009e57b563690f36624b59b9effb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(roswasmc_bounded_2_fanuc_msgs_msg_Frame const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_Frame_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_Frame const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_Frame_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_Frame const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_Frame_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_Frame* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->w,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->p,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->r,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_Frame_encode(roswasmc_bounded_2_fanuc_msgs_msg_Frame const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_Frame_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_Frame_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_Frame_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_Frame* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_Frame next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_fanuc_msgs_msg_BoolIO data[32];} roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_values_sequence;
/** Complete ROS2 fanuc_msgs/msg/IOCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_IOCmd {
  /** Declared field values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_values_sequence values;
} roswasmc_bounded_2_fanuc_msgs_msg_IOCmd;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOCMD_MAX_WIRE_BYTES 2276u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOCMD_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOCMD_CODEC_IDENTITY "UWVMROS1_462552bbaa3c9ed4caaf3108960d514e4ab61fcf2d6a84f0465b485bec1de45d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_valid(roswasmc_bounded_2_fanuc_msgs_msg_IOCmd const* value) {
  if(!value)return 0;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_valid(&value->values.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_IOCmd const* value) {size_t n=0;
  n+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  n+=roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_wire_size(&value->values.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_IOCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->values.size;++i){
  roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_write_fields(&value->values.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_IOCmd* value) {
  if(*offset>n||n-*offset<4)return 0;value->values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_read_fields(p,n,offset,&value->values.data[i]))return 0;
  }}
  return roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_encode(roswasmc_bounded_2_fanuc_msgs_msg_IOCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_IOCmd* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>2276u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_fanuc_msgs_msg_BoolIO data[32];} roswasmc_bounded_2_fanuc_msgs_msg_IOState_values_sequence;
/** Complete ROS2 fanuc_msgs/msg/IOState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_IOState {
  /** Declared field values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOState_values_sequence values;
} roswasmc_bounded_2_fanuc_msgs_msg_IOState;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOSTATE_MAX_WIRE_BYTES 2276u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOSTATE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_IOSTATE_CODEC_IDENTITY "UWVMROS1_332533445014c03d626a2c4dbdffd9421748aa6b73f49d407441fd6b9f2b24d8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_IOState_valid(roswasmc_bounded_2_fanuc_msgs_msg_IOState const* value) {
  if(!value)return 0;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_valid(&value->values.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_IOState_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_IOState const* value) {size_t n=0;
  n+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  n+=roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_wire_size(&value->values.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_IOState_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_IOState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->values.size;++i){
  roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_write_fields(&value->values.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_IOState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_IOState* value) {
  if(*offset>n||n-*offset<4)return 0;value->values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_BoolIO_read_fields(p,n,offset,&value->values.data[i]))return 0;
  }}
  return roswasmc_bounded_2_fanuc_msgs_msg_IOState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_IOState_encode(roswasmc_bounded_2_fanuc_msgs_msg_IOState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_IOState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_IOState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_IOState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_IOState_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_IOState* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOState next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>2276u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/NumReg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_NumReg {
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_2_fanuc_msgs_msg_NumReg;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREG_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREG_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREG_CODEC_IDENTITY "UWVMROS1_0fb9344c5768ea52ab072939f368f1e73dcc0ff83e74b4cb6d35fca47c1d29cc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_NumReg_valid(roswasmc_bounded_2_fanuc_msgs_msg_NumReg const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_NumReg_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_NumReg const* value) {size_t n=0;
  n+=2;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_NumReg_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_NumReg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_NumReg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_NumReg* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_msg_NumReg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_NumReg_encode(roswasmc_bounded_2_fanuc_msgs_msg_NumReg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_NumReg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_NumReg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_NumReg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_NumReg_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_NumReg* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_NumReg next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_NumReg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_fanuc_msgs_msg_NumReg data[32];} roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_values_sequence;
/** Complete ROS2 fanuc_msgs/msg/NumRegCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd {
  /** Declared field values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_values_sequence values;
} roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREGCMD_MAX_WIRE_BYTES 196u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREGCMD_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREGCMD_CODEC_IDENTITY "UWVMROS1_9c76500073889377525a0802c46bd96589a2639dd9534cf7ad2b9956cd95b5b6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_valid(roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd const* value) {
  if(!value)return 0;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_NumReg_valid(&value->values.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd const* value) {size_t n=0;
  n+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  n+=roswasmc_bounded_2_fanuc_msgs_msg_NumReg_wire_size(&value->values.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->values.size;++i){
  roswasmc_bounded_2_fanuc_msgs_msg_NumReg_write_fields(&value->values.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd* value) {
  if(*offset>n||n-*offset<4)return 0;value->values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_NumReg_read_fields(p,n,offset,&value->values.data[i]))return 0;
  }}
  return roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_encode(roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>196u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_NumRegCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_fanuc_msgs_msg_NumReg data[32];} roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_values_sequence;
/** Complete ROS2 fanuc_msgs/msg/NumRegState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_NumRegState {
  /** Declared field values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_values_sequence values;
} roswasmc_bounded_2_fanuc_msgs_msg_NumRegState;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREGSTATE_MAX_WIRE_BYTES 196u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREGSTATE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_NUMREGSTATE_CODEC_IDENTITY "UWVMROS1_9c8ff4917a6dd033e6a6776ae8525e93c2924d776f05fd54e7e8d2251fc48b3a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_valid(roswasmc_bounded_2_fanuc_msgs_msg_NumRegState const* value) {
  if(!value)return 0;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_NumReg_valid(&value->values.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_NumRegState const* value) {size_t n=0;
  n+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  n+=roswasmc_bounded_2_fanuc_msgs_msg_NumReg_wire_size(&value->values.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_NumRegState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->values.size;++i){
  roswasmc_bounded_2_fanuc_msgs_msg_NumReg_write_fields(&value->values.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_NumRegState* value) {
  if(*offset>n||n-*offset<4)return 0;value->values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->values.size>32u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(!roswasmc_bounded_2_fanuc_msgs_msg_NumReg_read_fields(p,n,offset,&value->values.data[i]))return 0;
  }}
  return roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_encode(roswasmc_bounded_2_fanuc_msgs_msg_NumRegState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_NumRegState* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_NumRegState next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>196u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_NumRegState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for fanuc_msgs/msg/RMIStatus.last_sequence_instruction; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_last_sequence_instruction_text;
/** UTF-8 byte storage for fanuc_msgs/msg/RMIStatus.last_sequence_error_message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_last_sequence_error_message_text;
/** Complete ROS2 fanuc_msgs/msg/RMIStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus {
  /** Declared field program_status; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t program_status;
  /** Declared field next_sequence; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t next_sequence;
  /** Declared field last_sequence; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t last_sequence;
  /** Declared field last_sequence_error; canonical capacity is part of CODEC_IDENTITY. */
  int32_t last_sequence_error;
  /** Declared field last_sequence_instruction; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_last_sequence_instruction_text last_sequence_instruction;
  /** Declared field last_sequence_error_message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_last_sequence_error_message_text last_sequence_error_message;
} roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_RMISTATUS_MAX_WIRE_BYTES 1048u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_RMISTATUS_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_RMISTATUS_CODEC_IDENTITY "UWVMROS1_dc2e7eaabf44807142581905838f8951653682a6c6d61ebcf56494b16a10d5e8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_valid(roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus const* value) {
  if(!value)return 0;
  if(value->last_sequence_instruction.size>512u||!uwvm_bounded_utf8(value->last_sequence_instruction.data,value->last_sequence_instruction.size))return 0;
  if(value->last_sequence_error_message.size>512u||!uwvm_bounded_utf8(value->last_sequence_error_message.data,value->last_sequence_error_message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4+value->last_sequence_instruction.size;
  n+=4+value->last_sequence_error_message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->program_status,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->next_sequence,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->last_sequence,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->last_sequence_error,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->last_sequence_instruction.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->last_sequence_instruction.data,value->last_sequence_instruction.size);*offset+=value->last_sequence_instruction.size;
  uwvm_bounded_put(p+*offset,value->last_sequence_error_message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->last_sequence_error_message.data,value->last_sequence_error_message.size);*offset+=value->last_sequence_error_message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->program_status,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->next_sequence,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->last_sequence,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->last_sequence_error,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->last_sequence_instruction.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->last_sequence_instruction.size>512u||value->last_sequence_instruction.size>n-*offset)return 0;uwvm_bounded_copy(value->last_sequence_instruction.data,p+*offset,value->last_sequence_instruction.size);*offset+=value->last_sequence_instruction.size;
  if(*offset>n||n-*offset<4)return 0;value->last_sequence_error_message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->last_sequence_error_message.size>512u||value->last_sequence_error_message.size>n-*offset)return 0;uwvm_bounded_copy(value->last_sequence_error_message.data,p+*offset,value->last_sequence_error_message.size);*offset+=value->last_sequence_error_message.size;
  return roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_encode(roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>1048u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_RMIStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/msg/RobotStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus {
  /** Declared field in_error; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t in_error;
  /** Declared field tp_enabled; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t tp_enabled;
  /** Declared field e_stopped; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t e_stopped;
  /** Declared field motion_possible; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t motion_possible;
  /** Declared field contact_stop_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t contact_stop_mode;
} roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_CODEC_IDENTITY "UWVMROS1_3f31ee3631dc6f4c90eb5ded6209ccebf0f4193ddfe5f5032bdd2bcd71f8cc31"
/** Original scalar constant CONTACT_STOP_MODE_NONE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_CONTACT_STOP_MODE_NONE UINT32_C(0)
/** Original scalar constant CONTACT_STOP_MODE_SAFE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_CONTACT_STOP_MODE_SAFE UINT32_C(1)
/** Original scalar constant CONTACT_STOP_MODE_STOP=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_CONTACT_STOP_MODE_STOP UINT32_C(2)
/** Original scalar constant CONTACT_STOP_MODE_DSBL=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_CONTACT_STOP_MODE_DSBL UINT32_C(3)
/** Original scalar constant CONTACT_STOP_MODE_ESCP=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUS_CONTACT_STOP_MODE_ESCP UINT32_C(4)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_valid(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus const* value) {
  if(!value)return 0;
  if(value->in_error>1)return 0;
  if(value->tp_enabled>1)return 0;
  if(value->e_stopped>1)return 0;
  if(value->motion_possible>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->in_error,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->tp_enabled,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->e_stopped,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->motion_possible,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->contact_stop_mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->in_error,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tp_enabled,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->e_stopped,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->motion_possible,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->contact_stop_mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_encode(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_RobotStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for fanuc_msgs/msg/RobotStatusExt.error_code; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_error_code_text;
/** UTF-8 byte storage for fanuc_msgs/msg/RobotStatusExt.control_mode; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_control_mode_text;
/** Complete ROS2 fanuc_msgs/msg/RobotStatusExt value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt {
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_error_code_text error_code;
  /** Declared field in_motion; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t in_motion;
  /** Declared field drives_powered; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t drives_powered;
  /** Declared field gen_override; canonical capacity is part of CODEC_IDENTITY. */
  int32_t gen_override;
  /** Declared field speed_clamp_limit; canonical capacity is part of CODEC_IDENTITY. */
  float speed_clamp_limit;
  /** Declared field control_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_control_mode_text control_mode;
} roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUSEXT_MAX_WIRE_BYTES 1042u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUSEXT_MIN_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_MSG_ROBOTSTATUSEXT_CODEC_IDENTITY "UWVMROS1_8ba47a11bfbe02026fe79b32e1ed02e5f609499ea55bb57c9f09af9fba389648"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_valid(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt const* value) {
  if(!value)return 0;
  if(value->error_code.size>512u||!uwvm_bounded_utf8(value->error_code.data,value->error_code.size))return 0;
  if(value->in_motion>1)return 0;
  if(value->drives_powered>1)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->speed_clamp_limit,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->control_mode.size>512u||!uwvm_bounded_utf8(value->control_mode.data,value->control_mode.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_wire_size(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt const* value) {size_t n=0;
  n+=4+value->error_code.size;
  n+=1;
  n+=1;
  n+=4;
  n+=4;
  n+=4+value->control_mode.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_write_fields(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->error_code.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_code.data,value->error_code.size);*offset+=value->error_code.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->in_motion,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->drives_powered,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->gen_override,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->speed_clamp_limit,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,value->control_mode.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->control_mode.data,value->control_mode.size);*offset+=value->control_mode.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt* value) {
  if(*offset>n||n-*offset<4)return 0;value->error_code.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_code.size>512u||value->error_code.size>n-*offset)return 0;uwvm_bounded_copy(value->error_code.data,p+*offset,value->error_code.size);*offset+=value->error_code.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->in_motion,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->drives_powered,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gen_override,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->speed_clamp_limit,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->control_mode.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->control_mode.size>512u||value->control_mode.size>n-*offset)return 0;uwvm_bounded_copy(value->control_mode.data,p+*offset,value->control_mode.size);*offset+=value->control_mode.size;
  return roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_encode(roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt* value) {
  roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt next={0};size_t offset=0;if(!value||(length&&!input)||length<18u||length>1042u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_RobotStatusExt_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/CfgForceSensor_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request {
  /** Declared field do_reset; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t do_reset;
  /** Declared field fs_type; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t fs_type;
} roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_CFGFORCESENSOR_REQUEST_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_CFGFORCESENSOR_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_CFGFORCESENSOR_REQUEST_CODEC_IDENTITY "UWVMROS1_01973ed21546cb747ad2ce2a990a8ec4e70efb66e1c8fc8856cf1082870984ff"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->do_reset,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->fs_type,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->do_reset,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->fs_type,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/CfgForceSensor_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_CFGFORCESENSOR_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_CFGFORCESENSOR_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_CFGFORCESENSOR_RESPONSE_CODEC_IDENTITY "UWVMROS1_41799ce44c937b1524889391db61e2b4d35795055cb4bbacfe5cc8bbd5b9a37b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_CfgForceSensor_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetAnalogIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
} roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETANALOGIO_REQUEST_MAX_WIRE_BYTES 70u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETANALOGIO_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETANALOGIO_REQUEST_CODEC_IDENTITY "UWVMROS1_cd567f97de2de581975c3d4f280eec545dea37c941d316fc3708bbc4d3af94d9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>70u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetAnalogIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETANALOGIO_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETANALOGIO_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETANALOGIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_3b53d3ce95d95454a782778c160fcc102ead869c5681c9c67224c373fef1b217"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetAnalogIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetBoolIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
} roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETBOOLIO_REQUEST_MAX_WIRE_BYTES 70u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETBOOLIO_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETBOOLIO_REQUEST_CODEC_IDENTITY "UWVMROS1_ce8c4b1efd2828e92656e897baf041e8965367a236079231101807c898d4b50c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>70u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetBoolIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETBOOLIO_RESPONSE_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETBOOLIO_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETBOOLIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_7dbdcdde8d76c4ef422ea158551dd912273220c25a08f28fc8d19fdf8bc8a01c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response const* value) {
  if(!value)return 0;
  if(value->value>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetBoolIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetCartesianPosition_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request {
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETCARTESIANPOSITION_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETCARTESIANPOSITION_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETCARTESIANPOSITION_REQUEST_CODEC_IDENTITY "UWVMROS1_a694a17bde88131efcadac4f134464a7777beab4b09e17624ae7b173ab62c3e4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetCartesianPosition_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field timetag; canonical capacity is part of CODEC_IDENTITY. */
  int32_t timetag;
  /** Declared field utool; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool;
  /** Declared field uframe; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe;
  /** Declared field front; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t front;
  /** Declared field up; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t up;
  /** Declared field left; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t left;
  /** Declared field flip; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flip;
  /** Declared field turn4; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn4;
  /** Declared field turn5; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn5;
  /** Declared field turn6; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn6;
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field w; canonical capacity is part of CODEC_IDENTITY. */
  float w;
  /** Declared field p; canonical capacity is part of CODEC_IDENTITY. */
  float p;
  /** Declared field r; canonical capacity is part of CODEC_IDENTITY. */
  float r;
  /** Declared field ext1; canonical capacity is part of CODEC_IDENTITY. */
  float ext1;
  /** Declared field ext2; canonical capacity is part of CODEC_IDENTITY. */
  float ext2;
  /** Declared field ext3; canonical capacity is part of CODEC_IDENTITY. */
  float ext3;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETCARTESIANPOSITION_RESPONSE_MAX_WIRE_BYTES 54u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETCARTESIANPOSITION_RESPONSE_MIN_WIRE_BYTES 54u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETCARTESIANPOSITION_RESPONSE_CODEC_IDENTITY "UWVMROS1_0516f0145717b06550674e02f0b9c47f9b312f4f361c791f56257fbb79e67329"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->timetag,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->front,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->up,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->left,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->flip,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn4,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn5,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn6,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->timetag,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->front,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->up,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->left,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flip,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn4,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn5,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn6,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->w,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->p,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->r,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<54u||length>54u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetCartesianPosition_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetGroupIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
} roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETGROUPIO_REQUEST_MAX_WIRE_BYTES 70u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETGROUPIO_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETGROUPIO_REQUEST_CODEC_IDENTITY "UWVMROS1_dbd2e8e791e8debc745843827ed296b51bf84159f4c4fe5d912c231d621de216"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>70u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetGroupIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t value;
} roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETGROUPIO_RESPONSE_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETGROUPIO_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETGROUPIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_9e131b91f36badc90ddae54a9855095241ab86c51b419b684d62879fae0bda7f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response const* value) {size_t n=0;
  n+=4;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->value,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetGroupIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetNumReg_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request {
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
} roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETNUMREG_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETNUMREG_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETNUMREG_REQUEST_CODEC_IDENTITY "UWVMROS1_727b5ff4ab9c8976280df12e80d3aee03b31288087adb94ee82161574fc8dcb5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetNumReg_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETNUMREG_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETNUMREG_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETNUMREG_RESPONSE_CODEC_IDENTITY "UWVMROS1_78ae7ff82b3b2cd7d512fee8335f14a0759a71425fcf3d1b150dbe5e2a0179b3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetNumReg_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadComp_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request {
  /** Declared field payload_schedule_id; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t payload_schedule_id;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADCOMP_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADCOMP_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADCOMP_REQUEST_CODEC_IDENTITY "UWVMROS1_d48b88619f4a68ceeed02336e9ef078c27905707a21d977448fbbfbd560f398d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->payload_schedule_id,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->payload_schedule_id,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadComp_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field cg_x; canonical capacity is part of CODEC_IDENTITY. */
  float cg_x;
  /** Declared field cg_y; canonical capacity is part of CODEC_IDENTITY. */
  float cg_y;
  /** Declared field cg_z; canonical capacity is part of CODEC_IDENTITY. */
  float cg_z;
  /** Declared field in_x; canonical capacity is part of CODEC_IDENTITY. */
  float in_x;
  /** Declared field in_y; canonical capacity is part of CODEC_IDENTITY. */
  float in_y;
  /** Declared field in_z; canonical capacity is part of CODEC_IDENTITY. */
  float in_z;
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADCOMP_RESPONSE_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADCOMP_RESPONSE_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADCOMP_RESPONSE_CODEC_IDENTITY "UWVMROS1_012b216c949f99755a36678b6be28d032f698b472ce4fa9ba975bc6d453ae4e4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadComp_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadID_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADID_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADID_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADID_REQUEST_CODEC_IDENTITY "UWVMROS1_6c943659905c8adc7aa4b73832584994c83bfe009d18dc30a11d99b9fc0d6eef"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request* value) {
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadID_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response {
  /** Declared field payload_schedule_id; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t payload_schedule_id;
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADID_RESPONSE_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADID_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADID_RESPONSE_CODEC_IDENTITY "UWVMROS1_5551f4eab590c87fd7679d593c9c446903bdaa02aa1ed2512b146768122a950c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response const* value) {size_t n=0;
  n+=2;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->payload_schedule_id,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->payload_schedule_id,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadID_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadValue_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request {
  /** Declared field payload_schedule_id; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t payload_schedule_id;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADVALUE_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADVALUE_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADVALUE_REQUEST_CODEC_IDENTITY "UWVMROS1_d82667bd644873ae4fa5de0289ed94791eb2a7af49c1b8f86ed7f91cf875407a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->payload_schedule_id,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->payload_schedule_id,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetPayloadValue_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field cg_x; canonical capacity is part of CODEC_IDENTITY. */
  float cg_x;
  /** Declared field cg_y; canonical capacity is part of CODEC_IDENTITY. */
  float cg_y;
  /** Declared field cg_z; canonical capacity is part of CODEC_IDENTITY. */
  float cg_z;
  /** Declared field in_x; canonical capacity is part of CODEC_IDENTITY. */
  float in_x;
  /** Declared field in_y; canonical capacity is part of CODEC_IDENTITY. */
  float in_y;
  /** Declared field in_z; canonical capacity is part of CODEC_IDENTITY. */
  float in_z;
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADVALUE_RESPONSE_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADVALUE_RESPONSE_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPAYLOADVALUE_RESPONSE_CODEC_IDENTITY "UWVMROS1_c3361ceec2ccd5b4d3ef566cbd885f8cddba75b4f1dc1712144147827689f06d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response const* value) {size_t n=0;
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
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPayloadValue_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetPosReg_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request {
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPOSREG_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPOSREG_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPOSREG_REQUEST_CODEC_IDENTITY "UWVMROS1_f7f1391c5ea385bff615d95c5e38f01a809828b142112829dc436916b063505a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for fanuc_msgs/srv/GetPosReg_Response.representation; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_representation_text;
/** Complete ROS2 fanuc_msgs/srv/GetPosReg_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field representation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_representation_text representation;
  /** Declared field utool; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool;
  /** Declared field uframe; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe;
  /** Declared field front; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t front;
  /** Declared field up; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t up;
  /** Declared field left; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t left;
  /** Declared field flip; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flip;
  /** Declared field turn4; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn4;
  /** Declared field turn5; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn5;
  /** Declared field turn6; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn6;
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field w; canonical capacity is part of CODEC_IDENTITY. */
  float w;
  /** Declared field p; canonical capacity is part of CODEC_IDENTITY. */
  float p;
  /** Declared field r; canonical capacity is part of CODEC_IDENTITY. */
  float r;
  /** Declared field ext1; canonical capacity is part of CODEC_IDENTITY. */
  float ext1;
  /** Declared field ext2; canonical capacity is part of CODEC_IDENTITY. */
  float ext2;
  /** Declared field ext3; canonical capacity is part of CODEC_IDENTITY. */
  float ext3;
  /** Declared field j1; canonical capacity is part of CODEC_IDENTITY. */
  float j1;
  /** Declared field j2; canonical capacity is part of CODEC_IDENTITY. */
  float j2;
  /** Declared field j3; canonical capacity is part of CODEC_IDENTITY. */
  float j3;
  /** Declared field j4; canonical capacity is part of CODEC_IDENTITY. */
  float j4;
  /** Declared field j5; canonical capacity is part of CODEC_IDENTITY. */
  float j5;
  /** Declared field j6; canonical capacity is part of CODEC_IDENTITY. */
  float j6;
  /** Declared field j7; canonical capacity is part of CODEC_IDENTITY. */
  float j7;
  /** Declared field j8; canonical capacity is part of CODEC_IDENTITY. */
  float j8;
  /** Declared field j9; canonical capacity is part of CODEC_IDENTITY. */
  float j9;
} roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPOSREG_RESPONSE_MAX_WIRE_BYTES 601u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPOSREG_RESPONSE_MIN_WIRE_BYTES 89u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETPOSREG_RESPONSE_CODEC_IDENTITY "UWVMROS1_e540225b0d532c460905582ad99ee175dd771e971c7e75f5dfb9325d39ed6b27"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response const* value) {
  if(!value)return 0;
  if(value->representation.size>512u||!uwvm_bounded_utf8(value->representation.data,value->representation.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j8,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j9,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->representation.size;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
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
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->representation.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->representation.data,value->representation.size);*offset+=value->representation.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->front,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->up,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->left,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->flip,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn4,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn5,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn6,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j8,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j9,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->representation.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->representation.size>512u||value->representation.size>n-*offset)return 0;uwvm_bounded_copy(value->representation.data,p+*offset,value->representation.size);*offset+=value->representation.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->front,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->up,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->left,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flip,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn4,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn5,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn6,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->w,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->p,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->r,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j8,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j9,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<89u||length>601u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetPosReg_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetTCPSpeed_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request {
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETTCPSPEED_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETTCPSPEED_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETTCPSPEED_REQUEST_CODEC_IDENTITY "UWVMROS1_6bdfd44465b75bbd4e232f802dbd3c0bfa28e90a93f9a28b3c15fb150bce7ac7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetTCPSpeed_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field timetag; canonical capacity is part of CODEC_IDENTITY. */
  int32_t timetag;
  /** Declared field speed; canonical capacity is part of CODEC_IDENTITY. */
  float speed;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETTCPSPEED_RESPONSE_MAX_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETTCPSPEED_RESPONSE_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETTCPSPEED_RESPONSE_CODEC_IDENTITY "UWVMROS1_3ec07d99342f275506452f32076f9e1ea01bf32f43662311708a9b44ca951147"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->speed,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->timetag,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->speed,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->timetag,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->speed,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>13u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetTCPSpeed_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameData_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request {
  /** Declared field uframe_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe_number;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEDATA_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEDATA_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEDATA_REQUEST_CODEC_IDENTITY "UWVMROS1_c1ef7077fd7f61ed8ff46931c8e39c5950658cfe1d36cffac10083ab304307be"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameData_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field uframe_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe_number;
  /** Declared field frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_Frame frame;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEDATA_RESPONSE_MAX_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEDATA_RESPONSE_MIN_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEDATA_RESPONSE_CODEC_IDENTITY "UWVMROS1_f956e1447bd4a15f547fdf5d6775836018d50153270fe8863c4b30d83422fd2e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(&value->frame))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_Frame_wire_size(&value->frame);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe_number,1);*offset+=1;
  roswasmc_bounded_2_fanuc_msgs_msg_Frame_write_fields(&value->frame,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe_number,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_read_fields(p,n,offset,&value->frame))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<30u||length>30u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameData_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameUTool_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request {
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEUTOOL_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEUTOOL_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEUTOOL_REQUEST_CODEC_IDENTITY "UWVMROS1_3d57da6f8003b33b2a079d4f284eb0f182d7e088074112b7f4ee25d3113bd443"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetUFrameUTool_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field uframe_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe_number;
  /** Declared field utool_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool_number;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEUTOOL_RESPONSE_MAX_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEUTOOL_RESPONSE_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUFRAMEUTOOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_f9cb64b89f17b6811e4073b9f94d0fd6844d21ec4b846f78bfd13cb14a35a6b3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>7u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUFrameUTool_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetUToolData_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request {
  /** Declared field utool_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool_number;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUTOOLDATA_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUTOOLDATA_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUTOOLDATA_REQUEST_CODEC_IDENTITY "UWVMROS1_10c3799dcb1b09dbcb6d1b794a00ecfaf69a8f457d95eeb92855f79dba7be1bc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/GetUToolData_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field utool_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool_number;
  /** Declared field frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_Frame frame;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUTOOLDATA_RESPONSE_MAX_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUTOOLDATA_RESPONSE_MIN_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_GETUTOOLDATA_RESPONSE_CODEC_IDENTITY "UWVMROS1_f3a2b94f337cec9c58156b803675f38885350cc3fc3171234c0d361afc800d81"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(&value->frame))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_Frame_wire_size(&value->frame);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool_number,1);*offset+=1;
  roswasmc_bounded_2_fanuc_msgs_msg_Frame_write_fields(&value->frame,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool_number,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_read_fields(p,n,offset,&value->frame))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<30u||length>30u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_GetUToolData_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/ReadError_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request {
  /** Declared field count; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t count;
} roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_READERROR_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_READERROR_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_READERROR_REQUEST_CODEC_IDENTITY "UWVMROS1_4b67ed8e2ecccefb6bdead80249f5916e6ea192d822183292db43351839fb6f9"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_default(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.count=UINT32_C(1);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->count,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->count,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for fanuc_msgs/srv/ReadError_Response.error_data; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_error_data_text;
/** Complete ROS2 fanuc_msgs/srv/ReadError_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field count; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t count;
  /** Declared field error_data; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_error_data_text error_data[5];
} roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_READERROR_RESPONSE_MAX_WIRE_BYTES 2585u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_READERROR_RESPONSE_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_READERROR_RESPONSE_CODEC_IDENTITY "UWVMROS1_6e28db10c7afb06cb1256f6d74a43e26c5efa6c13fec8826ae0e623a5eb954b6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<5u;++i){
  if(value->error_data[i].size>512u||!uwvm_bounded_utf8(value->error_data[i].data,value->error_data[i].size))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  {size_t i;for(i=0;i<5u;++i){
  n+=4+value->error_data[i].size;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->count,1);*offset+=1;
  {size_t i;for(i=0;i<5u;++i){
  uwvm_bounded_put(p+*offset,value->error_data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_data[i].data,value->error_data[i].size);*offset+=value->error_data[i].size;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->count,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<5u;++i){
  if(*offset>n||n-*offset<4)return 0;value->error_data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_data[i].size>512u||value->error_data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->error_data[i].data,p+*offset,value->error_data[i].size);*offset+=value->error_data[i].size;
  }}
  return roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>2585u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_ReadError_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/Reset_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_RESET_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_RESET_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_RESET_REQUEST_CODEC_IDENTITY "UWVMROS1_b73f661d569f6af8dad1cd6625f4c0c55ef2c14c79b3e9af0f17fdc02736abc4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request* value) {
  return roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_Reset_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/Reset_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_RESET_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_RESET_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_RESET_RESPONSE_CODEC_IDENTITY "UWVMROS1_5e9b92e5a112b4934ce0893afaa09156fdb47c26b0aa6ae9a27d61920d308ab8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_Reset_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetAnalogIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETANALOGIO_REQUEST_MAX_WIRE_BYTES 74u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETANALOGIO_REQUEST_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETANALOGIO_REQUEST_CODEC_IDENTITY "UWVMROS1_a6bb4acfae23db79464faff849f359657537026d7c79f8a6c60259caec5e87f4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>74u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetAnalogIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETANALOGIO_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETANALOGIO_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETANALOGIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_aeac383e2c4a292f1237494bff5b261cd0197542e00456b4ee0edf9b7fe8a39b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetAnalogIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetBoolIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETBOOLIO_REQUEST_MAX_WIRE_BYTES 71u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETBOOLIO_REQUEST_MIN_WIRE_BYTES 7u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETBOOLIO_REQUEST_CODEC_IDENTITY "UWVMROS1_0e8ace58c769670a2519e733d020a3ca902bff99982514d5cd9ecda391c1a4cf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  if(value->value>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<7u||length>71u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetBoolIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETBOOLIO_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETBOOLIO_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETBOOLIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_c9c68ca577dd6e4d1743385d9c69c8f1c6e0780ae247877d823ab97a68e675a0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetBoolIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetGenOverride_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  int8_t value;
} roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGENOVERRIDE_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGENOVERRIDE_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGENOVERRIDE_REQUEST_CODEC_IDENTITY "UWVMROS1_7d321e33aaf6f58013c33ffc012c1ddbd27aefff3f21d7456030a1d590362039"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetGenOverride_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGENOVERRIDE_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGENOVERRIDE_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGENOVERRIDE_RESPONSE_CODEC_IDENTITY "UWVMROS1_1ffcb25cad7dbf51c216753dac2c117a191fb081a5a562e63d34d447e61f0a59"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGenOverride_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetGroupIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request {
  /** Declared field io_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_IOType io_type;
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t value;
} roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGROUPIO_REQUEST_MAX_WIRE_BYTES 72u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGROUPIO_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGROUPIO_REQUEST_CODEC_IDENTITY "UWVMROS1_42a76aec283e26e3a2c99bb27a23778e34effb6c3328754793f783038a2b84bd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_valid(&value->io_type))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_IOType_wire_size(&value->io_type);
  n+=2;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_fanuc_msgs_msg_IOType_write_fields(&value->io_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request* value) {
  if(!roswasmc_bounded_2_fanuc_msgs_msg_IOType_read_fields(p,n,offset,&value->io_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->value,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>72u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetGroupIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGROUPIO_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGROUPIO_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETGROUPIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_aa6d2c99e8817273f8f2ab179df61d60efd8ee2cee0e1ec76c647956e22bff17"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetGroupIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetNumReg_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request {
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  float value;
} roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETNUMREG_REQUEST_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETNUMREG_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETNUMREG_REQUEST_CODEC_IDENTITY "UWVMROS1_7e362b671b8770db33f3876308c41a930863a0805771fa05fcbb170782207e36"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request const* value) {size_t n=0;
  n+=2;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetNumReg_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETNUMREG_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETNUMREG_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETNUMREG_RESPONSE_CODEC_IDENTITY "UWVMROS1_7c7850ad8bb5c861d9b436311a8cf985e2e029d362877be80c5a6cb19021e482"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetNumReg_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadComp_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request {
  /** Declared field payload_schedule_id; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t payload_schedule_id;
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field cg_x; canonical capacity is part of CODEC_IDENTITY. */
  float cg_x;
  /** Declared field cg_y; canonical capacity is part of CODEC_IDENTITY. */
  float cg_y;
  /** Declared field cg_z; canonical capacity is part of CODEC_IDENTITY. */
  float cg_z;
  /** Declared field in_x; canonical capacity is part of CODEC_IDENTITY. */
  float in_x;
  /** Declared field in_y; canonical capacity is part of CODEC_IDENTITY. */
  float in_y;
  /** Declared field in_z; canonical capacity is part of CODEC_IDENTITY. */
  float in_z;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADCOMP_REQUEST_MAX_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADCOMP_REQUEST_MIN_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADCOMP_REQUEST_CODEC_IDENTITY "UWVMROS1_92c495d71657249e15c172e850aa91869929a25817437567778df1b01d9037a8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request const* value) {size_t n=0;
  n+=2;
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
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->payload_schedule_id,2);*offset+=2;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->payload_schedule_id,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_z,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<30u||length>30u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadComp_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADCOMP_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADCOMP_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADCOMP_RESPONSE_CODEC_IDENTITY "UWVMROS1_37a9876e2a9c514fa8c4337ff1351f778d350318bdc6d8aa5d569b4039a2f76b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadComp_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadID_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request {
  /** Declared field payload_schedule_id; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t payload_schedule_id;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADID_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADID_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADID_REQUEST_CODEC_IDENTITY "UWVMROS1_aa15b59db17d2afb7ef9e74676a9b744763954ae063efafb91a3ab3c9852a1c0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->payload_schedule_id,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->payload_schedule_id,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadID_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADID_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADID_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADID_RESPONSE_CODEC_IDENTITY "UWVMROS1_7b704b36da10dc71b4cfaad92ea02ed22e4966077dc0940b03b8b8e49c5769c7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadID_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadValue_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request {
  /** Declared field payload_schedule_id; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t payload_schedule_id;
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field cg_x; canonical capacity is part of CODEC_IDENTITY. */
  float cg_x;
  /** Declared field cg_y; canonical capacity is part of CODEC_IDENTITY. */
  float cg_y;
  /** Declared field cg_z; canonical capacity is part of CODEC_IDENTITY. */
  float cg_z;
  /** Declared field use_in; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_in;
  /** Declared field in_x; canonical capacity is part of CODEC_IDENTITY. */
  float in_x;
  /** Declared field in_y; canonical capacity is part of CODEC_IDENTITY. */
  float in_y;
  /** Declared field in_z; canonical capacity is part of CODEC_IDENTITY. */
  float in_z;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADVALUE_REQUEST_MAX_WIRE_BYTES 31u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADVALUE_REQUEST_MIN_WIRE_BYTES 31u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADVALUE_REQUEST_CODEC_IDENTITY "UWVMROS1_5d56ce8f2f8df9c3450d6a5dc77e9e77861471805aa6b9c6f8815c2e7c324974"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->use_in>1)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request const* value) {size_t n=0;
  n+=2;
  n+=4;
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
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->payload_schedule_id,2);*offset+=2;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cg_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_in,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->in_z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->payload_schedule_id,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cg_z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_in,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->in_z,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<31u||length>31u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetPayloadValue_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADVALUE_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADVALUE_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPAYLOADVALUE_RESPONSE_CODEC_IDENTITY "UWVMROS1_09f30071b49ac57353429e8a2862ee80d3dcab9237e8192d008c12a3628cc6ab"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPayloadValue_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for fanuc_msgs/srv/SetPosReg_Request.representation; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_representation_text;
/** Complete ROS2 fanuc_msgs/srv/SetPosReg_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request {
  /** Declared field index; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t index;
  /** Declared field representation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_representation_text representation;
  /** Declared field utool; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool;
  /** Declared field uframe; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe;
  /** Declared field front; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t front;
  /** Declared field up; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t up;
  /** Declared field left; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t left;
  /** Declared field flip; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flip;
  /** Declared field turn4; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn4;
  /** Declared field turn5; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn5;
  /** Declared field turn6; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn6;
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field w; canonical capacity is part of CODEC_IDENTITY. */
  float w;
  /** Declared field p; canonical capacity is part of CODEC_IDENTITY. */
  float p;
  /** Declared field r; canonical capacity is part of CODEC_IDENTITY. */
  float r;
  /** Declared field ext1; canonical capacity is part of CODEC_IDENTITY. */
  float ext1;
  /** Declared field ext2; canonical capacity is part of CODEC_IDENTITY. */
  float ext2;
  /** Declared field ext3; canonical capacity is part of CODEC_IDENTITY. */
  float ext3;
  /** Declared field j1; canonical capacity is part of CODEC_IDENTITY. */
  float j1;
  /** Declared field j2; canonical capacity is part of CODEC_IDENTITY. */
  float j2;
  /** Declared field j3; canonical capacity is part of CODEC_IDENTITY. */
  float j3;
  /** Declared field j4; canonical capacity is part of CODEC_IDENTITY. */
  float j4;
  /** Declared field j5; canonical capacity is part of CODEC_IDENTITY. */
  float j5;
  /** Declared field j6; canonical capacity is part of CODEC_IDENTITY. */
  float j6;
  /** Declared field j7; canonical capacity is part of CODEC_IDENTITY. */
  float j7;
  /** Declared field j8; canonical capacity is part of CODEC_IDENTITY. */
  float j8;
  /** Declared field j9; canonical capacity is part of CODEC_IDENTITY. */
  float j9;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPOSREG_REQUEST_MAX_WIRE_BYTES 599u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPOSREG_REQUEST_MIN_WIRE_BYTES 87u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPOSREG_REQUEST_CODEC_IDENTITY "UWVMROS1_f9af742e55a044a1a363a45cbe4a0a8ce4ca62fcaf43759041cef89149d96d30"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request const* value) {
  if(!value)return 0;
  if(value->representation.size>512u||!uwvm_bounded_utf8(value->representation.data,value->representation.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j4,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j5,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j6,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j7,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j8,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j9,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request const* value) {size_t n=0;
  n+=2;
  n+=4+value->representation.size;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
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
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->index,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->representation.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->representation.data,value->representation.size);*offset+=value->representation.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->front,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->up,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->left,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->flip,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn4,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn5,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn6,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j4,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j5,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j6,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j7,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j8,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->j9,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->index,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->representation.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->representation.size>512u||value->representation.size>n-*offset)return 0;uwvm_bounded_copy(value->representation.data,p+*offset,value->representation.size);*offset+=value->representation.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->front,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->up,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->left,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flip,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn4,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn5,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn6,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->w,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->p,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->r,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j3,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j4,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j5,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j6,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j7,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j8,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->j9,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<87u||length>599u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetPosReg_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPOSREG_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPOSREG_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETPOSREG_RESPONSE_CODEC_IDENTITY "UWVMROS1_fb6f889d30c921df31f55cdc760346eb0d655816e71b52d4aac7a1e56d4e9cb2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetPosReg_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameData_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request {
  /** Declared field uframe_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe_number;
  /** Declared field frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_Frame frame;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEDATA_REQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEDATA_REQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEDATA_REQUEST_CODEC_IDENTITY "UWVMROS1_a067808d8fe5ca1418e18ac492313ad9211cc8f3bf402ad0ac51a2850a2bc169"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(&value->frame))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request const* value) {size_t n=0;
  n+=1;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_Frame_wire_size(&value->frame);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe_number,1);*offset+=1;
  roswasmc_bounded_2_fanuc_msgs_msg_Frame_write_fields(&value->frame,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe_number,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_read_fields(p,n,offset,&value->frame))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameData_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEDATA_RESPONSE_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEDATA_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEDATA_RESPONSE_CODEC_IDENTITY "UWVMROS1_f436e4fe5bd06b048f4f53d2df4933ac8eac5a784f31b42ccf13f5eaf1696eee"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameData_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameUTool_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request {
  /** Declared field uframe_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t uframe_number;
  /** Declared field utool_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool_number;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEUTOOL_REQUEST_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEUTOOL_REQUEST_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEUTOOL_REQUEST_CODEC_IDENTITY "UWVMROS1_0b86b0a37e4dc23e0ff651df867f5958e7440bf4712b47779d69ca699af4c09f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetUFrameUTool_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEUTOOL_RESPONSE_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEUTOOL_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUFRAMEUTOOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_b77837445c9663f29f23465cc7178a183d245486f74bcff732fc2f4025b82a67"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUFrameUTool_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetUToolData_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request {
  /** Declared field utool_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t utool_number;
  /** Declared field frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_fanuc_msgs_msg_Frame frame;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUTOOLDATA_REQUEST_MAX_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUTOOLDATA_REQUEST_MIN_WIRE_BYTES 26u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUTOOLDATA_REQUEST_CODEC_IDENTITY "UWVMROS1_7d5b911133645744c8def79bc16c8b7c4ee4e87a057ea33741031d6a94e620ab"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_valid(&value->frame))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request const* value) {size_t n=0;
  n+=1;
  n+=roswasmc_bounded_2_fanuc_msgs_msg_Frame_wire_size(&value->frame);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool_number,1);*offset+=1;
  roswasmc_bounded_2_fanuc_msgs_msg_Frame_write_fields(&value->frame,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool_number,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_fanuc_msgs_msg_Frame_read_fields(p,n,offset,&value->frame))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<26u||length>26u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SetUToolData_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field group; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group;
} roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUTOOLDATA_RESPONSE_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUTOOLDATA_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SETUTOOLDATA_RESPONSE_CODEC_IDENTITY "UWVMROS1_b33d265f275dfcb5fe20432d5e38b55574c85260135bded477c2d5fa2c85c7cb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SetUToolData_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SwitchControlState_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t status;
} roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_REQUEST_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_REQUEST_CODEC_IDENTITY "UWVMROS1_b485b7071fe2737c7dc8f0360a15b7be30f997430142d8260454fd423172cc21"
/** Original scalar constant STOP=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_REQUEST_STOP UINT32_C(0)
/** Original scalar constant START=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_REQUEST_START UINT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_valid(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->status,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_encode(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 fanuc_msgs/srv/SwitchControlState_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response;
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_FANUC_MSGS_SRV_SWITCHCONTROLSTATE_RESPONSE_CODEC_IDENTITY "UWVMROS1_8a3d7a3c1037174f4118843e34222ef8811eff3cc885a0c688d98071e1a9496c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_valid(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_wire_size(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_write_fields(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_encode(roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_decode(void const* input,size_t length,roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response* value) {
  roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_fanuc_msgs_srv_SwitchControlState_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/CallParam.type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_rmi_msgs_msg_CallParam_type_text;
/** UTF-8 byte storage for rmi_msgs/msg/CallParam.string_value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_CallParam_string_value_text;
/** Complete ROS2 rmi_msgs/msg/CallParam value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_CallParam {
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_CallParam_type_text type;
  /** Declared field int_value; canonical capacity is part of CODEC_IDENTITY. */
  int32_t int_value;
  /** Declared field float_value; canonical capacity is part of CODEC_IDENTITY. */
  float float_value;
  /** Declared field string_value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_CallParam_string_value_text string_value;
} roswasmc_bounded_2_rmi_msgs_msg_CallParam;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_MAX_WIRE_BYTES 592u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_CODEC_IDENTITY "UWVMROS1_ed625d80db6c4314b8373f2d205344d6ab8d71be46182dab5a497d36564a2255"
/** Original scalar constant AR="AR"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_AR "\101\122"
/** Original scalar constant PR="PR"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_PR "\120\122"
/** Original scalar constant SR="SR"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_SR "\123\122"
/** Original scalar constant R="R"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_R "\122"
/** Original scalar constant P="P"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_P "\120"
/** Original scalar constant INT="INT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_INT "\111\116\124"
/** Original scalar constant INTEGER="INT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_INTEGER "\111\116\124"
/** Original scalar constant FLOAT="FLOAT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_FLOAT "\106\114\117\101\124"
/** Original scalar constant STRING="String"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_STRING "\123\164\162\151\156\147"
/** Original scalar constant STR="String"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CALLPARAM_STR "\123\164\162\151\156\147"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_CallParam_valid(roswasmc_bounded_2_rmi_msgs_msg_CallParam const* value) {
  if(!value)return 0;
  if(value->type.size>64u||!uwvm_bounded_utf8(value->type.data,value->type.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->float_value,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->string_value.size>512u||!uwvm_bounded_utf8(value->string_value.data,value->string_value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_CallParam_wire_size(roswasmc_bounded_2_rmi_msgs_msg_CallParam const* value) {size_t n=0;
  n+=4+value->type.size;
  n+=4;
  n+=4;
  n+=4+value->string_value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_CallParam_write_fields(roswasmc_bounded_2_rmi_msgs_msg_CallParam const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->type.data,value->type.size);*offset+=value->type.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->int_value,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->float_value,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,value->string_value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->string_value.data,value->string_value.size);*offset+=value->string_value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_CallParam_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_CallParam* value) {
  if(*offset>n||n-*offset<4)return 0;value->type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->type.size>64u||value->type.size>n-*offset)return 0;uwvm_bounded_copy(value->type.data,p+*offset,value->type.size);*offset+=value->type.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->int_value,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->float_value,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->string_value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->string_value.size>512u||value->string_value.size>n-*offset)return 0;uwvm_bounded_copy(value->string_value.data,p+*offset,value->string_value.size);*offset+=value->string_value.size;
  return roswasmc_bounded_2_rmi_msgs_msg_CallParam_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_CallParam_encode(roswasmc_bounded_2_rmi_msgs_msg_CallParam const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_CallParam_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_CallParam_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_CallParam_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_CallParam_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_CallParam* value) {
  roswasmc_bounded_2_rmi_msgs_msg_CallParam next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>592u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_CallParam_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/msg/Cartesian value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_Cartesian {
  /** Declared field utool; canonical capacity is part of CODEC_IDENTITY. */
  int8_t utool;
  /** Declared field uframe; canonical capacity is part of CODEC_IDENTITY. */
  int8_t uframe;
  /** Declared field front; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t front;
  /** Declared field up; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t up;
  /** Declared field left; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t left;
  /** Declared field flip; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t flip;
  /** Declared field turn4; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn4;
  /** Declared field turn5; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn5;
  /** Declared field turn6; canonical capacity is part of CODEC_IDENTITY. */
  int8_t turn6;
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
  /** Declared field w; canonical capacity is part of CODEC_IDENTITY. */
  float w;
  /** Declared field p; canonical capacity is part of CODEC_IDENTITY. */
  float p;
  /** Declared field r; canonical capacity is part of CODEC_IDENTITY. */
  float r;
  /** Declared field ext1; canonical capacity is part of CODEC_IDENTITY. */
  float ext1;
  /** Declared field ext2; canonical capacity is part of CODEC_IDENTITY. */
  float ext2;
  /** Declared field ext3; canonical capacity is part of CODEC_IDENTITY. */
  float ext3;
} roswasmc_bounded_2_rmi_msgs_msg_Cartesian;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CARTESIAN_MAX_WIRE_BYTES 45u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CARTESIAN_MIN_WIRE_BYTES 45u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_CARTESIAN_CODEC_IDENTITY "UWVMROS1_2a0e1fff119007499a63d55c9722f666be1436a9212d9e43b556af347e8e4b40"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Cartesian_default(roswasmc_bounded_2_rmi_msgs_msg_Cartesian* value) {
  roswasmc_bounded_2_rmi_msgs_msg_Cartesian next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.utool=INT32_C(1);
  next.uframe=INT32_C(0);
  next.front=UINT32_C(1);
  next.up=UINT32_C(1);
  next.left=UINT32_C(0);
  next.flip=UINT32_C(0);
  next.turn4=INT32_C(0);
  next.turn5=INT32_C(0);
  next.turn6=INT32_C(0);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Cartesian_valid(roswasmc_bounded_2_rmi_msgs_msg_Cartesian const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_Cartesian_wire_size(roswasmc_bounded_2_rmi_msgs_msg_Cartesian const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
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
static inline void roswasmc_bounded_2_rmi_msgs_msg_Cartesian_write_fields(roswasmc_bounded_2_rmi_msgs_msg_Cartesian const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->utool,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->uframe,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->front,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->up,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->left,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->flip,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn4,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn5,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->turn6,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->w,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->p,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->r,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext1,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext2,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ext3,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Cartesian_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_Cartesian* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->utool,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->uframe,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->front,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->up,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->left,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->flip,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn4,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn5,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->turn6,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->w,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->p,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->r,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext1,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext2,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ext3,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_msg_Cartesian_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Cartesian_encode(roswasmc_bounded_2_rmi_msgs_msg_Cartesian const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_Cartesian_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_Cartesian_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_Cartesian_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Cartesian_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_Cartesian* value) {
  roswasmc_bounded_2_rmi_msgs_msg_Cartesian next={0};size_t offset=0;if(!value||(length&&!input)||length<45u||length>45u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Cartesian_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/Command.type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_rmi_msgs_msg_Command_type_text;
/** Complete ROS2 rmi_msgs/msg/Command value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_Command {
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Command_type_text type;
} roswasmc_bounded_2_rmi_msgs_msg_Command;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_CODEC_IDENTITY "UWVMROS1_d2741856b8cdf57aa716566edfc705957611557b7da5b6099b173903bdd9960c"
/** Original scalar constant INITIAZLIE="INITIALIZE"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_INITIAZLIE "\111\116\111\124\111\101\114\111\132\105"
/** Original scalar constant INIT="INITIALIZE"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_INIT "\111\116\111\124\111\101\114\111\132\105"
/** Original scalar constant ABORT="ABORT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_ABORT "\101\102\117\122\124"
/** Original scalar constant PAUSE="PAUSE"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_PAUSE "\120\101\125\123\105"
/** Original scalar constant RESUME="RESUME"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMAND_RESUME "\122\105\123\125\115\105"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Command_valid(roswasmc_bounded_2_rmi_msgs_msg_Command const* value) {
  if(!value)return 0;
  if(value->type.size>64u||!uwvm_bounded_utf8(value->type.data,value->type.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_Command_wire_size(roswasmc_bounded_2_rmi_msgs_msg_Command const* value) {size_t n=0;
  n+=4+value->type.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_Command_write_fields(roswasmc_bounded_2_rmi_msgs_msg_Command const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->type.data,value->type.size);*offset+=value->type.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Command_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_Command* value) {
  if(*offset>n||n-*offset<4)return 0;value->type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->type.size>64u||value->type.size>n-*offset)return 0;uwvm_bounded_copy(value->type.data,p+*offset,value->type.size);*offset+=value->type.size;
  return roswasmc_bounded_2_rmi_msgs_msg_Command_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Command_encode(roswasmc_bounded_2_rmi_msgs_msg_Command const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_Command_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_Command_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_Command_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Command_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_Command* value) {
  roswasmc_bounded_2_rmi_msgs_msg_Command next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Command_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/CommandOptions.rtsa; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_rtsa_text;
/** UTF-8 byte storage for rmi_msgs/msg/CommandOptions.pltzmode; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_pltzmode_text;
/** Complete ROS2 rmi_msgs/msg/CommandOptions value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_CommandOptions {
  /** Declared field use_group_mask; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_group_mask;
  /** Declared field group_mask; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t group_mask;
  /** Declared field use_rtsa; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_rtsa;
  /** Declared field rtsa; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_rtsa_text rtsa;
  /** Declared field use_pltzmode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_pltzmode;
  /** Declared field pltzmode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_pltzmode_text pltzmode;
} roswasmc_bounded_2_rmi_msgs_msg_CommandOptions;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_MAX_WIRE_BYTES 1036u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_CODEC_IDENTITY "UWVMROS1_0be0c508035725bf9686807cb8b5267e23f709b7aa34b0910a44ff3604670147"
/** Original scalar constant RTSA_ON="ON"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_RTSA_ON "\117\116"
/** Original scalar constant RTSA_OFF="OFF"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_RTSA_OFF "\117\106\106"
/** Original scalar constant PLTZ_ZERODN="ZERODN"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_PLTZ_ZERODN "\132\105\122\117\104\116"
/** Original scalar constant PLTZ_ZEROUP="ZEROUP"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_PLTZ_ZEROUP "\132\105\122\117\125\120"
/** Original scalar constant PLTZ_PSPIDN="PSPIDN"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_PLTZ_PSPIDN "\120\123\120\111\104\116"
/** Original scalar constant PLTZ_PSPIUP="PSPIUP"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_PLTZ_PSPIUP "\120\123\120\111\125\120"
/** Original scalar constant PLTZ_MSPIDN="MSPIDN"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_PLTZ_MSPIDN "\115\123\120\111\104\116"
/** Original scalar constant PLTZ_MSPIUP="MSPIUP"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_COMMANDOPTIONS_PLTZ_MSPIUP "\115\123\120\111\125\120"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_default(roswasmc_bounded_2_rmi_msgs_msg_CommandOptions* value) {
  roswasmc_bounded_2_rmi_msgs_msg_CommandOptions next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.use_group_mask=0;
  next.group_mask=UINT32_C(1);
  next.use_rtsa=0;
  next.rtsa.size=2u;uwvm_bounded_copy(next.rtsa.data,"\117\116",2u);
  next.use_pltzmode=0;
  next.pltzmode.size=6u;uwvm_bounded_copy(next.pltzmode.data,"\132\105\122\117\104\116",6u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_valid(roswasmc_bounded_2_rmi_msgs_msg_CommandOptions const* value) {
  if(!value)return 0;
  if(value->use_group_mask>1)return 0;
  if(value->use_rtsa>1)return 0;
  if(value->rtsa.size>512u||!uwvm_bounded_utf8(value->rtsa.data,value->rtsa.size))return 0;
  if(value->use_pltzmode>1)return 0;
  if(value->pltzmode.size>512u||!uwvm_bounded_utf8(value->pltzmode.data,value->pltzmode.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_wire_size(roswasmc_bounded_2_rmi_msgs_msg_CommandOptions const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=4+value->rtsa.size;
  n+=1;
  n+=4+value->pltzmode.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_write_fields(roswasmc_bounded_2_rmi_msgs_msg_CommandOptions const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_group_mask,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->group_mask,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_rtsa,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->rtsa.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->rtsa.data,value->rtsa.size);*offset+=value->rtsa.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_pltzmode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->pltzmode.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->pltzmode.data,value->pltzmode.size);*offset+=value->pltzmode.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_CommandOptions* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_group_mask,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->group_mask,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_rtsa,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->rtsa.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->rtsa.size>512u||value->rtsa.size>n-*offset)return 0;uwvm_bounded_copy(value->rtsa.data,p+*offset,value->rtsa.size);*offset+=value->rtsa.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_pltzmode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->pltzmode.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->pltzmode.size>512u||value->pltzmode.size>n-*offset)return 0;uwvm_bounded_copy(value->pltzmode.data,p+*offset,value->pltzmode.size);*offset+=value->pltzmode.size;
  return roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_encode(roswasmc_bounded_2_rmi_msgs_msg_CommandOptions const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_CommandOptions* value) {
  roswasmc_bounded_2_rmi_msgs_msg_CommandOptions next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>1036u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/msg/Joint value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_Joint {
  /** Declared field positions; canonical capacity is part of CODEC_IDENTITY. */
  float positions[9];
} roswasmc_bounded_2_rmi_msgs_msg_Joint;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_JOINT_MAX_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_JOINT_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_JOINT_CODEC_IDENTITY "UWVMROS1_0ba90adf6cc287bc306fd2a77c52c2e4d62731cfac37bb2cc116ee846249a42d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Joint_valid(roswasmc_bounded_2_rmi_msgs_msg_Joint const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->positions[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_Joint_wire_size(roswasmc_bounded_2_rmi_msgs_msg_Joint const* value) {size_t n=0;
  {size_t i;for(i=0;i<9u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_Joint_write_fields(roswasmc_bounded_2_rmi_msgs_msg_Joint const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<9u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->positions[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Joint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_Joint* value) {
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->positions[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_rmi_msgs_msg_Joint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Joint_encode(roswasmc_bounded_2_rmi_msgs_msg_Joint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_Joint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_Joint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_Joint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Joint_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_Joint* value) {
  roswasmc_bounded_2_rmi_msgs_msg_Joint next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>36u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Joint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/MotionInstructionOptions.wrist_joint; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_wrist_joint_text;
/** UTF-8 byte storage for rmi_msgs/msg/MotionInstructionOptions.mrot; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_mrot_text;
/** UTF-8 byte storage for rmi_msgs/msg/MotionInstructionOptions.lcb_type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_lcb_type_text;
/** UTF-8 byte storage for rmi_msgs/msg/MotionInstructionOptions.port_value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_port_value_text;
/** UTF-8 byte storage for rmi_msgs/msg/MotionInstructionOptions.no_blend; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_no_blend_text;
/** Complete ROS2 rmi_msgs/msg/MotionInstructionOptions value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions {
  /** Declared field use_acc; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_acc;
  /** Declared field acc; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t acc;
  /** Declared field use_offset_pr_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_offset_pr_number;
  /** Declared field offset_pr_number; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t offset_pr_number;
  /** Declared field use_vision_pr_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_vision_pr_number;
  /** Declared field vision_pr_number; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t vision_pr_number;
  /** Declared field use_wrist_joint; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_wrist_joint;
  /** Declared field wrist_joint; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_wrist_joint_text wrist_joint;
  /** Declared field use_mrot; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_mrot;
  /** Declared field mrot; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_mrot_text mrot;
  /** Declared field use_lcb_type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_lcb_type;
  /** Declared field lcb_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_lcb_type_text lcb_type;
  /** Declared field use_lcb_value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_lcb_value;
  /** Declared field lcb_value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t lcb_value;
  /** Declared field use_port_type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_port_type;
  /** Declared field port_type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t port_type;
  /** Declared field use_port_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_port_number;
  /** Declared field port_number; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t port_number;
  /** Declared field use_port_value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_port_value;
  /** Declared field port_value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_port_value_text port_value;
  /** Declared field use_tool_offset_pr_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_tool_offset_pr_number;
  /** Declared field tool_offset_pr_number; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t tool_offset_pr_number;
  /** Declared field use_alim; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_alim;
  /** Declared field alim; canonical capacity is part of CODEC_IDENTITY. */
  int32_t alim;
  /** Declared field use_alim_reg; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_alim_reg;
  /** Declared field alim_reg; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t alim_reg;
  /** Declared field use_no_blend; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t use_no_blend;
  /** Declared field no_blend; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_no_blend_text no_blend;
} roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_MAX_WIRE_BYTES 2612u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_MIN_WIRE_BYTES 52u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_CODEC_IDENTITY "UWVMROS1_f8d9ed2e083c577b0cae8965df71aa99643d12ce5a5a6ce558894edfb756bcc6"
/** Original scalar constant LCB_TYPE_TB="TB"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_LCB_TYPE_TB "\124\102"
/** Original scalar constant LCB_TYPE_TA="TA"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_LCB_TYPE_TA "\124\101"
/** Original scalar constant LCB_TYPE_DB="DB"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_LCB_TYPE_DB "\104\102"
/** Original scalar constant PORT_TYPE_DOUT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_PORT_TYPE_DOUT UINT32_C(1)
/** Original scalar constant PORT_TYPE_DO=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_PORT_TYPE_DO UINT32_C(1)
/** Original scalar constant PORT_TYPE_ROUT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_PORT_TYPE_ROUT UINT32_C(2)
/** Original scalar constant PORT_TYPE_RO=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONINSTRUCTIONOPTIONS_PORT_TYPE_RO UINT32_C(2)
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_default(roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions* value) {
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.use_acc=0;
  next.use_offset_pr_number=0;
  next.use_vision_pr_number=0;
  next.use_wrist_joint=0;
  next.wrist_joint.size=2u;uwvm_bounded_copy(next.wrist_joint.data,"\117\116",2u);
  next.use_mrot=0;
  next.mrot.size=2u;uwvm_bounded_copy(next.mrot.data,"\117\116",2u);
  next.use_lcb_type=0;
  next.use_lcb_value=0;
  next.use_port_type=0;
  next.use_port_number=0;
  next.use_port_value=0;
  next.use_tool_offset_pr_number=0;
  next.use_alim=0;
  next.use_alim_reg=0;
  next.use_no_blend=0;
  next.no_blend.size=2u;uwvm_bounded_copy(next.no_blend.data,"\117\116",2u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_valid(roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions const* value) {
  if(!value)return 0;
  if(value->use_acc>1)return 0;
  if(value->use_offset_pr_number>1)return 0;
  if(value->use_vision_pr_number>1)return 0;
  if(value->use_wrist_joint>1)return 0;
  if(value->wrist_joint.size>512u||!uwvm_bounded_utf8(value->wrist_joint.data,value->wrist_joint.size))return 0;
  if(value->use_mrot>1)return 0;
  if(value->mrot.size>512u||!uwvm_bounded_utf8(value->mrot.data,value->mrot.size))return 0;
  if(value->use_lcb_type>1)return 0;
  if(value->lcb_type.size>512u||!uwvm_bounded_utf8(value->lcb_type.data,value->lcb_type.size))return 0;
  if(value->use_lcb_value>1)return 0;
  if(value->use_port_type>1)return 0;
  if(value->use_port_number>1)return 0;
  if(value->use_port_value>1)return 0;
  if(value->port_value.size>512u||!uwvm_bounded_utf8(value->port_value.data,value->port_value.size))return 0;
  if(value->use_tool_offset_pr_number>1)return 0;
  if(value->use_alim>1)return 0;
  if(value->use_alim_reg>1)return 0;
  if(value->use_no_blend>1)return 0;
  if(value->no_blend.size>512u||!uwvm_bounded_utf8(value->no_blend.data,value->no_blend.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_wire_size(roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=2;
  n+=1;
  n+=2;
  n+=1;
  n+=4+value->wrist_joint.size;
  n+=1;
  n+=4+value->mrot.size;
  n+=1;
  n+=4+value->lcb_type.size;
  n+=1;
  n+=2;
  n+=1;
  n+=1;
  n+=1;
  n+=2;
  n+=1;
  n+=4+value->port_value.size;
  n+=1;
  n+=2;
  n+=1;
  n+=4;
  n+=1;
  n+=2;
  n+=1;
  n+=4+value->no_blend.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_write_fields(roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_acc,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->acc,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_offset_pr_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->offset_pr_number,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_vision_pr_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->vision_pr_number,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_wrist_joint,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->wrist_joint.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->wrist_joint.data,value->wrist_joint.size);*offset+=value->wrist_joint.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_mrot,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->mrot.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->mrot.data,value->mrot.size);*offset+=value->mrot.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_lcb_type,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->lcb_type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->lcb_type.data,value->lcb_type.size);*offset+=value->lcb_type.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_lcb_value,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->lcb_value,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_port_type,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->port_type,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_port_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->port_number,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_port_value,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->port_value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->port_value.data,value->port_value.size);*offset+=value->port_value.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_tool_offset_pr_number,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->tool_offset_pr_number,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_alim,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->alim,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_alim_reg,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->alim_reg,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->use_no_blend,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->no_blend.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->no_blend.data,value->no_blend.size);*offset+=value->no_blend.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_acc,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->acc,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_offset_pr_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->offset_pr_number,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_vision_pr_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->vision_pr_number,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_wrist_joint,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->wrist_joint.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->wrist_joint.size>512u||value->wrist_joint.size>n-*offset)return 0;uwvm_bounded_copy(value->wrist_joint.data,p+*offset,value->wrist_joint.size);*offset+=value->wrist_joint.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_mrot,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->mrot.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->mrot.size>512u||value->mrot.size>n-*offset)return 0;uwvm_bounded_copy(value->mrot.data,p+*offset,value->mrot.size);*offset+=value->mrot.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_lcb_type,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->lcb_type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->lcb_type.size>512u||value->lcb_type.size>n-*offset)return 0;uwvm_bounded_copy(value->lcb_type.data,p+*offset,value->lcb_type.size);*offset+=value->lcb_type.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_lcb_value,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->lcb_value,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_port_type,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->port_type,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_port_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->port_number,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_port_value,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->port_value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->port_value.size>512u||value->port_value.size>n-*offset)return 0;uwvm_bounded_copy(value->port_value.data,p+*offset,value->port_value.size);*offset+=value->port_value.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_tool_offset_pr_number,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->tool_offset_pr_number,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_alim,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->alim,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_alim_reg,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->alim_reg,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->use_no_blend,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->no_blend.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->no_blend.size>512u||value->no_blend.size>n-*offset)return 0;uwvm_bounded_copy(value->no_blend.data,p+*offset,value->no_blend.size);*offset+=value->no_blend.size;
  return roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_encode(roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions* value) {
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions next={0};size_t offset=0;if(!value||(length&&!input)||length<52u||length>2612u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/MotionType.type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_rmi_msgs_msg_MotionType_type_text;
/** Complete ROS2 rmi_msgs/msg/MotionType value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_MotionType {
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionType_type_text type;
} roswasmc_bounded_2_rmi_msgs_msg_MotionType;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_CODEC_IDENTITY "UWVMROS1_c51cc7a32f6853d13bedaf3306dd79e445de6c0d16ae0ec39034ce0ae6d19c8a"
/** Original scalar constant J="J"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_J "\112"
/** Original scalar constant JOINT="J"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_JOINT "\112"
/** Original scalar constant L="L"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_L "\114"
/** Original scalar constant LINEAR="L"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_LINEAR "\114"
/** Original scalar constant C="C"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_C "\103"
/** Original scalar constant CIRCULAR="C"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_CIRCULAR "\103"
/** Original scalar constant S="S"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_S "\123"
/** Original scalar constant SPLINE="S"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_MOTIONTYPE_SPLINE "\123"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_MotionType_default(roswasmc_bounded_2_rmi_msgs_msg_MotionType* value) {
  roswasmc_bounded_2_rmi_msgs_msg_MotionType next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.type.size=1u;uwvm_bounded_copy(next.type.data,"\112",1u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_MotionType_valid(roswasmc_bounded_2_rmi_msgs_msg_MotionType const* value) {
  if(!value)return 0;
  if(value->type.size>64u||!uwvm_bounded_utf8(value->type.data,value->type.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_MotionType_wire_size(roswasmc_bounded_2_rmi_msgs_msg_MotionType const* value) {size_t n=0;
  n+=4+value->type.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_MotionType_write_fields(roswasmc_bounded_2_rmi_msgs_msg_MotionType const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->type.data,value->type.size);*offset+=value->type.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_MotionType_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_MotionType* value) {
  if(*offset>n||n-*offset<4)return 0;value->type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->type.size>64u||value->type.size>n-*offset)return 0;uwvm_bounded_copy(value->type.data,p+*offset,value->type.size);*offset+=value->type.size;
  return roswasmc_bounded_2_rmi_msgs_msg_MotionType_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_MotionType_encode(roswasmc_bounded_2_rmi_msgs_msg_MotionType const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_MotionType_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_MotionType_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_MotionType_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_MotionType_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_MotionType* value) {
  roswasmc_bounded_2_rmi_msgs_msg_MotionType next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_MotionType_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/Representation.representation; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_msg_Representation_representation_text;
/** Complete ROS2 rmi_msgs/msg/Representation value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_Representation {
  /** Declared field representation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Representation_representation_text representation;
} roswasmc_bounded_2_rmi_msgs_msg_Representation;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_MAX_WIRE_BYTES 516u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_CODEC_IDENTITY "UWVMROS1_b439506f2dc2135bb484b3abb308dfdf5c07367af5a4d6d0f5435ad9bba8ea6f"
/** Original scalar constant JOINT="JOINT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_JOINT "\112\117\111\116\124"
/** Original scalar constant J="JOINT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_J "\112\117\111\116\124"
/** Original scalar constant CARTESIAN="CARTESIAN"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_CARTESIAN "\103\101\122\124\105\123\111\101\116"
/** Original scalar constant CART="CARTESIAN"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_CART "\103\101\122\124\105\123\111\101\116"
/** Original scalar constant C="CARTESIAN"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_REPRESENTATION_C "\103\101\122\124\105\123\111\101\116"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Representation_default(roswasmc_bounded_2_rmi_msgs_msg_Representation* value) {
  roswasmc_bounded_2_rmi_msgs_msg_Representation next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.representation.size=5u;uwvm_bounded_copy(next.representation.data,"\112\117\111\116\124",5u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Representation_valid(roswasmc_bounded_2_rmi_msgs_msg_Representation const* value) {
  if(!value)return 0;
  if(value->representation.size>512u||!uwvm_bounded_utf8(value->representation.data,value->representation.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_Representation_wire_size(roswasmc_bounded_2_rmi_msgs_msg_Representation const* value) {size_t n=0;
  n+=4+value->representation.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_Representation_write_fields(roswasmc_bounded_2_rmi_msgs_msg_Representation const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->representation.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->representation.data,value->representation.size);*offset+=value->representation.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_Representation_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_Representation* value) {
  if(*offset>n||n-*offset<4)return 0;value->representation.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->representation.size>512u||value->representation.size>n-*offset)return 0;uwvm_bounded_copy(value->representation.data,p+*offset,value->representation.size);*offset+=value->representation.size;
  return roswasmc_bounded_2_rmi_msgs_msg_Representation_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Representation_encode(roswasmc_bounded_2_rmi_msgs_msg_Representation const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_Representation_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_Representation_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_Representation_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_Representation_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_Representation* value) {
  roswasmc_bounded_2_rmi_msgs_msg_Representation next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>516u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Representation_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/SpeedType.type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_rmi_msgs_msg_SpeedType_type_text;
/** Complete ROS2 rmi_msgs/msg/SpeedType value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_SpeedType {
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_SpeedType_type_text type;
} roswasmc_bounded_2_rmi_msgs_msg_SpeedType;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_CODEC_IDENTITY "UWVMROS1_e4f4dbda84d8d8cd4d026b0beb52005663c7c4ee602c8d1e4d5853589777c282"
/** Original scalar constant PERCENT="Percent"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_PERCENT "\120\145\162\143\145\156\164"
/** Original scalar constant PERCENTAGE="Percent"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_PERCENTAGE "\120\145\162\143\145\156\164"
/** Original scalar constant MMSEC="mmSec"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_MMSEC "\155\155\123\145\143"
/** Original scalar constant INCHMIN="InchMin"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_INCHMIN "\111\156\143\150\115\151\156"
/** Original scalar constant TIME="Time"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_TIME "\124\151\155\145"
/** Original scalar constant SECONDS="Time"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_SECONDS "\124\151\155\145"
/** Original scalar constant SEC="Time"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_SEC "\124\151\155\145"
/** Original scalar constant MSEC="mSec"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_SPEEDTYPE_MSEC "\155\123\145\143"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_SpeedType_default(roswasmc_bounded_2_rmi_msgs_msg_SpeedType* value) {
  roswasmc_bounded_2_rmi_msgs_msg_SpeedType next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.type.size=7u;uwvm_bounded_copy(next.type.data,"\120\145\162\143\145\156\164",7u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_SpeedType_valid(roswasmc_bounded_2_rmi_msgs_msg_SpeedType const* value) {
  if(!value)return 0;
  if(value->type.size>64u||!uwvm_bounded_utf8(value->type.data,value->type.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_SpeedType_wire_size(roswasmc_bounded_2_rmi_msgs_msg_SpeedType const* value) {size_t n=0;
  n+=4+value->type.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_SpeedType_write_fields(roswasmc_bounded_2_rmi_msgs_msg_SpeedType const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->type.data,value->type.size);*offset+=value->type.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_SpeedType_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_SpeedType* value) {
  if(*offset>n||n-*offset<4)return 0;value->type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->type.size>64u||value->type.size>n-*offset)return 0;uwvm_bounded_copy(value->type.data,p+*offset,value->type.size);*offset+=value->type.size;
  return roswasmc_bounded_2_rmi_msgs_msg_SpeedType_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_SpeedType_encode(roswasmc_bounded_2_rmi_msgs_msg_SpeedType const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_SpeedType_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_SpeedType_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_SpeedType_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_SpeedType_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_SpeedType* value) {
  roswasmc_bounded_2_rmi_msgs_msg_SpeedType next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_SpeedType_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/msg/TermType.type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_rmi_msgs_msg_TermType_type_text;
/** Complete ROS2 rmi_msgs/msg/TermType value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_msg_TermType {
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_TermType_type_text type;
} roswasmc_bounded_2_rmi_msgs_msg_TermType;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_TERMTYPE_MAX_WIRE_BYTES 68u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_TERMTYPE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_TERMTYPE_CODEC_IDENTITY "UWVMROS1_5ddbb483126e0ab5799ed865480da457179d83a271581c1471aec940bedc4535"
/** Original scalar constant FINE="FINE"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_TERMTYPE_FINE "\106\111\116\105"
/** Original scalar constant CNT="CNT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_TERMTYPE_CNT "\103\116\124"
/** Original scalar constant CR="CR"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_TERMTYPE_CR "\103\122"
/** Original scalar constant CD="CD"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_RMI_MSGS_MSG_TERMTYPE_CD "\103\104"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_TermType_default(roswasmc_bounded_2_rmi_msgs_msg_TermType* value) {
  roswasmc_bounded_2_rmi_msgs_msg_TermType next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.type.size=4u;uwvm_bounded_copy(next.type.data,"\106\111\116\105",4u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_TermType_valid(roswasmc_bounded_2_rmi_msgs_msg_TermType const* value) {
  if(!value)return 0;
  if(value->type.size>64u||!uwvm_bounded_utf8(value->type.data,value->type.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_msg_TermType_wire_size(roswasmc_bounded_2_rmi_msgs_msg_TermType const* value) {size_t n=0;
  n+=4+value->type.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_msg_TermType_write_fields(roswasmc_bounded_2_rmi_msgs_msg_TermType const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->type.data,value->type.size);*offset+=value->type.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_msg_TermType_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_msg_TermType* value) {
  if(*offset>n||n-*offset<4)return 0;value->type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->type.size>64u||value->type.size>n-*offset)return 0;uwvm_bounded_copy(value->type.data,p+*offset,value->type.size);*offset+=value->type.size;
  return roswasmc_bounded_2_rmi_msgs_msg_TermType_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_TermType_encode(roswasmc_bounded_2_rmi_msgs_msg_TermType const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_msg_TermType_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_msg_TermType_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_msg_TermType_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_msg_TermType_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_msg_TermType* value) {
  roswasmc_bounded_2_rmi_msgs_msg_TermType next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>68u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_msg_TermType_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/srv/AddCall_Request.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_program_name_text;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_rmi_msgs_msg_CallParam data[16];} roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_params_sequence;
/** Complete ROS2 rmi_msgs/srv/AddCall_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request {
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_program_name_text program_name;
  /** Declared field params; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_params_sequence params;
} roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDCALL_REQUEST_MAX_WIRE_BYTES 9992u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDCALL_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDCALL_REQUEST_CODEC_IDENTITY "UWVMROS1_4d652cfbaef29487a0a688932ee926d7a4026c5e7ac8a0eace583b503c37bcd8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request const* value) {
  if(!value)return 0;
  if(value->program_name.size>512u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->params.size>16u)return 0;
  {size_t i;for(i=0;i<value->params.size;++i){
  if(!roswasmc_bounded_2_rmi_msgs_msg_CallParam_valid(&value->params.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request const* value) {size_t n=0;
  n+=4+value->program_name.size;
  n+=4;
  if(value->params.size>16u)return 0;
  {size_t i;for(i=0;i<value->params.size;++i){
  n+=roswasmc_bounded_2_rmi_msgs_msg_CallParam_wire_size(&value->params.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,value->params.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->params.size;++i){
  roswasmc_bounded_2_rmi_msgs_msg_CallParam_write_fields(&value->params.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>512u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<4)return 0;value->params.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->params.size>16u)return 0;
  {size_t i;for(i=0;i<value->params.size;++i){
  if(!roswasmc_bounded_2_rmi_msgs_msg_CallParam_read_fields(p,n,offset,&value->params.data[i]))return 0;
  }}
  return roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>9992u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddCall_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddCall_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field sequence_id; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sequence_id;
} roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDCALL_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDCALL_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDCALL_RESPONSE_CODEC_IDENTITY "UWVMROS1_4f8495dfc3c69ec640444fd24843a7bb3e30008d90945bdd64817d8acb7964e1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sequence_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sequence_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddCall_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddMotionInstruction_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request {
  /** Declared field motion_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionType motion_type;
  /** Declared field incremental; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t incremental;
  /** Declared field representation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Representation representation;
  /** Declared field joint; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Joint joint;
  /** Declared field cartesian; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Cartesian cartesian;
  /** Declared field speed_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_SpeedType speed_type;
  /** Declared field speed_value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t speed_value;
  /** Declared field term_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_TermType term_type;
  /** Declared field term_value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t term_value;
  /** Declared field via_representation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Representation via_representation;
  /** Declared field via_joint; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Joint via_joint;
  /** Declared field via_cartesian; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Cartesian via_cartesian;
  /** Declared field options; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions options;
} roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDMOTIONINSTRUCTION_REQUEST_MAX_WIRE_BYTES 4014u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDMOTIONINSTRUCTION_REQUEST_MIN_WIRE_BYTES 238u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDMOTIONINSTRUCTION_REQUEST_CODEC_IDENTITY "UWVMROS1_3f22c49cb2d32e6a8f3954f3d3cbf0021c87e0363b1f2ded4ac20a97a5b59391"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_default(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_rmi_msgs_msg_MotionType_default(&next.motion_type);
  next.incremental=0;
  (void)roswasmc_bounded_2_rmi_msgs_msg_Representation_default(&next.representation);
  (void)roswasmc_bounded_2_rmi_msgs_msg_Cartesian_default(&next.cartesian);
  (void)roswasmc_bounded_2_rmi_msgs_msg_SpeedType_default(&next.speed_type);
  (void)roswasmc_bounded_2_rmi_msgs_msg_TermType_default(&next.term_type);
  (void)roswasmc_bounded_2_rmi_msgs_msg_Representation_default(&next.via_representation);
  (void)roswasmc_bounded_2_rmi_msgs_msg_Cartesian_default(&next.via_cartesian);
  (void)roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_default(&next.options);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_MotionType_valid(&value->motion_type))return 0;
  if(value->incremental>1)return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Representation_valid(&value->representation))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Joint_valid(&value->joint))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Cartesian_valid(&value->cartesian))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_SpeedType_valid(&value->speed_type))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_TermType_valid(&value->term_type))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Representation_valid(&value->via_representation))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Joint_valid(&value->via_joint))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Cartesian_valid(&value->via_cartesian))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_valid(&value->options))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_rmi_msgs_msg_MotionType_wire_size(&value->motion_type);
  n+=1;
  n+=roswasmc_bounded_2_rmi_msgs_msg_Representation_wire_size(&value->representation);
  n+=roswasmc_bounded_2_rmi_msgs_msg_Joint_wire_size(&value->joint);
  n+=roswasmc_bounded_2_rmi_msgs_msg_Cartesian_wire_size(&value->cartesian);
  n+=roswasmc_bounded_2_rmi_msgs_msg_SpeedType_wire_size(&value->speed_type);
  n+=2;
  n+=roswasmc_bounded_2_rmi_msgs_msg_TermType_wire_size(&value->term_type);
  n+=1;
  n+=roswasmc_bounded_2_rmi_msgs_msg_Representation_wire_size(&value->via_representation);
  n+=roswasmc_bounded_2_rmi_msgs_msg_Joint_wire_size(&value->via_joint);
  n+=roswasmc_bounded_2_rmi_msgs_msg_Cartesian_wire_size(&value->via_cartesian);
  n+=roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_wire_size(&value->options);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_rmi_msgs_msg_MotionType_write_fields(&value->motion_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->incremental,1);*offset+=1;
  roswasmc_bounded_2_rmi_msgs_msg_Representation_write_fields(&value->representation,p,offset);
  roswasmc_bounded_2_rmi_msgs_msg_Joint_write_fields(&value->joint,p,offset);
  roswasmc_bounded_2_rmi_msgs_msg_Cartesian_write_fields(&value->cartesian,p,offset);
  roswasmc_bounded_2_rmi_msgs_msg_SpeedType_write_fields(&value->speed_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed_value,2);*offset+=2;
  roswasmc_bounded_2_rmi_msgs_msg_TermType_write_fields(&value->term_type,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->term_value,1);*offset+=1;
  roswasmc_bounded_2_rmi_msgs_msg_Representation_write_fields(&value->via_representation,p,offset);
  roswasmc_bounded_2_rmi_msgs_msg_Joint_write_fields(&value->via_joint,p,offset);
  roswasmc_bounded_2_rmi_msgs_msg_Cartesian_write_fields(&value->via_cartesian,p,offset);
  roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_write_fields(&value->options,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request* value) {
  if(!roswasmc_bounded_2_rmi_msgs_msg_MotionType_read_fields(p,n,offset,&value->motion_type))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->incremental,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Representation_read_fields(p,n,offset,&value->representation))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Joint_read_fields(p,n,offset,&value->joint))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Cartesian_read_fields(p,n,offset,&value->cartesian))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_SpeedType_read_fields(p,n,offset,&value->speed_type))return 0;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->speed_value,&bits,2);}*offset+=2;
  if(!roswasmc_bounded_2_rmi_msgs_msg_TermType_read_fields(p,n,offset,&value->term_type))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->term_value,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Representation_read_fields(p,n,offset,&value->via_representation))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Joint_read_fields(p,n,offset,&value->via_joint))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Cartesian_read_fields(p,n,offset,&value->via_cartesian))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_MotionInstructionOptions_read_fields(p,n,offset,&value->options))return 0;
  return roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<238u||length>4014u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddMotionInstruction_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field sequence_id; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sequence_id;
} roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDMOTIONINSTRUCTION_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDMOTIONINSTRUCTION_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDMOTIONINSTRUCTION_RESPONSE_CODEC_IDENTITY "UWVMROS1_79fee3e965fda63c3a53ad2178ff48614878acc9f6af74cc27a8c2812b68ac6b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sequence_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sequence_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddMotionInstruction_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddSetPayload_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request {
  /** Declared field schedule_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t schedule_number;
} roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETPAYLOAD_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETPAYLOAD_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETPAYLOAD_REQUEST_CODEC_IDENTITY "UWVMROS1_45da9f81e8c26f49490a4e6eba5c8be0ee67285124540eb1c3e1f2c3e0fd821b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->schedule_number,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->schedule_number,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddSetPayload_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field sequence_id; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sequence_id;
} roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETPAYLOAD_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETPAYLOAD_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETPAYLOAD_RESPONSE_CODEC_IDENTITY "UWVMROS1_bfc5bc7b92f292c2980496cb18fbe953ea1b49d45f7950f0125bbbf880bdf72a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sequence_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sequence_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetPayload_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddSetUFrame_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request {
  /** Declared field frame_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t frame_number;
} roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUFRAME_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUFRAME_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUFRAME_REQUEST_CODEC_IDENTITY "UWVMROS1_64c83dd2be89b4b6dcf3b3c2ea70184971e6a359914ff1a5cd26d4b114d35475"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->frame_number,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->frame_number,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddSetUFrame_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field sequence_id; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sequence_id;
} roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUFRAME_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUFRAME_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUFRAME_RESPONSE_CODEC_IDENTITY "UWVMROS1_e344ef6efacc8d569111d1c6ef4028e1a5bb54af49d3e8f486ae89b83c674647"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sequence_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sequence_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUFrame_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddSetUTool_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request {
  /** Declared field tool_number; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t tool_number;
} roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUTOOL_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUTOOL_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUTOOL_REQUEST_CODEC_IDENTITY "UWVMROS1_d1327022326cd41caed6001449517a41f589e148c38d6db30923922175f05c46"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->tool_number,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tool_number,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddSetUTool_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field sequence_id; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sequence_id;
} roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUTOOL_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUTOOL_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDSETUTOOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_dd4c36458027b87559200cf5f6aa72a60af2d103f663f2fa9daca47ce8020ff6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sequence_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sequence_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddSetUTool_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for rmi_msgs/srv/AddWaitDIN_Request.port_value; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_port_value_text;
/** Complete ROS2 rmi_msgs/srv/AddWaitDIN_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request {
  /** Declared field port_number; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t port_number;
  /** Declared field port_value; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_port_value_text port_value;
} roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITDIN_REQUEST_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITDIN_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITDIN_REQUEST_CODEC_IDENTITY "UWVMROS1_c32214f1ff1b0b6980583fc3088f412e856efb5eaa9c336cf798ef8ad85c61ab"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request const* value) {
  if(!value)return 0;
  if(value->port_value.size>512u||!uwvm_bounded_utf8(value->port_value.data,value->port_value.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request const* value) {size_t n=0;
  n+=2;
  n+=4+value->port_value.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->port_number,2);*offset+=2;
  uwvm_bounded_put(p+*offset,value->port_value.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->port_value.data,value->port_value.size);*offset+=value->port_value.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->port_number,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<4)return 0;value->port_value.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->port_value.size>512u||value->port_value.size>n-*offset)return 0;uwvm_bounded_copy(value->port_value.data,p+*offset,value->port_value.size);*offset+=value->port_value.size;
  return roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddWaitDIN_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field sequence_id; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sequence_id;
} roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITDIN_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITDIN_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITDIN_RESPONSE_CODEC_IDENTITY "UWVMROS1_7d3933f573619ba49f19df7c11f1d73ef211c17581f660f8c0fbcc152cd519d4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sequence_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sequence_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitDIN_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddWaitTime_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request {
  /** Declared field time; canonical capacity is part of CODEC_IDENTITY. */
  float time;
} roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITTIME_REQUEST_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITTIME_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITTIME_REQUEST_CODEC_IDENTITY "UWVMROS1_8487f4b7dcfb044c72f7fb62a31ffe9cdc4321a9ebdaa0550a363e32acbadc46"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->time,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/AddWaitTime_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
  /** Declared field sequence_id; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sequence_id;
} roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITTIME_RESPONSE_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITTIME_RESPONSE_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_ADDWAITTIME_RESPONSE_CODEC_IDENTITY "UWVMROS1_f9578c08b971f32f91d1c94b8713cafe648e25147394e20c85bb335254fa20c8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sequence_id,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sequence_id,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_AddWaitTime_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/CallCommand_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request {
  /** Declared field command; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_Command command;
  /** Declared field options; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_rmi_msgs_msg_CommandOptions options;
} roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_CALLCOMMAND_REQUEST_MAX_WIRE_BYTES 1104u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_CALLCOMMAND_REQUEST_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_CALLCOMMAND_REQUEST_CODEC_IDENTITY "UWVMROS1_d1f890a2beec0ca9294ff4f641d1738855d576a917ed05eb34517886c73f381c"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_default(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_default(&next.options);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_valid(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_Command_valid(&value->command))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_valid(&value->options))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_wire_size(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_rmi_msgs_msg_Command_wire_size(&value->command);
  n+=roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_wire_size(&value->options);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_write_fields(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_rmi_msgs_msg_Command_write_fields(&value->command,p,offset);
  roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_write_fields(&value->options,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request* value) {
  if(!roswasmc_bounded_2_rmi_msgs_msg_Command_read_fields(p,n,offset,&value->command))return 0;
  if(!roswasmc_bounded_2_rmi_msgs_msg_CommandOptions_read_fields(p,n,offset,&value->options))return 0;
  return roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_encode(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request* value) {
  roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>1104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 rmi_msgs/srv/CallCommand_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t result;
} roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response;
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_CALLCOMMAND_RESPONSE_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_CALLCOMMAND_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_RMI_MSGS_SRV_CALLCOMMAND_RESPONSE_CODEC_IDENTITY "UWVMROS1_64d92e6fadc651073469318ad15d2e98ec3d059c629838cefc5820b17ab6f1bc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_valid(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_wire_size(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_write_fields(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_encode(roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_decode(void const* input,size_t length,roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response* value) {
  roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_rmi_msgs_srv_CallCommand_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
