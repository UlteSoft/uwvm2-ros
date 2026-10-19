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
/** UTF-8 byte storage for geometry_msgs/msg/TransformStamped.child_frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_child_frame_id_text;
/** Complete ROS2 geometry_msgs/msg/TransformStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_TransformStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field child_frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_child_frame_id_text child_frame_id;
  /** Declared field transform; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Transform transform;
} roswasmc_bounded_2_geometry_msgs_msg_TransformStamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORMSTAMPED_MAX_WIRE_BYTES 648u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORMSTAMPED_MIN_WIRE_BYTES 72u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORMSTAMPED_CODEC_IDENTITY "UWVMROS1_be553b5a9c73b687cde56ff1c381e3c67168fd2c0366830be3c65382c5bbd792"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_default(roswasmc_bounded_2_geometry_msgs_msg_TransformStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_TransformStamped next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_Transform_default(&next.transform);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_valid(roswasmc_bounded_2_geometry_msgs_msg_TransformStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->child_frame_id.size>512u||!uwvm_bounded_utf8(value->child_frame_id.data,value->child_frame_id.size))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_valid(&value->transform))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_wire_size(roswasmc_bounded_2_geometry_msgs_msg_TransformStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4+value->child_frame_id.size;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Transform_wire_size(&value->transform);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_write_fields(roswasmc_bounded_2_geometry_msgs_msg_TransformStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->child_frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->child_frame_id.data,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  roswasmc_bounded_2_geometry_msgs_msg_Transform_write_fields(&value->transform,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_TransformStamped* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->child_frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->child_frame_id.size>512u||value->child_frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->child_frame_id.data,p+*offset,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_read_fields(p,n,offset,&value->transform))return 0;
  return roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_encode(roswasmc_bounded_2_geometry_msgs_msg_TransformStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_TransformStamped* value) {
  roswasmc_bounded_2_geometry_msgs_msg_TransformStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<72u||length>648u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/msg/DebugLevel value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_msg_DebugLevel {
  /** Declared field val; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t val;
} roswasmc_bounded_2_industrial_msgs_msg_DebugLevel;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_CODEC_IDENTITY "UWVMROS1_989475cc5d981d8c5ab91fb091235871f7a1c7d87d246eb2250086e6c3c37c79"
/** Original scalar constant DEBUG=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_DEBUG UINT32_C(5)
/** Original scalar constant INFO=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_INFO UINT32_C(4)
/** Original scalar constant WARN=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_WARN UINT32_C(3)
/** Original scalar constant ERROR=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_ERROR UINT32_C(2)
/** Original scalar constant FATAL=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_FATAL UINT32_C(1)
/** Original scalar constant NONE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEBUGLEVEL_NONE UINT32_C(0)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_valid(roswasmc_bounded_2_industrial_msgs_msg_DebugLevel const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_wire_size(roswasmc_bounded_2_industrial_msgs_msg_DebugLevel const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_write_fields(roswasmc_bounded_2_industrial_msgs_msg_DebugLevel const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->val,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_msg_DebugLevel* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->val,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_encode(roswasmc_bounded_2_industrial_msgs_msg_DebugLevel const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_msg_DebugLevel* value) {
  roswasmc_bounded_2_industrial_msgs_msg_DebugLevel next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for industrial_msgs/msg/DeviceInfo.model; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_model_text;
/** UTF-8 byte storage for industrial_msgs/msg/DeviceInfo.serial_number; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_serial_number_text;
/** UTF-8 byte storage for industrial_msgs/msg/DeviceInfo.hw_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_hw_version_text;
/** UTF-8 byte storage for industrial_msgs/msg/DeviceInfo.sw_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_sw_version_text;
/** UTF-8 byte storage for industrial_msgs/msg/DeviceInfo.address; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_address_text;
/** Complete ROS2 industrial_msgs/msg/DeviceInfo value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo {
  /** Declared field model; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_model_text model;
  /** Declared field serial_number; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_serial_number_text serial_number;
  /** Declared field hw_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_hw_version_text hw_version;
  /** Declared field sw_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_sw_version_text sw_version;
  /** Declared field address; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_address_text address;
} roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEVICEINFO_MAX_WIRE_BYTES 2580u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEVICEINFO_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_DEVICEINFO_CODEC_IDENTITY "UWVMROS1_f9a5962800e17495f34dbb73f37373df22f900e528c630497417f78cdecb283b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_valid(roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo const* value) {
  if(!value)return 0;
  if(value->model.size>512u||!uwvm_bounded_utf8(value->model.data,value->model.size))return 0;
  if(value->serial_number.size>512u||!uwvm_bounded_utf8(value->serial_number.data,value->serial_number.size))return 0;
  if(value->hw_version.size>512u||!uwvm_bounded_utf8(value->hw_version.data,value->hw_version.size))return 0;
  if(value->sw_version.size>512u||!uwvm_bounded_utf8(value->sw_version.data,value->sw_version.size))return 0;
  if(value->address.size>512u||!uwvm_bounded_utf8(value->address.data,value->address.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_wire_size(roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo const* value) {size_t n=0;
  n+=4+value->model.size;
  n+=4+value->serial_number.size;
  n+=4+value->hw_version.size;
  n+=4+value->sw_version.size;
  n+=4+value->address.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_write_fields(roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->model.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->model.data,value->model.size);*offset+=value->model.size;
  uwvm_bounded_put(p+*offset,value->serial_number.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->serial_number.data,value->serial_number.size);*offset+=value->serial_number.size;
  uwvm_bounded_put(p+*offset,value->hw_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->hw_version.data,value->hw_version.size);*offset+=value->hw_version.size;
  uwvm_bounded_put(p+*offset,value->sw_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->sw_version.data,value->sw_version.size);*offset+=value->sw_version.size;
  uwvm_bounded_put(p+*offset,value->address.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->address.data,value->address.size);*offset+=value->address.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo* value) {
  if(*offset>n||n-*offset<4)return 0;value->model.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->model.size>512u||value->model.size>n-*offset)return 0;uwvm_bounded_copy(value->model.data,p+*offset,value->model.size);*offset+=value->model.size;
  if(*offset>n||n-*offset<4)return 0;value->serial_number.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->serial_number.size>512u||value->serial_number.size>n-*offset)return 0;uwvm_bounded_copy(value->serial_number.data,p+*offset,value->serial_number.size);*offset+=value->serial_number.size;
  if(*offset>n||n-*offset<4)return 0;value->hw_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->hw_version.size>512u||value->hw_version.size>n-*offset)return 0;uwvm_bounded_copy(value->hw_version.data,p+*offset,value->hw_version.size);*offset+=value->hw_version.size;
  if(*offset>n||n-*offset<4)return 0;value->sw_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->sw_version.size>512u||value->sw_version.size>n-*offset)return 0;uwvm_bounded_copy(value->sw_version.data,p+*offset,value->sw_version.size);*offset+=value->sw_version.size;
  if(*offset>n||n-*offset<4)return 0;value->address.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->address.size>512u||value->address.size>n-*offset)return 0;uwvm_bounded_copy(value->address.data,p+*offset,value->address.size);*offset+=value->address.size;
  return roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_encode(roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo* value) {
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>2580u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/msg/RobotMode value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_msg_RobotMode {
  /** Declared field val; canonical capacity is part of CODEC_IDENTITY. */
  int8_t val;
} roswasmc_bounded_2_industrial_msgs_msg_RobotMode;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTMODE_CODEC_IDENTITY "UWVMROS1_4f9173c5e27815ca33be603b8645a22420bda3ab308962127060d2413d092c4c"
/** Original scalar constant UNKNOWN=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTMODE_UNKNOWN -INT32_C(1)
/** Original scalar constant MANUAL=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTMODE_MANUAL INT32_C(1)
/** Original scalar constant AUTO=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTMODE_AUTO INT32_C(2)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_RobotMode_valid(roswasmc_bounded_2_industrial_msgs_msg_RobotMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_msg_RobotMode_wire_size(roswasmc_bounded_2_industrial_msgs_msg_RobotMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_msg_RobotMode_write_fields(roswasmc_bounded_2_industrial_msgs_msg_RobotMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->val,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_RobotMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_msg_RobotMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->val,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_industrial_msgs_msg_RobotMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_RobotMode_encode(roswasmc_bounded_2_industrial_msgs_msg_RobotMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_msg_RobotMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_msg_RobotMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_msg_RobotMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_RobotMode_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_msg_RobotMode* value) {
  roswasmc_bounded_2_industrial_msgs_msg_RobotMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_msg_RobotMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/msg/TriState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_msg_TriState {
  /** Declared field val; canonical capacity is part of CODEC_IDENTITY. */
  int8_t val;
} roswasmc_bounded_2_industrial_msgs_msg_TriState;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_CODEC_IDENTITY "UWVMROS1_b7ae62ce19d642ad46e962ffb50cd76c43c96ac673ac36d690f99c1dd8d2a841"
/** Original scalar constant UNKNOWN=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_UNKNOWN -INT32_C(1)
/** Original scalar constant TRUE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_TRUE INT32_C(1)
/** Original scalar constant ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_ON INT32_C(1)
/** Original scalar constant ENABLED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_ENABLED INT32_C(1)
/** Original scalar constant HIGH=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_HIGH INT32_C(1)
/** Original scalar constant CLOSED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_CLOSED INT32_C(1)
/** Original scalar constant FALSE=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_FALSE INT32_C(0)
/** Original scalar constant OFF=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_OFF INT32_C(0)
/** Original scalar constant DISABLED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_DISABLED INT32_C(0)
/** Original scalar constant LOW=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_LOW INT32_C(0)
/** Original scalar constant OPEN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_TRISTATE_OPEN INT32_C(0)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(roswasmc_bounded_2_industrial_msgs_msg_TriState const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(roswasmc_bounded_2_industrial_msgs_msg_TriState const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_msg_TriState_write_fields(roswasmc_bounded_2_industrial_msgs_msg_TriState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->val,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_TriState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_msg_TriState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->val,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_TriState_encode(roswasmc_bounded_2_industrial_msgs_msg_TriState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_msg_TriState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_TriState_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_msg_TriState* value) {
  roswasmc_bounded_2_industrial_msgs_msg_TriState next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;int32_t data[16];} roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_error_codes_sequence;
/** Complete ROS2 industrial_msgs/msg/RobotStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_msg_RobotStatus {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_RobotMode mode;
  /** Declared field e_stopped; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_TriState e_stopped;
  /** Declared field drives_powered; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_TriState drives_powered;
  /** Declared field motion_possible; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_TriState motion_possible;
  /** Declared field in_motion; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_TriState in_motion;
  /** Declared field in_error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_TriState in_error;
  /** Declared field error_codes; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_error_codes_sequence error_codes;
} roswasmc_bounded_2_industrial_msgs_msg_RobotStatus;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTSTATUS_MAX_WIRE_BYTES 150u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTSTATUS_MIN_WIRE_BYTES 22u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_ROBOTSTATUS_CODEC_IDENTITY "UWVMROS1_1086f16ba5d614f3d87c3ab195240b5a0683ef490f2e92434e4ea3bdbe7554ba"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_valid(roswasmc_bounded_2_industrial_msgs_msg_RobotStatus const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_RobotMode_valid(&value->mode))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(&value->e_stopped))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(&value->drives_powered))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(&value->motion_possible))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(&value->in_motion))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_valid(&value->in_error))return 0;
  if(value->error_codes.size>16u)return 0;
  {size_t i;for(i=0;i<value->error_codes.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_wire_size(roswasmc_bounded_2_industrial_msgs_msg_RobotStatus const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_industrial_msgs_msg_RobotMode_wire_size(&value->mode);
  n+=roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(&value->e_stopped);
  n+=roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(&value->drives_powered);
  n+=roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(&value->motion_possible);
  n+=roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(&value->in_motion);
  n+=roswasmc_bounded_2_industrial_msgs_msg_TriState_wire_size(&value->in_error);
  n+=4;
  if(value->error_codes.size>16u)return 0;
  {size_t i;for(i=0;i<value->error_codes.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_write_fields(roswasmc_bounded_2_industrial_msgs_msg_RobotStatus const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_industrial_msgs_msg_RobotMode_write_fields(&value->mode,p,offset);
  roswasmc_bounded_2_industrial_msgs_msg_TriState_write_fields(&value->e_stopped,p,offset);
  roswasmc_bounded_2_industrial_msgs_msg_TriState_write_fields(&value->drives_powered,p,offset);
  roswasmc_bounded_2_industrial_msgs_msg_TriState_write_fields(&value->motion_possible,p,offset);
  roswasmc_bounded_2_industrial_msgs_msg_TriState_write_fields(&value->in_motion,p,offset);
  roswasmc_bounded_2_industrial_msgs_msg_TriState_write_fields(&value->in_error,p,offset);
  uwvm_bounded_put(p+*offset,value->error_codes.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->error_codes.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_codes.data[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_msg_RobotStatus* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_RobotMode_read_fields(p,n,offset,&value->mode))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_read_fields(p,n,offset,&value->e_stopped))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_read_fields(p,n,offset,&value->drives_powered))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_read_fields(p,n,offset,&value->motion_possible))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_read_fields(p,n,offset,&value->in_motion))return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_TriState_read_fields(p,n,offset,&value->in_error))return 0;
  if(*offset>n||n-*offset<4)return 0;value->error_codes.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_codes.size>16u)return 0;
  {size_t i;for(i=0;i<value->error_codes.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error_codes.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_encode(roswasmc_bounded_2_industrial_msgs_msg_RobotStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_msg_RobotStatus* value) {
  roswasmc_bounded_2_industrial_msgs_msg_RobotStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<22u||length>150u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_msg_RobotStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/msg/ServiceReturnCode value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode {
  /** Declared field val; canonical capacity is part of CODEC_IDENTITY. */
  int8_t val;
} roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_SERVICERETURNCODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_SERVICERETURNCODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_SERVICERETURNCODE_CODEC_IDENTITY "UWVMROS1_62eb4415a63d7686f16ea343193238838c78aa8e9b9786a65c92fb227e03d3e5"
/** Original scalar constant SUCCESS=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_SERVICERETURNCODE_SUCCESS INT32_C(1)
/** Original scalar constant FAILURE=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_MSG_SERVICERETURNCODE_FAILURE -INT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->val,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->val,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_encode(roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode* value) {
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/CmdJointTrajectory_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request {
  /** Declared field trajectory; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory trajectory;
} roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_CMDJOINTTRAJECTORY_REQUEST_MAX_WIRE_BYTES 4164u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_CMDJOINTTRAJECTORY_REQUEST_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_CMDJOINTTRAJECTORY_REQUEST_CODEC_IDENTITY "UWVMROS1_1def3934b243546a4591b7be1ef2eb27002fbd9d3725302016e3b1846805149d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_valid(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(&value->trajectory))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_wire_size(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(&value->trajectory);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_write_fields(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_write_fields(&value->trajectory,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request* value) {
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_read_fields(p,n,offset,&value->trajectory))return 0;
  return roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_encode(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request* value) {
  roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>4164u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/CmdJointTrajectory_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response {
  /** Declared field code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode code;
} roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_CMDJOINTTRAJECTORY_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_CMDJOINTTRAJECTORY_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_CMDJOINTTRAJECTORY_RESPONSE_CODEC_IDENTITY "UWVMROS1_1e4d064ef659e5d1cc73bdd90f82657a16087c96e0f75b632e2be2be884bca8b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_valid(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(&value->code))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_wire_size(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(&value->code);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_write_fields(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(&value->code,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response* value) {
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields(p,n,offset,&value->code))return 0;
  return roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_encode(roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response* value) {
  roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_CmdJointTrajectory_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/GetRobotInfo_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_REQUEST_CODEC_IDENTITY "UWVMROS1_688a272f6ee39921013c6c39a7b0b0c4e2cda5119787088234103518ba25c1d2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_valid(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_wire_size(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_write_fields(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request* value) {
  return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_encode(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request* value) {
  roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo data[16];} roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_robots_sequence;
/** Complete ROS2 industrial_msgs/srv/GetRobotInfo_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response {
  /** Declared field controller; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo controller;
  /** Declared field robots; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_robots_sequence robots;
  /** Declared field code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode code;
} roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_RESPONSE_MAX_WIRE_BYTES 32768u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_RESPONSE_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_RESPONSE_CODEC_IDENTITY "UWVMROS1_6d37cdcd8afc918329d7856e682d7fcdae2931832474a9194143c21079ed4152"
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_RESPONSE_CARTESIAN_MAX_WIRE_BYTES UINT64_C(43865)
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_GETROBOTINFO_RESPONSE_AGGREGATE_WIRE_BYTES 32768u
/** Shape validation retains every field; aggregate quota is checked separately. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_shape(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response const* value) {if(!value)return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_valid(&value->controller))return 0;
  if(value->robots.size>16u)return 0;
  {size_t i;for(i=0;i<value->robots.size;++i){
  if(!roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_valid(&value->robots.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(&value->code))return 0;
  return 1;}
/** Checked addition under this record's full admitted wire extent. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_add(size_t* n,size_t amount) {if(*n>32768u||amount>32768u-*n)return 0;*n+=amount;return 1;}
/** Full validation and measure; quota90 preserves the caller's size sentinel. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_measure(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response const* value,size_t* length) {size_t n=0;
  if(!length||!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_shape(value)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  {size_t nested=roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_wire_size(&value->controller);if(!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_add(&n,nested))return UWVM_BOUNDED_TOO_LARGE;}
  if(!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->robots.size;++i){
  {size_t nested=roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_wire_size(&value->robots.data[i]);if(!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_add(&n,nested))return UWVM_BOUNDED_TOO_LARGE;}
  }}
  {size_t nested=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(&value->code);if(!roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_add(&n,nested))return UWVM_BOUNDED_TOO_LARGE;}
  *length=n;return 0;}
static inline int roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_valid(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response const* value) {size_t n=0;return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_measure(value,&n)==0;}
/** Compatibility size view; use measure for an unambiguous error/status. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_wire_size(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response const* value) {size_t n=0;return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_measure(value,&n)?0:n;}
/** Internal no-fail writer, entered only after complete measure/overlap proof. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_write_fields(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_write_fields(&value->controller,p,offset);
  uwvm_bounded_put(p+*offset,value->robots.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->robots.size;++i){
  roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_write_fields(&value->robots.data[i],p,offset);
  }}
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(&value->code,p,offset);
}
/** Internal reader/status keeps all intermediate mutations private. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_read_status(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response* value) {size_t measured=0;
  if(!value||!offset||*offset>n||(n&&!p))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_read_fields(p,n,offset,&value->controller))return UWVM_BOUNDED_INVALID;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->robots.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->robots.size>16u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->robots.size;++i){
  if(!roswasmc_bounded_2_industrial_msgs_msg_DeviceInfo_read_fields(p,n,offset,&value->robots.data[i]))return UWVM_BOUNDED_INVALID;
  }}
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields(p,n,offset,&value->code))return UWVM_BOUNDED_INVALID;
  return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_measure(value,&measured);
}
static inline int roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response* value) {return roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_read_status(p,n,offset,value)==0;}
/** No writes before full shape, nested quotas, exact size and disjoint destinations. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_encode(roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response const* value,void* output,size_t capacity,size_t* length) {size_t n=0,offset=0;uint32_t error;
  if(!length)return UWVM_BOUNDED_INVALID;error=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_measure(value,&n);if(error)return error;
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;}
/** The compiler may reserve the full temporary stack frame even on preflight failure. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response* value) {size_t offset=0;uint32_t error;
  if(!value||(length&&!input)||length<25u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(length>32768u)return UWVM_BOUNDED_TOO_LARGE;
  {roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response next={0};error=roswasmc_bounded_2_industrial_msgs_srv_GetRobotInfo_Response_read_status((unsigned char const*)input,length,&offset,&next);if(error)return error;if(offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;}}
/** Complete ROS2 industrial_msgs/srv/SetDrivePower_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request {
  /** Declared field drive_power; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t drive_power;
} roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETDRIVEPOWER_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETDRIVEPOWER_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETDRIVEPOWER_REQUEST_CODEC_IDENTITY "UWVMROS1_b4fc5e109e131a97820ab5856b34839c572ac0b0c256d70457c176e401844f96"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_valid(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request const* value) {
  if(!value)return 0;
  if(value->drive_power>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_wire_size(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_write_fields(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->drive_power,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->drive_power,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_encode(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request* value) {
  roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/SetDrivePower_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response {
  /** Declared field code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode code;
} roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETDRIVEPOWER_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETDRIVEPOWER_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETDRIVEPOWER_RESPONSE_CODEC_IDENTITY "UWVMROS1_050fb9a727ebedd240e9023b9a2379a5d72e0d9fa335bead0c40d783abb22cff"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_valid(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(&value->code))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_wire_size(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(&value->code);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_write_fields(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(&value->code,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response* value) {
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields(p,n,offset,&value->code))return 0;
  return roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_encode(roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response* value) {
  roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_SetDrivePower_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/SetRemoteLoggerLevel_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request {
  /** Declared field level; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_DebugLevel level;
} roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETREMOTELOGGERLEVEL_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETREMOTELOGGERLEVEL_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETREMOTELOGGERLEVEL_REQUEST_CODEC_IDENTITY "UWVMROS1_bbadd39c05cab90b14f4b1fad23ff05ac83325f6cc57810cd649e04fa29fe014"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_valid(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_valid(&value->level))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_wire_size(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_wire_size(&value->level);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_write_fields(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_write_fields(&value->level,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request* value) {
  if(!roswasmc_bounded_2_industrial_msgs_msg_DebugLevel_read_fields(p,n,offset,&value->level))return 0;
  return roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_encode(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request* value) {
  roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/SetRemoteLoggerLevel_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response {
  /** Declared field code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode code;
} roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETREMOTELOGGERLEVEL_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETREMOTELOGGERLEVEL_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_SETREMOTELOGGERLEVEL_RESPONSE_CODEC_IDENTITY "UWVMROS1_b8681d031a4bb5a67b842277af2e2156e8339bdef23be88d2b64e296a7e3d63b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_valid(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(&value->code))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_wire_size(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(&value->code);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_write_fields(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(&value->code,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response* value) {
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields(p,n,offset,&value->code))return 0;
  return roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_encode(roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response* value) {
  roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_SetRemoteLoggerLevel_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/StartMotion_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STARTMOTION_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STARTMOTION_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STARTMOTION_REQUEST_CODEC_IDENTITY "UWVMROS1_0a96fb9489f863612a91bf82f995f2f28bcb7fb403bde74c617925c789e4dcd8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_valid(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_wire_size(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_write_fields(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request* value) {
  return roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_encode(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request* value) {
  roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/StartMotion_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response {
  /** Declared field code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode code;
} roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STARTMOTION_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STARTMOTION_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STARTMOTION_RESPONSE_CODEC_IDENTITY "UWVMROS1_3bb3650fbe638a6d300bcd9fcd5acf574bd54d00a7368edd3014bf44dcf66c16"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_valid(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(&value->code))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_wire_size(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(&value->code);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_write_fields(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(&value->code,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response* value) {
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields(p,n,offset,&value->code))return 0;
  return roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_encode(roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response* value) {
  roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_StartMotion_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/StopMotion_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STOPMOTION_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STOPMOTION_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STOPMOTION_REQUEST_CODEC_IDENTITY "UWVMROS1_fe5a7cf0da30270eab5cfb1ab8c1fbd37d2b4c4f91ef29662a0ddcac39e6ef49"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_valid(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_wire_size(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_write_fields(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request* value) {
  return roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_encode(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request* value) {
  roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 industrial_msgs/srv/StopMotion_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response {
  /** Declared field code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode code;
} roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response;
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STOPMOTION_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STOPMOTION_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_INDUSTRIAL_MSGS_SRV_STOPMOTION_RESPONSE_CODEC_IDENTITY "UWVMROS1_c26bda240d86becc5b8b4ea80380eba3602092a5ec4a06818dbfbd38c24015b5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_valid(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_valid(&value->code))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_wire_size(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_wire_size(&value->code);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_write_fields(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_write_fields(&value->code,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response* value) {
  if(!roswasmc_bounded_2_industrial_msgs_msg_ServiceReturnCode_read_fields(p,n,offset,&value->code))return 0;
  return roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_encode(roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_decode(void const* input,size_t length,roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response* value) {
  roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_industrial_msgs_srv_StopMotion_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/msg/AlarmCauseRemedy.cause; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_cause_text;
/** UTF-8 byte storage for motoros2_interfaces/msg/AlarmCauseRemedy.remedy; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_remedy_text;
/** Complete ROS2 motoros2_interfaces/msg/AlarmCauseRemedy value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy {
  /** Declared field cause; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_cause_text cause;
  /** Declared field remedy; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_remedy_text remedy;
} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ALARMCAUSEREMEDY_MAX_WIRE_BYTES 1032u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ALARMCAUSEREMEDY_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ALARMCAUSEREMEDY_CODEC_IDENTITY "UWVMROS1_f977609022f4ca593b9089d0038d1c28d1bc4b533e272f38c3379495f326ddf5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_valid(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy const* value) {
  if(!value)return 0;
  if(value->cause.size>512u||!uwvm_bounded_utf8(value->cause.data,value->cause.size))return 0;
  if(value->remedy.size>512u||!uwvm_bounded_utf8(value->remedy.data,value->remedy.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy const* value) {size_t n=0;
  n+=4+value->cause.size;
  n+=4+value->remedy.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->cause.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->cause.data,value->cause.size);*offset+=value->cause.size;
  uwvm_bounded_put(p+*offset,value->remedy.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->remedy.data,value->remedy.size);*offset+=value->remedy.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy* value) {
  if(*offset>n||n-*offset<4)return 0;value->cause.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->cause.size>512u||value->cause.size>n-*offset)return 0;uwvm_bounded_copy(value->cause.data,p+*offset,value->cause.size);*offset+=value->cause.size;
  if(*offset>n||n-*offset<4)return 0;value->remedy.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->remedy.size>512u||value->remedy.size>n-*offset)return 0;uwvm_bounded_copy(value->remedy.data,p+*offset,value->remedy.size);*offset+=value->remedy.size;
  return roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_encode(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>1032u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/msg/AlarmInfo.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_message_text;
/** UTF-8 byte storage for motoros2_interfaces/msg/AlarmInfo.contents; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_contents_text;
/** UTF-8 byte storage for motoros2_interfaces/msg/AlarmInfo.description; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_description_text;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy data[16];} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_help_sequence;
/** Complete ROS2 motoros2_interfaces/msg/AlarmInfo value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo {
  /** Declared field number; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t number;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_message_text message;
  /** Declared field sub_code; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sub_code;
  /** Declared field datetime; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Time datetime;
  /** Declared field contents; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_contents_text contents;
  /** Declared field description; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_description_text description;
  /** Declared field help; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_help_sequence help;
} roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ALARMINFO_MAX_WIRE_BYTES 25248u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ALARMINFO_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ALARMINFO_CODEC_IDENTITY "UWVMROS1_7cf3fa3037f28f295dd62009e708ea3e46a80f83471d0a0f9fb55af125c0c27f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_valid(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(&value->datetime))return 0;
  if(value->contents.size>4096u||!uwvm_bounded_utf8(value->contents.data,value->contents.size))return 0;
  if(value->description.size>4096u||!uwvm_bounded_utf8(value->description.data,value->description.size))return 0;
  if(value->help.size>16u)return 0;
  {size_t i;for(i=0;i<value->help.size;++i){
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_valid(&value->help.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=4;
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(&value->datetime);
  n+=4+value->contents.size;
  n+=4+value->description.size;
  n+=4;
  if(value->help.size>16u)return 0;
  {size_t i;for(i=0;i<value->help.size;++i){
  n+=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_wire_size(&value->help.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->number,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sub_code,4);*offset+=4;
  roswasmc_bounded_2_builtin_interfaces_msg_Time_write_fields(&value->datetime,p,offset);
  uwvm_bounded_put(p+*offset,value->contents.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->contents.data,value->contents.size);*offset+=value->contents.size;
  uwvm_bounded_put(p+*offset,value->description.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->description.data,value->description.size);*offset+=value->description.size;
  uwvm_bounded_put(p+*offset,value->help.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->help.size;++i){
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_write_fields(&value->help.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->number,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sub_code,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_read_fields(p,n,offset,&value->datetime))return 0;
  if(*offset>n||n-*offset<4)return 0;value->contents.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->contents.size>4096u||value->contents.size>n-*offset)return 0;uwvm_bounded_copy(value->contents.data,p+*offset,value->contents.size);*offset+=value->contents.size;
  if(*offset>n||n-*offset<4)return 0;value->description.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->description.size>4096u||value->description.size>n-*offset)return 0;uwvm_bounded_copy(value->description.data,p+*offset,value->description.size);*offset+=value->description.size;
  if(*offset>n||n-*offset<4)return 0;value->help.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->help.size>16u)return 0;
  {size_t i;for(i=0;i<value->help.size;++i){
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmCauseRemedy_read_fields(p,n,offset,&value->help.data[i]))return 0;
  }}
  return roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_encode(roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>25248u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/msg/ErrorInfo.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_message_text;
/** Complete ROS2 motoros2_interfaces/msg/ErrorInfo value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo {
  /** Declared field number; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t number;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_message_text message;
  /** Declared field sub_code; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sub_code;
  /** Declared field datetime; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Time datetime;
} roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ERRORINFO_MAX_WIRE_BYTES 532u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ERRORINFO_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_ERRORINFO_CODEC_IDENTITY "UWVMROS1_1cfe60c0af6bc157db84c4d9e0b20a8f2e5c78fd4afd6908d32b3217f3734b67"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_valid(roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_valid(&value->datetime))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=4;
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Time_wire_size(&value->datetime);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->number,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sub_code,4);*offset+=4;
  roswasmc_bounded_2_builtin_interfaces_msg_Time_write_fields(&value->datetime,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->number,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sub_code,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Time_read_fields(p,n,offset,&value->datetime))return 0;
  return roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_encode(roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>532u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/msg/InformJobCrudResultCodes value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_CODEC_IDENTITY "UWVMROS1_f5ed037b83926b48a88fa486ed8db5f5d3e39e4a486d4a90190ffd6a70452613"
/** Original scalar constant RC_OK=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_RC_OK UINT32_C(1)
/** Original scalar constant STR_OK="Success"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_STR_OK "\123\165\143\143\145\163\163"
/** Original scalar constant RC_ERR_REFRESHING_JOB_LIST=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_RC_ERR_REFRESHING_JOB_LIST UINT32_C(10)
/** Original scalar constant STR_ERR_REFRESHING_JOB_LIST="Error refreshing job list"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_STR_ERR_REFRESHING_JOB_LIST "\105\162\162\157\162\040\162\145\146\162\145\163\150\151\156\147\040\152\157\142\040\154\151\163\164"
/** Original scalar constant RC_ERR_RETRIEVING_FILE_COUNT=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_RC_ERR_RETRIEVING_FILE_COUNT UINT32_C(11)
/** Original scalar constant STR_ERR_RETRIEVING_FILE_COUNT="Error retrieving file count"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_STR_ERR_RETRIEVING_FILE_COUNT "\105\162\162\157\162\040\162\145\164\162\151\145\166\151\156\147\040\146\151\154\145\040\143\157\165\156\164"
/** Original scalar constant RC_ERR_TOO_MANY_JOBS=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_RC_ERR_TOO_MANY_JOBS UINT32_C(12)
/** Original scalar constant STR_ERR_TOO_MANY_JOBS="Too many jobs"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_STR_ERR_TOO_MANY_JOBS "\124\157\157\040\155\141\156\171\040\152\157\142\163"
/** Original scalar constant RC_ERR_RETRIEVING_JOB_NAME_FROM_LIST=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_RC_ERR_RETRIEVING_JOB_NAME_FROM_LIST UINT32_C(13)
/** Original scalar constant STR_ERR_RETRIEVING_JOB_NAME_FROM_LIST="Failed retrieving job name from list"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_STR_ERR_RETRIEVING_JOB_NAME_FROM_LIST "\106\141\151\154\145\144\040\162\145\164\162\151\145\166\151\156\147\040\152\157\142\040\156\141\155\145\040\146\162\157\155\040\154\151\163\164"
/** Original scalar constant RC_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_RC_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE UINT32_C(14)
/** Original scalar constant STR_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE="Error adding job name to service response"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INFORMJOBCRUDRESULTCODES_STR_ERR_APPENDING_JOB_NAME_TO_SVC_RESPONSE "\105\162\162\157\162\040\141\144\144\151\156\147\040\152\157\142\040\156\141\155\145\040\164\157\040\163\145\162\166\151\143\145\040\162\145\163\160\157\156\163\145"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_valid(roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes* value) {
  return roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_encode(roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_InformJobCrudResultCodes_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/msg/InitTrajEnum value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t value;
} roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_CODEC_IDENTITY "UWVMROS1_9279979f9e3570f48d6e23bf812a4680e3deed9af3309019efdff5464d7b7763"
/** Original scalar constant INIT_TRAJ_OK=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_OK UINT32_C(0)
/** Original scalar constant INIT_TRAJ_OK_STR="OK"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_OK_STR "\117\113"
/** Original scalar constant INIT_TRAJ_UNSPECIFIED=200; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_UNSPECIFIED UINT32_C(200)
/** Original scalar constant INIT_TRAJ_UNSPECIFIED_STR="Unspecified failure"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_UNSPECIFIED_STR "\125\156\163\160\145\143\151\146\151\145\144\040\146\141\151\154\165\162\145"
/** Original scalar constant INIT_TRAJ_TOO_SMALL=201; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_TOO_SMALL UINT32_C(201)
/** Original scalar constant INIT_TRAJ_TOO_SMALL_STR="Trajectory must contain at least two points."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_TOO_SMALL_STR "\124\162\141\152\145\143\164\157\162\171\040\155\165\163\164\040\143\157\156\164\141\151\156\040\141\164\040\154\145\141\163\164\040\164\167\157\040\160\157\151\156\164\163\056"
/** Original scalar constant INIT_TRAJ_TOO_BIG=202; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_TOO_BIG UINT32_C(202)
/** Original scalar constant INIT_TRAJ_TOO_BIG_STR="Trajectory contains too many points (Not enough memory)."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_TOO_BIG_STR "\124\162\141\152\145\143\164\157\162\171\040\143\157\156\164\141\151\156\163\040\164\157\157\040\155\141\156\171\040\160\157\151\156\164\163\040\050\116\157\164\040\145\156\157\165\147\150\040\155\145\155\157\162\171\051\056"
/** Original scalar constant INIT_TRAJ_ALREADY_IN_MOTION=203; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_ALREADY_IN_MOTION UINT32_C(203)
/** Original scalar constant INIT_TRAJ_ALREADY_IN_MOTION_STR="Already running a trajectory."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_ALREADY_IN_MOTION_STR "\101\154\162\145\141\144\171\040\162\165\156\156\151\156\147\040\141\040\164\162\141\152\145\143\164\157\162\171\056"
/** Original scalar constant INIT_TRAJ_INVALID_STARTING_POS=204; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_STARTING_POS UINT32_C(204)
/** Original scalar constant INIT_TRAJ_INVALID_STARTING_POS_STR="The first point must match the robot's current position."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_STARTING_POS_STR "\124\150\145\040\146\151\162\163\164\040\160\157\151\156\164\040\155\165\163\164\040\155\141\164\143\150\040\164\150\145\040\162\157\142\157\164\047\163\040\143\165\162\162\145\156\164\040\160\157\163\151\164\151\157\156\056"
/** Original scalar constant INIT_TRAJ_INVALID_VELOCITY=205; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_VELOCITY UINT32_C(205)
/** Original scalar constant INIT_TRAJ_INVALID_VELOCITY_STR="The commanded velocity is too high."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_VELOCITY_STR "\124\150\145\040\143\157\155\155\141\156\144\145\144\040\166\145\154\157\143\151\164\171\040\151\163\040\164\157\157\040\150\151\147\150\056"
/** Original scalar constant INIT_TRAJ_INVALID_JOINTNAME=206; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_JOINTNAME UINT32_C(206)
/** Original scalar constant INIT_TRAJ_INVALID_JOINTNAME_STR="Invalid joint name specified. Check motoros2_config.yaml."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_JOINTNAME_STR "\111\156\166\141\154\151\144\040\152\157\151\156\164\040\156\141\155\145\040\163\160\145\143\151\146\151\145\144\056\040\103\150\145\143\153\040\155\157\164\157\162\157\163\062\137\143\157\156\146\151\147\056\171\141\155\154\056"
/** Original scalar constant INIT_TRAJ_INCOMPLETE_JOINTLIST=207; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INCOMPLETE_JOINTLIST UINT32_C(207)
/** Original scalar constant INIT_TRAJ_INCOMPLETE_JOINTLIST_STR="Trajectory must contain data for all joints."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INCOMPLETE_JOINTLIST_STR "\124\162\141\152\145\143\164\157\162\171\040\155\165\163\164\040\143\157\156\164\141\151\156\040\144\141\164\141\040\146\157\162\040\141\154\154\040\152\157\151\156\164\163\056"
/** Original scalar constant INIT_TRAJ_INVALID_TIME=208; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_TIME UINT32_C(208)
/** Original scalar constant INIT_TRAJ_INVALID_TIME_STR="Invalid time in trajectory."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_TIME_STR "\111\156\166\141\154\151\144\040\164\151\155\145\040\151\156\040\164\162\141\152\145\143\164\157\162\171\056"
/** Original scalar constant INIT_TRAJ_WRONG_MODE=209; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_WRONG_MODE UINT32_C(209)
/** Original scalar constant INIT_TRAJ_WRONG_MODE_STR="Must call start_traj_mode service"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_WRONG_MODE_STR "\115\165\163\164\040\143\141\154\154\040\163\164\141\162\164\137\164\162\141\152\137\155\157\144\145\040\163\145\162\166\151\143\145"
/** Original scalar constant INIT_TRAJ_BACKWARD_TIME=210; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_BACKWARD_TIME UINT32_C(210)
/** Original scalar constant INIT_TRAJ_BACKWARD_TIME_STR="Trajectory message contains waypoints that are not strictly increasing in time."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_BACKWARD_TIME_STR "\124\162\141\152\145\143\164\157\162\171\040\155\145\163\163\141\147\145\040\143\157\156\164\141\151\156\163\040\167\141\171\160\157\151\156\164\163\040\164\150\141\164\040\141\162\145\040\156\157\164\040\163\164\162\151\143\164\154\171\040\151\156\143\162\145\141\163\151\156\147\040\151\156\040\164\151\155\145\056"
/** Original scalar constant INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS=211; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS UINT32_C(211)
/** Original scalar constant INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS_STR="Trajectory did not contain position data for all axes."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_WRONG_NUMBER_OF_POSITIONS_STR "\124\162\141\152\145\143\164\157\162\171\040\144\151\144\040\156\157\164\040\143\157\156\164\141\151\156\040\160\157\163\151\164\151\157\156\040\144\141\164\141\040\146\157\162\040\141\154\154\040\141\170\145\163\056"
/** Original scalar constant INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES=212; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES UINT32_C(212)
/** Original scalar constant INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES_STR="Trajectory did not contain velocity data for all axes."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_WRONG_NUMBER_OF_VELOCITIES_STR "\124\162\141\152\145\143\164\157\162\171\040\144\151\144\040\156\157\164\040\143\157\156\164\141\151\156\040\166\145\154\157\143\151\164\171\040\144\141\164\141\040\146\157\162\040\141\154\154\040\141\170\145\163\056"
/** Original scalar constant INIT_TRAJ_INVALID_ENDING_VELOCITY=213; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_ENDING_VELOCITY UINT32_C(213)
/** Original scalar constant INIT_TRAJ_INVALID_ENDING_VELOCITY_STR="The final point in the trajectory must have zero velocity."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_ENDING_VELOCITY_STR "\124\150\145\040\146\151\156\141\154\040\160\157\151\156\164\040\151\156\040\164\150\145\040\164\162\141\152\145\143\164\157\162\171\040\155\165\163\164\040\150\141\166\145\040\172\145\162\157\040\166\145\154\157\143\151\164\171\056"
/** Original scalar constant INIT_TRAJ_INVALID_ENDING_ACCELERATION=214; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_ENDING_ACCELERATION UINT32_C(214)
/** Original scalar constant INIT_TRAJ_INVALID_ENDING_ACCELERATION_STR="The final point in the trajectory must have zero acceleration."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_INVALID_ENDING_ACCELERATION_STR "\124\150\145\040\146\151\156\141\154\040\160\157\151\156\164\040\151\156\040\164\150\145\040\164\162\141\152\145\143\164\157\162\171\040\155\165\163\164\040\150\141\166\145\040\172\145\162\157\040\141\143\143\145\154\145\162\141\164\151\157\156\056"
/** Original scalar constant INIT_TRAJ_DUPLICATE_JOINT_NAME=215; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_DUPLICATE_JOINT_NAME UINT32_C(215)
/** Original scalar constant INIT_TRAJ_DUPLICATE_JOINT_NAME_STR="The trajectory contains duplicate joint names."; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_INITTRAJENUM_INIT_TRAJ_DUPLICATE_JOINT_NAME_STR "\124\150\145\040\164\162\141\152\145\143\164\157\162\171\040\143\157\156\164\141\151\156\163\040\144\165\160\154\151\143\141\164\145\040\152\157\151\156\164\040\156\141\155\145\163\056"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_valid(roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->value,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_encode(roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_InitTrajEnum_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/msg/IoResultCodes value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_CODEC_IDENTITY "UWVMROS1_ef9ab829ae79d4e58336a56259b6beb2ad26a8b1b4070013d9004ac4ab269915"
/** Original scalar constant OK=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_OK UINT32_C(0)
/** Original scalar constant OK_STR="Success"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_OK_STR "\123\165\143\143\145\163\163"
/** Original scalar constant READ_ADDRESS_INVALID=1001; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_READ_ADDRESS_INVALID UINT32_C(1001)
/** Original scalar constant READ_ADDRESS_INVALID_STR="Address cannot be read from (out of readable range)"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_READ_ADDRESS_INVALID_STR "\101\144\144\162\145\163\163\040\143\141\156\156\157\164\040\142\145\040\162\145\141\144\040\146\162\157\155\040\050\157\165\164\040\157\146\040\162\145\141\144\141\142\154\145\040\162\141\156\147\145\051"
/** Original scalar constant WRITE_ADDRESS_INVALID=1002; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_WRITE_ADDRESS_INVALID UINT32_C(1002)
/** Original scalar constant WRITE_ADDRESS_INVALID_STR="Address cannot be written to (out of writable range)"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_WRITE_ADDRESS_INVALID_STR "\101\144\144\162\145\163\163\040\143\141\156\156\157\164\040\142\145\040\167\162\151\164\164\145\156\040\164\157\040\050\157\165\164\040\157\146\040\167\162\151\164\141\142\154\145\040\162\141\156\147\145\051"
/** Original scalar constant WRITE_VALUE_INVALID=1003; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_WRITE_VALUE_INVALID UINT32_C(1003)
/** Original scalar constant WRITE_VALUE_INVALID_STR="Illegal value for the type of IO element addressed"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_WRITE_VALUE_INVALID_STR "\111\154\154\145\147\141\154\040\166\141\154\165\145\040\146\157\162\040\164\150\145\040\164\171\160\145\040\157\146\040\111\117\040\145\154\145\155\145\156\164\040\141\144\144\162\145\163\163\145\144"
/** Original scalar constant READ_API_ERROR=1004; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_READ_API_ERROR UINT32_C(1004)
/** Original scalar constant READ_API_ERROR_STR="The MotoPlus function mpReadIO returned -1. No further information is available"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_READ_API_ERROR_STR "\124\150\145\040\115\157\164\157\120\154\165\163\040\146\165\156\143\164\151\157\156\040\155\160\122\145\141\144\111\117\040\162\145\164\165\162\156\145\144\040\055\061\056\040\116\157\040\146\165\162\164\150\145\162\040\151\156\146\157\162\155\141\164\151\157\156\040\151\163\040\141\166\141\151\154\141\142\154\145"
/** Original scalar constant WRITE_API_ERROR=1005; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_WRITE_API_ERROR UINT32_C(1005)
/** Original scalar constant WRITE_API_ERROR_STR="The MotoPlus function mpWriteIO returned -1. No further information is available"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_WRITE_API_ERROR_STR "\124\150\145\040\115\157\164\157\120\154\165\163\040\146\165\156\143\164\151\157\156\040\155\160\127\162\151\164\145\111\117\040\162\145\164\165\162\156\145\144\040\055\061\056\040\116\157\040\146\165\162\164\150\145\162\040\151\156\146\157\162\155\141\164\151\157\156\040\151\163\040\141\166\141\151\154\141\142\154\145"
/** Original scalar constant UNKNOWN_API_ERROR_STR="Unknown error accessing I/O. No further information is available"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_IORESULTCODES_UNKNOWN_API_ERROR_STR "\125\156\153\156\157\167\156\040\145\162\162\157\162\040\141\143\143\145\163\163\151\156\147\040\111\057\117\056\040\116\157\040\146\165\162\164\150\145\162\040\151\156\146\157\162\155\141\164\151\157\156\040\151\163\040\141\166\141\151\154\141\142\154\145"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_valid(roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes* value) {
  return roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_encode(roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_IoResultCodes_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/msg/MotionReadyEnum value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t value;
} roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_CODEC_IDENTITY "UWVMROS1_c8d4c2b0eba665e01cddc730421a3a42c5b344f4de35ae0a32204891603326d6"
/** Original scalar constant READY=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_READY UINT32_C(1)
/** Original scalar constant READY_STR="Ready"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_READY_STR "\122\145\141\144\171"
/** Original scalar constant NOT_READY_UNSPECIFIED=100; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_UNSPECIFIED UINT32_C(100)
/** Original scalar constant NOT_READY_UNSPECIFIED_STR="Unspecified"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_UNSPECIFIED_STR "\125\156\163\160\145\143\151\146\151\145\144"
/** Original scalar constant NOT_READY_ALARM=101; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ALARM UINT32_C(101)
/** Original scalar constant NOT_READY_ALARM_STR="The controller has an active Alarm"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ALARM_STR "\124\150\145\040\143\157\156\164\162\157\154\154\145\162\040\150\141\163\040\141\156\040\141\143\164\151\166\145\040\101\154\141\162\155"
/** Original scalar constant NOT_READY_ERROR=102; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ERROR UINT32_C(102)
/** Original scalar constant NOT_READY_ERROR_STR="The controller has an active Error"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ERROR_STR "\124\150\145\040\143\157\156\164\162\157\154\154\145\162\040\150\141\163\040\141\156\040\141\143\164\151\166\145\040\105\162\162\157\162"
/** Original scalar constant NOT_READY_ESTOP=103; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ESTOP UINT32_C(103)
/** Original scalar constant NOT_READY_ESTOP_STR="The controller is in E-Stop"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ESTOP_STR "\124\150\145\040\143\157\156\164\162\157\154\154\145\162\040\151\163\040\151\156\040\105\055\123\164\157\160"
/** Original scalar constant NOT_READY_NOT_PLAY=104; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_PLAY UINT32_C(104)
/** Original scalar constant NOT_READY_NOT_PLAY_STR="The teach pendant must not be in TEACH mode"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_PLAY_STR "\124\150\145\040\164\145\141\143\150\040\160\145\156\144\141\156\164\040\155\165\163\164\040\156\157\164\040\142\145\040\151\156\040\124\105\101\103\110\040\155\157\144\145"
/** Original scalar constant NOT_READY_NOT_REMOTE=105; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_REMOTE UINT32_C(105)
/** Original scalar constant NOT_READY_NOT_REMOTE_STR="The teach pendant must be in REMOTE mode"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_REMOTE_STR "\124\150\145\040\164\145\141\143\150\040\160\145\156\144\141\156\164\040\155\165\163\164\040\142\145\040\151\156\040\122\105\115\117\124\105\040\155\157\144\145"
/** Original scalar constant NOT_READY_SERVO_OFF=106; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_SERVO_OFF UINT32_C(106)
/** Original scalar constant NOT_READY_SERVO_OFF_STR="Servo power is OFF. Please call start_traj_mode or start_point_queue_mode service"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_SERVO_OFF_STR "\123\145\162\166\157\040\160\157\167\145\162\040\151\163\040\117\106\106\056\040\120\154\145\141\163\145\040\143\141\154\154\040\163\164\141\162\164\137\164\162\141\152\137\155\157\144\145\040\157\162\040\163\164\141\162\164\137\160\157\151\156\164\137\161\165\145\165\145\137\155\157\144\145\040\163\145\162\166\151\143\145"
/** Original scalar constant NOT_READY_HOLD=107; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_HOLD UINT32_C(107)
/** Original scalar constant NOT_READY_HOLD_STR="The controller is in HOLD"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_HOLD_STR "\124\150\145\040\143\157\156\164\162\157\154\154\145\162\040\151\163\040\151\156\040\110\117\114\104"
/** Original scalar constant NOT_READY_JOB_NOT_STARTED=108; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_JOB_NOT_STARTED UINT32_C(108)
/** Original scalar constant NOT_READY_JOB_NOT_STARTED_STR="The INIT_ROS job has not started. Please call start_traj_mode or start_point_queue_mode service"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_JOB_NOT_STARTED_STR "\124\150\145\040\111\116\111\124\137\122\117\123\040\152\157\142\040\150\141\163\040\156\157\164\040\163\164\141\162\164\145\144\056\040\120\154\145\141\163\145\040\143\141\154\154\040\163\164\141\162\164\137\164\162\141\152\137\155\157\144\145\040\157\162\040\163\164\141\162\164\137\160\157\151\156\164\137\161\165\145\165\145\137\155\157\144\145\040\163\145\162\166\151\143\145"
/** Original scalar constant NOT_READY_NOT_ON_WAIT_CMD=109; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_ON_WAIT_CMD UINT32_C(109)
/** Original scalar constant NOT_READY_NOT_ON_WAIT_CMD_STR="INFORM job is not on a WAIT command. Check the format of INIT_ROS"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_ON_WAIT_CMD_STR "\111\116\106\117\122\115\040\152\157\142\040\151\163\040\156\157\164\040\157\156\040\141\040\127\101\111\124\040\143\157\155\155\141\156\144\056\040\103\150\145\143\153\040\164\150\145\040\146\157\162\155\141\164\040\157\146\040\111\116\111\124\137\122\117\123"
/** Original scalar constant NOT_READY_PFL_ACTIVE=111; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_PFL_ACTIVE UINT32_C(111)
/** Original scalar constant NOT_READY_PFL_ACTIVE_STR="A PFL event has occurred. Please return the robot to a safe state"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_PFL_ACTIVE_STR "\101\040\120\106\114\040\145\166\145\156\164\040\150\141\163\040\157\143\143\165\162\162\145\144\056\040\120\154\145\141\163\145\040\162\145\164\165\162\156\040\164\150\145\040\162\157\142\157\164\040\164\157\040\141\040\163\141\146\145\040\163\164\141\164\145"
/** Original scalar constant NOT_READY_INC_MOVE_ERROR=112; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_INC_MOVE_ERROR UINT32_C(112)
/** Original scalar constant NOT_READY_INC_MOVE_ERROR_STR="The controller is in an error state. Please call reset_error to attempt to clear the error"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_INC_MOVE_ERROR_STR "\124\150\145\040\143\157\156\164\162\157\154\154\145\162\040\151\163\040\151\156\040\141\156\040\145\162\162\157\162\040\163\164\141\164\145\056\040\120\154\145\141\163\145\040\143\141\154\154\040\162\145\163\145\164\137\145\162\162\157\162\040\164\157\040\141\164\164\145\155\160\164\040\164\157\040\143\154\145\141\162\040\164\150\145\040\145\162\162\157\162"
/** Original scalar constant NOT_READY_OTHER_PROGRAM_RUNNING=113; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_OTHER_PROGRAM_RUNNING UINT32_C(113)
/** Original scalar constant NOT_READY_OTHER_PROGRAM_RUNNING_STR="Controller is running another job"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_OTHER_PROGRAM_RUNNING_STR "\103\157\156\164\162\157\154\154\145\162\040\151\163\040\162\165\156\156\151\156\147\040\141\156\157\164\150\145\162\040\152\157\142"
/** Original scalar constant NOT_READY_OTHER_TRAJ_MODE_ACTIVE=114; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_OTHER_TRAJ_MODE_ACTIVE UINT32_C(114)
/** Original scalar constant NOT_READY_OTHER_TRAJ_MODE_ACTIVE_STR="Another trajectory mode is already active. Please call stop_traj_mode service"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_OTHER_TRAJ_MODE_ACTIVE_STR "\101\156\157\164\150\145\162\040\164\162\141\152\145\143\164\157\162\171\040\155\157\144\145\040\151\163\040\141\154\162\145\141\144\171\040\141\143\164\151\166\145\056\040\120\154\145\141\163\145\040\143\141\154\154\040\163\164\157\160\137\164\162\141\152\137\155\157\144\145\040\163\145\162\166\151\143\145"
/** Original scalar constant NOT_READY_NOT_CONT_CYCLE_MODE=115; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_CONT_CYCLE_MODE UINT32_C(115)
/** Original scalar constant NOT_READY_NOT_CONT_CYCLE_MODE_STR="AUTO cycle mode not set. Please set the cycle mode to AUTO"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_NOT_CONT_CYCLE_MODE_STR "\101\125\124\117\040\143\171\143\154\145\040\155\157\144\145\040\156\157\164\040\163\145\164\056\040\120\154\145\141\163\145\040\163\145\164\040\164\150\145\040\143\171\143\154\145\040\155\157\144\145\040\164\157\040\101\125\124\117"
/** Original scalar constant NOT_READY_MAJOR_ALARM=116; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_MAJOR_ALARM UINT32_C(116)
/** Original scalar constant NOT_READY_MAJOR_ALARM_STR="Major Alarms can't be reset. Please check the teach pendant"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_MAJOR_ALARM_STR "\115\141\152\157\162\040\101\154\141\162\155\163\040\143\141\156\047\164\040\142\145\040\162\145\163\145\164\056\040\120\154\145\141\163\145\040\143\150\145\143\153\040\164\150\145\040\164\145\141\143\150\040\160\145\156\144\141\156\164"
/** Original scalar constant NOT_READY_ECO_MODE=117; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ECO_MODE UINT32_C(117)
/** Original scalar constant NOT_READY_ECO_MODE_STR="Couldn't disable eco mode"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_ECO_MODE_STR "\103\157\165\154\144\156\047\164\040\144\151\163\141\142\154\145\040\145\143\157\040\155\157\144\145"
/** Original scalar constant NOT_READY_SERVO_ON_TIMEOUT=118; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_SERVO_ON_TIMEOUT UINT32_C(118)
/** Original scalar constant NOT_READY_SERVO_ON_TIMEOUT_STR="Timed out while waiting for servo on"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_MOTIONREADYENUM_NOT_READY_SERVO_ON_TIMEOUT_STR "\124\151\155\145\144\040\157\165\164\040\167\150\151\154\145\040\167\141\151\164\151\156\147\040\146\157\162\040\163\145\162\166\157\040\157\156"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_valid(roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->value,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_encode(roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/msg/QueueResultEnum value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t value;
} roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_CODEC_IDENTITY "UWVMROS1_146c6ce3f275b9e4a81622d73cca3d2298cf9f7bb95a783c0dea6279cb5b4f23"
/** Original scalar constant SUCCESS=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_SUCCESS UINT32_C(1)
/** Original scalar constant SUCCESS_STR=""; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_SUCCESS_STR ""
/** Original scalar constant WRONG_MODE=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_WRONG_MODE UINT32_C(2)
/** Original scalar constant WRONG_MODE_STR="Must call start_point_queue_mode service"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_WRONG_MODE_STR "\115\165\163\164\040\143\141\154\154\040\163\164\141\162\164\137\160\157\151\156\164\137\161\165\145\165\145\137\155\157\144\145\040\163\145\162\166\151\143\145"
/** Original scalar constant INIT_FAILURE=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_INIT_FAILURE UINT32_C(3)
/** Original scalar constant INIT_FAILURE_STR="Failed to initialize the streaming trajectory"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_INIT_FAILURE_STR "\106\141\151\154\145\144\040\164\157\040\151\156\151\164\151\141\154\151\172\145\040\164\150\145\040\163\164\162\145\141\155\151\156\147\040\164\162\141\152\145\143\164\157\162\171"
/** Original scalar constant BUSY=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_BUSY UINT32_C(4)
/** Original scalar constant BUSY_STR="Busy processing the previous trajectory point. Please resend the next point shortly"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_BUSY_STR "\102\165\163\171\040\160\162\157\143\145\163\163\151\156\147\040\164\150\145\040\160\162\145\166\151\157\165\163\040\164\162\141\152\145\143\164\157\162\171\040\160\157\151\156\164\056\040\120\154\145\141\163\145\040\162\145\163\145\156\144\040\164\150\145\040\156\145\170\164\040\160\157\151\156\164\040\163\150\157\162\164\154\171"
/** Original scalar constant INVALID_JOINT_LIST=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_INVALID_JOINT_LIST UINT32_C(5)
/** Original scalar constant INVALID_JOINT_LIST_STR="Point must contain data for all joints"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_INVALID_JOINT_LIST_STR "\120\157\151\156\164\040\155\165\163\164\040\143\157\156\164\141\151\156\040\144\141\164\141\040\146\157\162\040\141\154\154\040\152\157\151\156\164\163"
/** Original scalar constant UNABLE_TO_PROCESS_POINT=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_UNABLE_TO_PROCESS_POINT UINT32_C(6)
/** Original scalar constant UNABLE_TO_PROCESS_POINT_STR="Error while processing the trajectory point. Check debug log for more details"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_QUEUERESULTENUM_UNABLE_TO_PROCESS_POINT_STR "\105\162\162\157\162\040\167\150\151\154\145\040\160\162\157\143\145\163\163\151\156\147\040\164\150\145\040\164\162\141\152\145\143\164\157\162\171\040\160\157\151\156\164\056\040\103\150\145\143\153\040\144\145\142\165\147\040\154\157\147\040\146\157\162\040\155\157\162\145\040\144\145\164\141\151\154\163"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_valid(roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum const* value) {size_t n=0;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum* value) {
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->value,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_encode(roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/msg/SelectionResultCodes value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t value;
} roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_CODEC_IDENTITY "UWVMROS1_46168b1f851163985e75842d9d795c5715160864a7736e7f92f95818c9435602"
/** Original scalar constant OK=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_OK UINT32_C(0)
/** Original scalar constant OK_STR=""; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_OK_STR ""
/** Original scalar constant INVALID_CONTROLLER_STATE=400; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_INVALID_CONTROLLER_STATE UINT32_C(400)
/** Original scalar constant INVALID_CONTROLLER_STATE_STR="Pendant is not in REMOTE mode"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_INVALID_CONTROLLER_STATE_STR "\120\145\156\144\141\156\164\040\151\163\040\156\157\164\040\151\156\040\122\105\115\117\124\105\040\155\157\144\145"
/** Original scalar constant INVALID_CONTROL_GROUP=401; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_INVALID_CONTROL_GROUP UINT32_C(401)
/** Original scalar constant INVALID_CONTROL_GROUP_STR="Invalid control group"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_INVALID_CONTROL_GROUP_STR "\111\156\166\141\154\151\144\040\143\157\156\164\162\157\154\040\147\162\157\165\160"
/** Original scalar constant INVALID_SELECTION_INDEX=402; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_INVALID_SELECTION_INDEX UINT32_C(402)
/** Original scalar constant INVALID_SELECTION_INDEX_STR="Invalid tool selection"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_MSG_SELECTIONRESULTCODES_INVALID_SELECTION_INDEX_STR "\111\156\166\141\154\151\144\040\164\157\157\154\040\163\145\154\145\143\164\151\157\156"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_valid(roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_wire_size(roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_write_fields(roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_encode(roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes* value) {
  roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/GetActiveAlarmInfo_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_REQUEST_CODEC_IDENTITY "UWVMROS1_aadcd63f3bc47424c38767afdc1589561e3c90e8aeae9bdceef88b1033d57a7e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request* value) {
  return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/GetActiveAlarmInfo_Response.result_message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_result_message_text;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo data[16];} roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_alarms_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo data[16];} roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_errors_sequence;
/** Complete ROS2 motoros2_interfaces/srv/GetActiveAlarmInfo_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field result_message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_result_message_text result_message;
  /** Declared field alarms; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_alarms_sequence alarms;
  /** Declared field errors; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_errors_sequence errors;
} roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_RESPONSE_MAX_WIRE_BYTES 32768u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_RESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_RESPONSE_CODEC_IDENTITY "UWVMROS1_e11bdbb95b6ded88d035696260c58dad32bcf3604aa4e89dd41c0dab26541567"
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_RESPONSE_CARTESIAN_MAX_WIRE_BYTES UINT64_C(413008)
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_GETACTIVEALARMINFO_RESPONSE_AGGREGATE_WIRE_BYTES 32768u
/** Shape validation retains every field; aggregate quota is checked separately. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_shape(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response const* value) {if(!value)return 0;
  if(value->result_message.size>512u||!uwvm_bounded_utf8(value->result_message.data,value->result_message.size))return 0;
  if(value->alarms.size>16u)return 0;
  {size_t i;for(i=0;i<value->alarms.size;++i){
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_valid(&value->alarms.data[i]))return 0;
  }}
  if(value->errors.size>16u)return 0;
  {size_t i;for(i=0;i<value->errors.size;++i){
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_valid(&value->errors.data[i]))return 0;
  }}
  return 1;}
/** Checked addition under this record's full admitted wire extent. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(size_t* n,size_t amount) {if(*n>32768u||amount>32768u-*n)return 0;*n+=amount;return 1;}
/** Full validation and measure; quota90 preserves the caller's size sentinel. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_measure(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response const* value,size_t* length) {size_t n=0;
  if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_shape(value)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(&n,4)||!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(&n,value->result_message.size))return UWVM_BOUNDED_TOO_LARGE;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->alarms.size;++i){
  {size_t nested=roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_wire_size(&value->alarms.data[i]);if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(&n,nested))return UWVM_BOUNDED_TOO_LARGE;}
  }}
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(&n,4))return UWVM_BOUNDED_TOO_LARGE;
  {size_t i;for(i=0;i<value->errors.size;++i){
  {size_t nested=roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_wire_size(&value->errors.data[i]);if(!roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_add(&n,nested))return UWVM_BOUNDED_TOO_LARGE;}
  }}
  *length=n;return 0;}
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response const* value) {size_t n=0;return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_measure(value,&n)==0;}
/** Compatibility size view; use measure for an unambiguous error/status. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response const* value) {size_t n=0;return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_measure(value,&n)?0:n;}
/** Internal no-fail writer, entered only after complete measure/overlap proof. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->result_message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->result_message.data,value->result_message.size);*offset+=value->result_message.size;
  uwvm_bounded_put(p+*offset,value->alarms.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->alarms.size;++i){
  roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_write_fields(&value->alarms.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->errors.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->errors.size;++i){
  roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_write_fields(&value->errors.data[i],p,offset);
  }}
}
/** Internal reader/status keeps all intermediate mutations private. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_read_status(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response* value) {size_t measured=0;
  if(!value||!offset||*offset>n||(n&&!p))return UWVM_BOUNDED_INVALID;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->result_message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->result_message.size>512u||value->result_message.size>n-*offset)return UWVM_BOUNDED_INVALID;uwvm_bounded_copy(value->result_message.data,p+*offset,value->result_message.size);*offset+=value->result_message.size;
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->alarms.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->alarms.size>16u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->alarms.size;++i){
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_AlarmInfo_read_fields(p,n,offset,&value->alarms.data[i]))return UWVM_BOUNDED_INVALID;
  }}
  if(*offset>n||n-*offset<4)return UWVM_BOUNDED_INVALID;value->errors.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->errors.size>16u)return UWVM_BOUNDED_INVALID;
  {size_t i;for(i=0;i<value->errors.size;++i){
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_ErrorInfo_read_fields(p,n,offset,&value->errors.data[i]))return UWVM_BOUNDED_INVALID;
  }}
  return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_measure(value,&measured);
}
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response* value) {return roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_read_status(p,n,offset,value)==0;}
/** No writes before full shape, nested quotas, exact size and disjoint destinations. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response const* value,void* output,size_t capacity,size_t* length) {size_t n=0,offset=0;uint32_t error;
  if(!length)return UWVM_BOUNDED_INVALID;error=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_measure(value,&n);if(error)return error;
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;}
/** The compiler may reserve the full temporary stack frame even on preflight failure. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response* value) {size_t offset=0;uint32_t error;
  if(!value||(length&&!input)||length<16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(length>32768u)return UWVM_BOUNDED_TOO_LARGE;
  {roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response next={0};error=roswasmc_bounded_2_motoros2_interfaces_srv_GetActiveAlarmInfo_Response_read_status((unsigned char const*)input,length,&offset,&next);if(error)return error;if(offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;}}
/** Complete ROS2 motoros2_interfaces/srv/ListInformJobs_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_LISTINFORMJOBS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_LISTINFORMJOBS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_LISTINFORMJOBS_REQUEST_CODEC_IDENTITY "UWVMROS1_e77eedadef98d98321b1757b88284022afcdb1ffef52b98820b0c4ba596d643c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request* value) {
  return roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/ListInformJobs_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_message_text;
/** UTF-8 byte storage for motoros2_interfaces/srv/ListInformJobs_Response.names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_names_text;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_names_text data[16];} roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_names_sequence;
/** Complete ROS2 motoros2_interfaces/srv/ListInformJobs_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_message_text message;
  /** Declared field names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_names_sequence names;
} roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_LISTINFORMJOBS_RESPONSE_MAX_WIRE_BYTES 8780u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_LISTINFORMJOBS_RESPONSE_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_LISTINFORMJOBS_RESPONSE_CODEC_IDENTITY "UWVMROS1_49021f0abc00f8df1ef49605a244cb6bba8d8b45ece5365ae35d747d2506eadc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->names.size>16u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(value->names.data[i].size>512u||!uwvm_bounded_utf8(value->names.data[i].data,value->names.data[i].size))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=4;
  if(value->names.size>16u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  n+=4+value->names.data[i].size;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,value->names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->names.size;++i){
  uwvm_bounded_put(p+*offset,value->names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->names.data[i].data,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<4)return 0;value->names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.size>16u)return 0;
  {size_t i;for(i=0;i<value->names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->names.data[i].size>512u||value->names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->names.data[i].data,p+*offset,value->names.data[i].size);*offset+=value->names.data[i].size;
  }}
  return roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>8780u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ListInformJobs_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/QueueTrajPoint_Request.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_joint_names_text;
/** At most12 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_joint_names_text data[12];} roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_joint_names_sequence;
/** Complete ROS2 motoros2_interfaces/srv/QueueTrajPoint_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request {
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_joint_names_sequence joint_names;
  /** Declared field point; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint point;
} roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_QUEUETRAJPOINT_REQUEST_MAX_WIRE_BYTES 1228u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_QUEUETRAJPOINT_REQUEST_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_QUEUETRAJPOINT_REQUEST_CODEC_IDENTITY "UWVMROS1_3d37f9981d6abaca33ec0b5378fa3a1ac301791cc9be94d5bdcf4ea6aa81b8d0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request const* value) {
  if(!value)return 0;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>64u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->point))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request const* value) {size_t n=0;
  n+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->point);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->point,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>12u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>64u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->point))return 0;
  return roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>1228u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/QueueTrajPoint_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/QueueTrajPoint_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_message_text message;
} roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_QUEUETRAJPOINT_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_QUEUETRAJPOINT_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_QUEUETRAJPOINT_RESPONSE_CODEC_IDENTITY "UWVMROS1_d4887ed24d39332a5ccfd4a3d072004dd8f314fe5c166bf0bd57520a826cbccd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_valid(&value->result_code))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_wire_size(&value->result_code);
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_write_fields(&value->result_code,p,offset);
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response* value) {
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_QueueResultEnum_read_fields(p,n,offset,&value->result_code))return 0;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_QueueTrajPoint_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/ReadGroupIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request {
  /** Declared field address; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t address;
} roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READGROUPIO_REQUEST_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READGROUPIO_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READGROUPIO_REQUEST_CODEC_IDENTITY "UWVMROS1_6ee09ac379e169fc23b81a551e9a9ed21c3f82b5010859984e911b17152beb7f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->address,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->address,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/ReadGroupIO_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/ReadGroupIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_message_text message;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READGROUPIO_RESPONSE_MAX_WIRE_BYTES 522u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READGROUPIO_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READGROUPIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_292fcf6e8c46a2509ee4cc5238d443157f37fed0de898367248b967e9001ea0d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>522u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadGroupIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/ReadMRegister_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request {
  /** Declared field address; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t address;
} roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READMREGISTER_REQUEST_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READMREGISTER_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READMREGISTER_REQUEST_CODEC_IDENTITY "UWVMROS1_f31f0983466054f5ac5dcb2ab80ab7721d606a681b833ebb376cc87f96054f6f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->address,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->address,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/ReadMRegister_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/ReadMRegister_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_message_text message;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t value;
} roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READMREGISTER_RESPONSE_MAX_WIRE_BYTES 523u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READMREGISTER_RESPONSE_MIN_WIRE_BYTES 11u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READMREGISTER_RESPONSE_CODEC_IDENTITY "UWVMROS1_8d6ded2876adfa2079711205cb0110a82ead6f51db05f266bf285308cd815a04"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=1;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->value,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<11u||length>523u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadMRegister_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/ReadSingleIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request {
  /** Declared field address; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t address;
} roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READSINGLEIO_REQUEST_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READSINGLEIO_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READSINGLEIO_REQUEST_CODEC_IDENTITY "UWVMROS1_30e28db303b772f26b2b3cf6165a0ff79037d037d60e8115b4ee8e177f17e8bb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request const* value) {size_t n=0;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->address,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->address,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/ReadSingleIO_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/ReadSingleIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_message_text message;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  int32_t value;
} roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READSINGLEIO_RESPONSE_MAX_WIRE_BYTES 525u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READSINGLEIO_RESPONSE_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_READSINGLEIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_b42013571274e85769ffac4d47129de8b76e4a675bdef3efefebc9ee19e92082"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>525u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ReadSingleIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/ResetError_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_RESETERROR_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_RESETERROR_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_RESETERROR_REQUEST_CODEC_IDENTITY "UWVMROS1_2737855d0608478946e2d9ab7f06c92585199c1082ec8406a012f50ad56beae3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request* value) {
  return roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/ResetError_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/ResetError_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_message_text message;
} roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_RESETERROR_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_RESETERROR_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_RESETERROR_RESPONSE_CODEC_IDENTITY "UWVMROS1_0af14ee02c157ecc39b143225a580e475473306389ccc3ba198cf22ca47d82f3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_valid(&value->result_code))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_wire_size(&value->result_code);
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_write_fields(&value->result_code,p,offset);
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response* value) {
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_read_fields(p,n,offset,&value->result_code))return 0;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_ResetError_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/SelectMotionTool_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request {
  /** Declared field group_number; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t group_number;
  /** Declared field tool_number; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t tool_number;
} roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_SELECTMOTIONTOOL_REQUEST_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_SELECTMOTIONTOOL_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_SELECTMOTIONTOOL_REQUEST_CODEC_IDENTITY "UWVMROS1_e6c884ae0a97337c1e441efe099b7869dc95205fcbca0a8082d4d9b06f5b438c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->group_number,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->tool_number,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->group_number,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tool_number,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/SelectMotionTool_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/SelectMotionTool_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_message_text message;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_SELECTMOTIONTOOL_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_SELECTMOTIONTOOL_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_SELECTMOTIONTOOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_088349a49c14c52282c25b428628462c48618c3b5202df560dcc5c9bed742640"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_valid(&value->result_code))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_wire_size(&value->result_code);
  n+=4+value->message.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_write_fields(&value->result_code,p,offset);
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response* value) {
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_SelectionResultCodes_read_fields(p,n,offset,&value->result_code))return 0;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_SelectMotionTool_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/StartPointQueueMode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTPOINTQUEUEMODE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTPOINTQUEUEMODE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTPOINTQUEUEMODE_REQUEST_CODEC_IDENTITY "UWVMROS1_58a4827fc6535ef0ffc64ee25c5c4af75aa0fe199fa0350a88a95d2e991164a1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request* value) {
  return roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/StartPointQueueMode_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/StartPointQueueMode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_message_text message;
} roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTPOINTQUEUEMODE_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTPOINTQUEUEMODE_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTPOINTQUEUEMODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_bc553d5640e1c4644a5d5bdc2dfcefc9dbe789ef1e2a23dd620f08963467b6ff"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_valid(&value->result_code))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_wire_size(&value->result_code);
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_write_fields(&value->result_code,p,offset);
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response* value) {
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_read_fields(p,n,offset,&value->result_code))return 0;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartPointQueueMode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/StartTrajMode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTTRAJMODE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTTRAJMODE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTTRAJMODE_REQUEST_CODEC_IDENTITY "UWVMROS1_0b8b2ad5db5901047035e1070b675618afc516a1ae8c3308cfdcad1174d34789"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request* value) {
  return roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/StartTrajMode_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/StartTrajMode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_message_text message;
} roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTTRAJMODE_RESPONSE_MAX_WIRE_BYTES 518u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTTRAJMODE_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_STARTTRAJMODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_170e25737e6a2b06f5d5d9f331d748c97cf0306284271757953a4a801c722939"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_valid(&value->result_code))return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_wire_size(&value->result_code);
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_write_fields(&value->result_code,p,offset);
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response* value) {
  if(!roswasmc_bounded_2_motoros2_interfaces_msg_MotionReadyEnum_read_fields(p,n,offset,&value->result_code))return 0;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>518u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_StartTrajMode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/WriteGroupIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request {
  /** Declared field address; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t address;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEGROUPIO_REQUEST_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEGROUPIO_REQUEST_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEGROUPIO_REQUEST_CODEC_IDENTITY "UWVMROS1_2d348ed71408a0dd1dd28ee2ed2ca1db56bb07a14546d0fe13ccf64e8553d52c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->address,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->address,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/WriteGroupIO_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/WriteGroupIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_message_text message;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEGROUPIO_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEGROUPIO_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEGROUPIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_06769934711b27fd32cdcbf849504e160189ee82ea32e0e71a2903af47c5d766"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteGroupIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/WriteMRegister_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request {
  /** Declared field address; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t address;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t value;
} roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEMREGISTER_REQUEST_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEMREGISTER_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEMREGISTER_REQUEST_CODEC_IDENTITY "UWVMROS1_8df4536be27f0632052725d5616fd5b047f49fc306fc7735854d9de4fb15d4ef"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request const* value) {size_t n=0;
  n+=4;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->address,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->address,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->value,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/WriteMRegister_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/WriteMRegister_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_message_text message;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEMREGISTER_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEMREGISTER_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITEMREGISTER_RESPONSE_CODEC_IDENTITY "UWVMROS1_a61cc0596a7545ef4836bdc64dc9d6b4ed99b0abd18a726eb37e6215f51c729a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteMRegister_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 motoros2_interfaces/srv/WriteSingleIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request {
  /** Declared field address; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t address;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  int32_t value;
} roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITESINGLEIO_REQUEST_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITESINGLEIO_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITESINGLEIO_REQUEST_CODEC_IDENTITY "UWVMROS1_6361affcd8b7ebaaa1fe6b5806e64a7c18f7a96a1178614cfce487de813a6392"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_valid(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->address,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->address,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_encode(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for motoros2_interfaces/srv/WriteSingleIO_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[512];} roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_message_text;
/** Complete ROS2 motoros2_interfaces/srv/WriteSingleIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response {
  /** Declared field result_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t result_code;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_message_text message;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response;
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITESINGLEIO_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITESINGLEIO_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_MOTOROS2_INTERFACES_SRV_WRITESINGLEIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_12328db372a572fe9b0c7207331a7971d5d7c9033376e3be410db579b85fb794"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_valid(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response const* value) {
  if(!value)return 0;
  if(value->message.size>512u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_wire_size(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response const* value) {size_t n=0;
  n+=4;
  n+=4+value->message.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_write_fields(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->result_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->result_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>512u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_encode(roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response* value) {
  roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_motoros2_interfaces_srv_WriteSingleIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_geometry_msgs_msg_TransformStamped data[16];} roswasmc_bounded_2_tf2_msgs_msg_TFMessage_transforms_sequence;
/** Complete ROS2 tf2_msgs/msg/TFMessage value; no field projects away native data. */
typedef struct roswasmc_bounded_2_tf2_msgs_msg_TFMessage {
  /** Declared field transforms; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_tf2_msgs_msg_TFMessage_transforms_sequence transforms;
} roswasmc_bounded_2_tf2_msgs_msg_TFMessage;
#define ROSWASMC_BOUNDED_2_TF2_MSGS_MSG_TFMESSAGE_MAX_WIRE_BYTES 10372u
#define ROSWASMC_BOUNDED_2_TF2_MSGS_MSG_TFMESSAGE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_TF2_MSGS_MSG_TFMESSAGE_CODEC_IDENTITY "UWVMROS1_4428597a9c2783fd8f0f083a16e8fd1b3d286487f4800c55fcc5571e7f79e65a"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_tf2_msgs_msg_TFMessage_default(roswasmc_bounded_2_tf2_msgs_msg_TFMessage* value) {
  roswasmc_bounded_2_tf2_msgs_msg_TFMessage next={0};if(!value)return UWVM_BOUNDED_INVALID;
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_tf2_msgs_msg_TFMessage_valid(roswasmc_bounded_2_tf2_msgs_msg_TFMessage const* value) {
  if(!value)return 0;
  if(value->transforms.size>16u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_valid(&value->transforms.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_tf2_msgs_msg_TFMessage_wire_size(roswasmc_bounded_2_tf2_msgs_msg_TFMessage const* value) {size_t n=0;
  n+=4;
  if(value->transforms.size>16u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_wire_size(&value->transforms.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_tf2_msgs_msg_TFMessage_write_fields(roswasmc_bounded_2_tf2_msgs_msg_TFMessage const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->transforms.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_write_fields(&value->transforms.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_tf2_msgs_msg_TFMessage_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_tf2_msgs_msg_TFMessage* value) {
  if(*offset>n||n-*offset<4)return 0;value->transforms.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->transforms.size>16u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_read_fields(p,n,offset,&value->transforms.data[i]))return 0;
  }}
  return roswasmc_bounded_2_tf2_msgs_msg_TFMessage_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_tf2_msgs_msg_TFMessage_encode(roswasmc_bounded_2_tf2_msgs_msg_TFMessage const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_tf2_msgs_msg_TFMessage_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_tf2_msgs_msg_TFMessage_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_tf2_msgs_msg_TFMessage_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_tf2_msgs_msg_TFMessage_decode(void const* input,size_t length,roswasmc_bounded_2_tf2_msgs_msg_TFMessage* value) {
  roswasmc_bounded_2_tf2_msgs_msg_TFMessage next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>10372u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_tf2_msgs_msg_TFMessage_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
