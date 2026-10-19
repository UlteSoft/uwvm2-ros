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
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[8];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_positions_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[8];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_velocities_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[8];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_accelerations_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[8];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_effort_sequence;
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
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_MAX_WIRE_BYTES 280u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_CODEC_IDENTITY "UWVMROS1_a512b971211baa8604440e02a046b586e7970293242607002ceb89196c9d99b1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value) {
  if(!value)return 0;
  if(value->positions.size>8u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->positions.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->velocities.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocities.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->accelerations.size>8u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->accelerations.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->effort.size>8u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->effort.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->time_from_start))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value) {size_t n=0;
  n+=4;
  if(value->positions.size>8u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->velocities.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->accelerations.size>8u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->effort.size>8u)return 0;
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
  if(value->positions.size>8u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->positions.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocities.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocities.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocities.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->accelerations.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->accelerations.size>8u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->accelerations.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->effort.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->effort.size>8u)return 0;
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
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>280u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for trajectory_msgs/msg/JointTrajectory.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_text;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_text data[8];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_sequence;
/** At most1 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint data[1];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_points_sequence;
/** Complete ROS2 trajectory_msgs/msg/JointTrajectory value; no field projects away native data. */
typedef struct roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_sequence joint_names;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_points_sequence points;
} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory;
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_MAX_WIRE_BYTES 1484u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_CODEC_IDENTITY "UWVMROS1_e32bd7016b5fefcab6adeb42072e2d56c45d4b9f7cb372bdb5bfac97abf8cb77"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>128u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(value->points.size>1u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->points.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=4;
  if(value->points.size>1u)return 0;
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
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>128u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>1u)return 0;
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
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>1484u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
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
/** At most1 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_geometry_msgs_msg_Transform data[1];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_transforms_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_geometry_msgs_msg_Twist data[8];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_velocities_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_geometry_msgs_msg_Twist data[8];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_accelerations_sequence;
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
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORYPOINT_MAX_WIRE_BYTES 844u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORYPOINT_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORYPOINT_CODEC_IDENTITY "UWVMROS1_4b4b6ba78f11918fcdc138544f7ef1a140d1fc51b385349ab1ad5716472c3daf"
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
  if(value->transforms.size>1u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_valid(&value->transforms.data[i]))return 0;
  }}
  if(value->velocities.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value->velocities.data[i]))return 0;
  }}
  if(value->accelerations.size>8u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value->accelerations.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->time_from_start))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint const* value) {size_t n=0;
  n+=4;
  if(value->transforms.size>1u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_Transform_wire_size(&value->transforms.data[i]);
  }}
  n+=4;
  if(value->velocities.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  n+=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(&value->velocities.data[i]);
  }}
  n+=4;
  if(value->accelerations.size>8u)return 0;
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
  if(value->transforms.size>1u)return 0;
  {size_t i;for(i=0;i<value->transforms.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Transform_read_fields(p,n,offset,&value->transforms.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocities.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocities.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields(p,n,offset,&value->velocities.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->accelerations.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->accelerations.size>8u)return 0;
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
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>844u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for trajectory_msgs/msg/MultiDOFJointTrajectory.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_text;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_text data[8];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_sequence;
/** At most1 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint data[1];} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_points_sequence;
/** Complete ROS2 trajectory_msgs/msg/MultiDOFJointTrajectory value; no field projects away native data. */
typedef struct roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_joint_names_sequence joint_names;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_points_sequence points;
} roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory;
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORY_MAX_WIRE_BYTES 2048u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORY_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_MULTIDOFJOINTTRAJECTORY_CODEC_IDENTITY "UWVMROS1_b822a2d5db9b83e916dcc9db4932952a314773ef53c9fbaf825902e0eb7c8cb5"
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
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>128u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(value->points.size>1u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectoryPoint_valid(&value->points.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=4;
  if(value->points.size>1u)return 0;
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
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>128u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>1u)return 0;
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
  roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>2048u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_MultiDOFJointTrajectory_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/msg/JointTolerance.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_control_msgs_msg_JointTolerance_name_text;
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
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTTOLERANCE_MAX_WIRE_BYTES 156u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTTOLERANCE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTTOLERANCE_CODEC_IDENTITY "UWVMROS1_3864af88029b3b1f51907b3da63d2d821104c29b9469a146a5a34f73f3598b8a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(roswasmc_bounded_2_control_msgs_msg_JointTolerance const* value) {
  if(!value)return 0;
  if(value->name.size>128u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
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
  if(value->name.size>128u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
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
  roswasmc_bounded_2_control_msgs_msg_JointTolerance next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>156u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/msg/JointComponentTolerance.joint_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_joint_name_text;
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
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_MAX_WIRE_BYTES 158u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_MIN_WIRE_BYTES 30u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_MSG_JOINTCOMPONENTTOLERANCE_CODEC_IDENTITY "UWVMROS1_8a10f659b38feae6dfcf89818d35a164743b02c0e5cfe2d86aae7eda56e914fb"
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
  if(value->joint_name.size>128u||!uwvm_bounded_utf8(value->joint_name.data,value->joint_name.size))return 0;
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
  if(value->joint_name.size>128u||value->joint_name.size>n-*offset)return 0;uwvm_bounded_copy(value->joint_name.data,p+*offset,value->joint_name.size);*offset+=value->joint_name.size;
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
  roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance next={0};size_t offset=0;if(!value||(length&&!input)||length<30u||length>158u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointTolerance data[8];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_path_tolerance_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance data[8];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_component_path_tolerance_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointTolerance data[8];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_goal_tolerance_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance data[8];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_component_goal_tolerance_sequence;
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
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_MAX_WIRE_BYTES 8580u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_MIN_WIRE_BYTES 64u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_GOAL_CODEC_IDENTITY "UWVMROS1_fa8334eb129cc91c50febaccfebb371f6fbdd82188d6301823344d6664300018"
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
  if(value->path_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value->path_tolerance.data[i]))return 0;
  }}
  if(value->component_path_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->component_path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_valid(&value->component_path_tolerance.data[i]))return 0;
  }}
  if(value->goal_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value->goal_tolerance.data[i]))return 0;
  }}
  if(value->component_goal_tolerance.size>8u)return 0;
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
  if(value->path_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value->path_tolerance.data[i]);
  }}
  n+=4;
  if(value->component_path_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->component_path_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_wire_size(&value->component_path_tolerance.data[i]);
  }}
  n+=4;
  if(value->goal_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value->goal_tolerance.data[i]);
  }}
  n+=4;
  if(value->component_goal_tolerance.size>8u)return 0;
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
  if(value->path_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields(p,n,offset,&value->path_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->component_path_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->component_path_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->component_path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointComponentTolerance_read_fields(p,n,offset,&value->component_path_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->goal_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->goal_tolerance.size>8u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields(p,n,offset,&value->goal_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->component_goal_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->component_goal_tolerance.size>8u)return 0;
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
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<64u||length>8580u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/action/FollowJointTrajectory_Result.error_string; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_error_string_text;
/** Complete ROS2 control_msgs/action/FollowJointTrajectory_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result {
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  int32_t error_code;
  /** Declared field error_string; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_error_string_text error_string;
} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result;
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_MAX_WIRE_BYTES 264u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_RESULT_CODEC_IDENTITY "UWVMROS1_df1e08076a5c725b865cc8807462d9ccf9577df11cbb71fcc0d280d77d0e5649"
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
  if(value->error_string.size>256u||!uwvm_bounded_utf8(value->error_string.data,value->error_string.size))return 0;
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
  if(value->error_string.size>256u||value->error_string.size>n-*offset)return 0;uwvm_bounded_copy(value->error_string.data,p+*offset,value->error_string.size);*offset+=value->error_string.size;
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
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>264u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for control_msgs/action/FollowJointTrajectory_Feedback.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_joint_names_text;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_joint_names_text data[8];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_joint_names_sequence;
/** UTF-8 byte storage for control_msgs/action/FollowJointTrajectory_Feedback.multi_dof_joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_multi_dof_joint_names_text;
/** At most1 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_multi_dof_joint_names_text data[1];} roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_multi_dof_joint_names_sequence;
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
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_MAX_WIRE_BYTES 4708u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_MIN_WIRE_BYTES 152u
#define ROSWASMC_BOUNDED_2_CONTROL_MSGS_ACTION_FOLLOWJOINTTRAJECTORY_FEEDBACK_CODEC_IDENTITY "UWVMROS1_ae2175027c3e45997d272b9610d15ef52cc665d23c0a371892d10b3d100e3529"
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
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>128u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->error))return 0;
  if(value->multi_dof_joint_names.size>1u)return 0;
  {size_t i;for(i=0;i<value->multi_dof_joint_names.size;++i){
  if(value->multi_dof_joint_names.data[i].size>128u||!uwvm_bounded_utf8(value->multi_dof_joint_names.data[i].data,value->multi_dof_joint_names.data[i].size))return 0;
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
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->desired);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->actual);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->error);
  n+=4;
  if(value->multi_dof_joint_names.size>1u)return 0;
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
  if(value->joint_names.size>8u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>128u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->error))return 0;
  if(*offset>n||n-*offset<4)return 0;value->multi_dof_joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->multi_dof_joint_names.size>1u)return 0;
  {size_t i;for(i=0;i<value->multi_dof_joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->multi_dof_joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->multi_dof_joint_names.data[i].size>128u||value->multi_dof_joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->multi_dof_joint_names.data[i].data,p+*offset,value->multi_dof_joint_names.data[i].size);*offset+=value->multi_dof_joint_names.data[i].size;
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
  roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<152u||length>4708u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_control_msgs_action_FollowJointTrajectory_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for sensor_msgs/msg/JointState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text data[8];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[8];} roswasmc_bounded_2_sensor_msgs_msg_JointState_position_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[8];} roswasmc_bounded_2_sensor_msgs_msg_JointState_velocity_sequence;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[8];} roswasmc_bounded_2_sensor_msgs_msg_JointState_effort_sequence;
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
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MAX_WIRE_BYTES 1404u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_CODEC_IDENTITY "UWVMROS1_bba20c36db70a98b594ba603d05b8a979038fb95b701221ba3b8689f55b06b11"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(roswasmc_bounded_2_sensor_msgs_msg_JointState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->name.size>8u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(value->name.data[i].size>128u||!uwvm_bounded_utf8(value->name.data[i].data,value->name.data[i].size))return 0;
  }}
  if(value->position.size>8u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->velocity.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->effort.size>8u)return 0;
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
  if(value->name.size>8u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  n+=4+value->name.data[i].size;
  }}
  n+=4;
  if(value->position.size>8u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->velocity.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->effort.size>8u)return 0;
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
  if(value->name.size>8u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->name.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.data[i].size>128u||value->name.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->name.data[i].data,p+*offset,value->name.data[i].size);*offset+=value->name.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->position.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->position.size>8u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocity.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocity.size>8u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->effort.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->effort.size>8u)return 0;
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
  roswasmc_bounded_2_sensor_msgs_msg_JointState next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>1404u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
