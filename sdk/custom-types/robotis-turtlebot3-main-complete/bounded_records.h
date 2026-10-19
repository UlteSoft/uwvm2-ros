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
/** Complete ROS2 geometry_msgs/msg/PoseWithCovariance value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance {
  /** Declared field pose; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Pose pose;
  /** Declared field covariance; canonical capacity is part of CODEC_IDENTITY. */
  double covariance[36];
} roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSEWITHCOVARIANCE_MAX_WIRE_BYTES 344u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSEWITHCOVARIANCE_MIN_WIRE_BYTES 344u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_POSEWITHCOVARIANCE_CODEC_IDENTITY "UWVMROS1_8eb3e25c45769eb29691011c05315ed50f2b97eff488f9e8ee2dc70bddab4dc2"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_default(roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance* value) {
  roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_Pose_default(&next.pose);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_valid(roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Pose_valid(&value->pose))return 0;
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_wire_size(roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Pose_wire_size(&value->pose);
  {size_t i;for(i=0;i<36u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_write_fields(roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Pose_write_fields(&value->pose,p,offset);
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Pose_read_fields(p,n,offset,&value->pose))return 0;
  {size_t i;for(i=0;i<36u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_encode(roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance* value) {
  roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance next={0};size_t offset=0;if(!value||(length&&!input)||length<344u||length>344u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** UTF-8 byte storage for geometry_msgs/msg/TransformStamped.child_frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_child_frame_id_text;
/** Complete ROS2 geometry_msgs/msg/TransformStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_TransformStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field child_frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_child_frame_id_text child_frame_id;
  /** Declared field transform; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Transform transform;
} roswasmc_bounded_2_geometry_msgs_msg_TransformStamped;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORMSTAMPED_MAX_WIRE_BYTES 456u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORMSTAMPED_MIN_WIRE_BYTES 72u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TRANSFORMSTAMPED_CODEC_IDENTITY "UWVMROS1_1cb6033612e0632fe87e6cf44bf5a899220370c372ba25b989a066171925ee7a"
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
  if(value->child_frame_id.size>256u||!uwvm_bounded_utf8(value->child_frame_id.data,value->child_frame_id.size))return 0;
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
  if(value->child_frame_id.size>256u||value->child_frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->child_frame_id.data,p+*offset,value->child_frame_id.size);*offset+=value->child_frame_id.size;
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
  roswasmc_bounded_2_geometry_msgs_msg_TransformStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<72u||length>456u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TransformStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS2 geometry_msgs/msg/TwistWithCovariance value; no field projects away native data. */
typedef struct roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance {
  /** Declared field twist; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Twist twist;
  /** Declared field covariance; canonical capacity is part of CODEC_IDENTITY. */
  double covariance[36];
} roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance;
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWISTWITHCOVARIANCE_MAX_WIRE_BYTES 336u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWISTWITHCOVARIANCE_MIN_WIRE_BYTES 336u
#define ROSWASMC_BOUNDED_2_GEOMETRY_MSGS_MSG_TWISTWITHCOVARIANCE_CODEC_IDENTITY "UWVMROS1_96c9fddad7b7c240e850d8c27ddb5f8343764cac791ef9e210196f7941aaf404"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_valid(roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value->twist))return 0;
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_wire_size(roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(&value->twist);
  {size_t i;for(i=0;i<36u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_write_fields(roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Twist_write_fields(&value->twist,p,offset);
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields(p,n,offset,&value->twist))return 0;
  {size_t i;for(i=0;i<36u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_encode(roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_decode(void const* input,size_t length,roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance* value) {
  roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance next={0};size_t offset=0;if(!value||(length&&!input)||length<336u||length>336u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for nav_msgs/msg/Odometry.child_frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_nav_msgs_msg_Odometry_child_frame_id_text;
/** Complete ROS2 nav_msgs/msg/Odometry value; no field projects away native data. */
typedef struct roswasmc_bounded_2_nav_msgs_msg_Odometry {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field child_frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_nav_msgs_msg_Odometry_child_frame_id_text child_frame_id;
  /** Declared field pose; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance pose;
  /** Declared field twist; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance twist;
} roswasmc_bounded_2_nav_msgs_msg_Odometry;
#define ROSWASMC_BOUNDED_2_NAV_MSGS_MSG_ODOMETRY_MAX_WIRE_BYTES 1080u
#define ROSWASMC_BOUNDED_2_NAV_MSGS_MSG_ODOMETRY_MIN_WIRE_BYTES 696u
#define ROSWASMC_BOUNDED_2_NAV_MSGS_MSG_ODOMETRY_CODEC_IDENTITY "UWVMROS1_4eb9aa4edb3d288080deaae0326c26a75923e69d40c9b10219baf6d441ea8111"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_nav_msgs_msg_Odometry_default(roswasmc_bounded_2_nav_msgs_msg_Odometry* value) {
  roswasmc_bounded_2_nav_msgs_msg_Odometry next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_default(&next.pose);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_nav_msgs_msg_Odometry_valid(roswasmc_bounded_2_nav_msgs_msg_Odometry const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->child_frame_id.size>256u||!uwvm_bounded_utf8(value->child_frame_id.data,value->child_frame_id.size))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_valid(&value->pose))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_valid(&value->twist))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_nav_msgs_msg_Odometry_wire_size(roswasmc_bounded_2_nav_msgs_msg_Odometry const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4+value->child_frame_id.size;
  n+=roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_wire_size(&value->pose);
  n+=roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_wire_size(&value->twist);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_nav_msgs_msg_Odometry_write_fields(roswasmc_bounded_2_nav_msgs_msg_Odometry const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->child_frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->child_frame_id.data,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_write_fields(&value->pose,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_write_fields(&value->twist,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_nav_msgs_msg_Odometry_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_nav_msgs_msg_Odometry* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->child_frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->child_frame_id.size>256u||value->child_frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->child_frame_id.data,p+*offset,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseWithCovariance_read_fields(p,n,offset,&value->pose))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_TwistWithCovariance_read_fields(p,n,offset,&value->twist))return 0;
  return roswasmc_bounded_2_nav_msgs_msg_Odometry_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_nav_msgs_msg_Odometry_encode(roswasmc_bounded_2_nav_msgs_msg_Odometry const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_nav_msgs_msg_Odometry_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_nav_msgs_msg_Odometry_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_nav_msgs_msg_Odometry_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_nav_msgs_msg_Odometry_decode(void const* input,size_t length,roswasmc_bounded_2_nav_msgs_msg_Odometry* value) {
  roswasmc_bounded_2_nav_msgs_msg_Odometry next={0};size_t offset=0;if(!value||(length&&!input)||length<696u||length>1080u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_nav_msgs_msg_Odometry_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[16];} roswasmc_bounded_2_sensor_msgs_msg_BatteryState_cell_voltage_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[16];} roswasmc_bounded_2_sensor_msgs_msg_BatteryState_cell_temperature_sequence;
/** UTF-8 byte storage for sensor_msgs/msg/BatteryState.location; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_sensor_msgs_msg_BatteryState_location_text;
/** UTF-8 byte storage for sensor_msgs/msg/BatteryState.serial_number; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_sensor_msgs_msg_BatteryState_serial_number_text;
/** Complete ROS2 sensor_msgs/msg/BatteryState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_sensor_msgs_msg_BatteryState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field voltage; canonical capacity is part of CODEC_IDENTITY. */
  float voltage;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  float temperature;
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  float current;
  /** Declared field charge; canonical capacity is part of CODEC_IDENTITY. */
  float charge;
  /** Declared field capacity; canonical capacity is part of CODEC_IDENTITY. */
  float capacity;
  /** Declared field design_capacity; canonical capacity is part of CODEC_IDENTITY. */
  float design_capacity;
  /** Declared field percentage; canonical capacity is part of CODEC_IDENTITY. */
  float percentage;
  /** Declared field power_supply_status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t power_supply_status;
  /** Declared field power_supply_health; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t power_supply_health;
  /** Declared field power_supply_technology; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t power_supply_technology;
  /** Declared field present; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t present;
  /** Declared field cell_voltage; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_BatteryState_cell_voltage_sequence cell_voltage;
  /** Declared field cell_temperature; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_BatteryState_cell_temperature_sequence cell_temperature;
  /** Declared field location; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_BatteryState_location_text location;
  /** Declared field serial_number; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_BatteryState_serial_number_text serial_number;
} roswasmc_bounded_2_sensor_msgs_msg_BatteryState;
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_MAX_WIRE_BYTES 828u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_MIN_WIRE_BYTES 60u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_CODEC_IDENTITY "UWVMROS1_9083c57dde00bba912db4dd736027cb92bbed2b1a79f965570dee86e1ff5a96e"
/** Original scalar constant POWER_SUPPLY_STATUS_UNKNOWN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_STATUS_UNKNOWN UINT32_C(0)
/** Original scalar constant POWER_SUPPLY_STATUS_CHARGING=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_STATUS_CHARGING UINT32_C(1)
/** Original scalar constant POWER_SUPPLY_STATUS_DISCHARGING=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_STATUS_DISCHARGING UINT32_C(2)
/** Original scalar constant POWER_SUPPLY_STATUS_NOT_CHARGING=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_STATUS_NOT_CHARGING UINT32_C(3)
/** Original scalar constant POWER_SUPPLY_STATUS_FULL=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_STATUS_FULL UINT32_C(4)
/** Original scalar constant POWER_SUPPLY_HEALTH_UNKNOWN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_UNKNOWN UINT32_C(0)
/** Original scalar constant POWER_SUPPLY_HEALTH_GOOD=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_GOOD UINT32_C(1)
/** Original scalar constant POWER_SUPPLY_HEALTH_OVERHEAT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_OVERHEAT UINT32_C(2)
/** Original scalar constant POWER_SUPPLY_HEALTH_DEAD=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_DEAD UINT32_C(3)
/** Original scalar constant POWER_SUPPLY_HEALTH_OVERVOLTAGE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_OVERVOLTAGE UINT32_C(4)
/** Original scalar constant POWER_SUPPLY_HEALTH_UNSPEC_FAILURE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_UNSPEC_FAILURE UINT32_C(5)
/** Original scalar constant POWER_SUPPLY_HEALTH_COLD=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_COLD UINT32_C(6)
/** Original scalar constant POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE UINT32_C(7)
/** Original scalar constant POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE UINT32_C(8)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_UNKNOWN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_UNKNOWN UINT32_C(0)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_NIMH=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_NIMH UINT32_C(1)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LION=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LION UINT32_C(2)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LIPO=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LIPO UINT32_C(3)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LIFE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LIFE UINT32_C(4)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_NICD=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_NICD UINT32_C(5)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LIMN=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LIMN UINT32_C(6)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_TERNARY=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_TERNARY UINT32_C(7)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_VRLA=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_VRLA UINT32_C(8)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_BatteryState_valid(roswasmc_bounded_2_sensor_msgs_msg_BatteryState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->voltage,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->charge,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->capacity,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->design_capacity,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->percentage,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->present>1)return 0;
  if(value->cell_voltage.size>16u)return 0;
  {size_t i;for(i=0;i<value->cell_voltage.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cell_voltage.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->cell_temperature.size>16u)return 0;
  {size_t i;for(i=0;i<value->cell_temperature.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cell_temperature.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->location.size>256u||!uwvm_bounded_utf8(value->location.data,value->location.size))return 0;
  if(value->serial_number.size>256u||!uwvm_bounded_utf8(value->serial_number.data,value->serial_number.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_sensor_msgs_msg_BatteryState_wire_size(roswasmc_bounded_2_sensor_msgs_msg_BatteryState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=4;
  if(value->cell_voltage.size>16u)return 0;
  {size_t i;for(i=0;i<value->cell_voltage.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->cell_temperature.size>16u)return 0;
  {size_t i;for(i=0;i<value->cell_temperature.size;++i){
  n+=4;
  }}
  n+=4+value->location.size;
  n+=4+value->serial_number.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_sensor_msgs_msg_BatteryState_write_fields(roswasmc_bounded_2_sensor_msgs_msg_BatteryState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->voltage,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->charge,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->capacity,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->design_capacity,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->percentage,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->power_supply_status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->power_supply_health,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->power_supply_technology,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->present,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->cell_voltage.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->cell_voltage.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cell_voltage.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->cell_temperature.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->cell_temperature.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cell_temperature.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->location.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->location.data,value->location.size);*offset+=value->location.size;
  uwvm_bounded_put(p+*offset,value->serial_number.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->serial_number.data,value->serial_number.size);*offset+=value->serial_number.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_BatteryState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_sensor_msgs_msg_BatteryState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->voltage,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->charge,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->capacity,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->design_capacity,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->percentage,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->power_supply_status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->power_supply_health,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->power_supply_technology,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->present,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->cell_voltage.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->cell_voltage.size>16u)return 0;
  {size_t i;for(i=0;i<value->cell_voltage.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cell_voltage.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->cell_temperature.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->cell_temperature.size>16u)return 0;
  {size_t i;for(i=0;i<value->cell_temperature.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cell_temperature.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->location.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->location.size>256u||value->location.size>n-*offset)return 0;uwvm_bounded_copy(value->location.data,p+*offset,value->location.size);*offset+=value->location.size;
  if(*offset>n||n-*offset<4)return 0;value->serial_number.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->serial_number.size>256u||value->serial_number.size>n-*offset)return 0;uwvm_bounded_copy(value->serial_number.data,p+*offset,value->serial_number.size);*offset+=value->serial_number.size;
  return roswasmc_bounded_2_sensor_msgs_msg_BatteryState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_BatteryState_encode(roswasmc_bounded_2_sensor_msgs_msg_BatteryState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_sensor_msgs_msg_BatteryState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_sensor_msgs_msg_BatteryState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_sensor_msgs_msg_BatteryState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_BatteryState_decode(void const* input,size_t length,roswasmc_bounded_2_sensor_msgs_msg_BatteryState* value) {
  roswasmc_bounded_2_sensor_msgs_msg_BatteryState next={0};size_t offset=0;if(!value||(length&&!input)||length<60u||length>828u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_BatteryState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 sensor_msgs/msg/Imu value; no field projects away native data. */
typedef struct roswasmc_bounded_2_sensor_msgs_msg_Imu {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field orientation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion orientation;
  /** Declared field orientation_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double orientation_covariance[9];
  /** Declared field angular_velocity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 angular_velocity;
  /** Declared field angular_velocity_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double angular_velocity_covariance[9];
  /** Declared field linear_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 linear_acceleration;
  /** Declared field linear_acceleration_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double linear_acceleration_covariance[9];
} roswasmc_bounded_2_sensor_msgs_msg_Imu;
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_IMU_MAX_WIRE_BYTES 436u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_IMU_MIN_WIRE_BYTES 308u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_IMU_CODEC_IDENTITY "UWVMROS1_e38d1c936a2a4a870b2f069af8d1796f69e6ce7b8cae0a792925ed2f6c0cae1b"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_Imu_default(roswasmc_bounded_2_sensor_msgs_msg_Imu* value) {
  roswasmc_bounded_2_sensor_msgs_msg_Imu next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&next.orientation);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_Imu_valid(roswasmc_bounded_2_sensor_msgs_msg_Imu const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_valid(&value->orientation))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->orientation_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->angular_velocity))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->angular_velocity_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->linear_acceleration))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->linear_acceleration_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_sensor_msgs_msg_Imu_wire_size(roswasmc_bounded_2_sensor_msgs_msg_Imu const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Quaternion_wire_size(&value->orientation);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->angular_velocity);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->linear_acceleration);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_sensor_msgs_msg_Imu_write_fields(roswasmc_bounded_2_sensor_msgs_msg_Imu const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Quaternion_write_fields(&value->orientation,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->orientation_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->angular_velocity,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->angular_velocity_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->linear_acceleration,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->linear_acceleration_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_Imu_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_sensor_msgs_msg_Imu* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Quaternion_read_fields(p,n,offset,&value->orientation))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->orientation_covariance[i],&bits,8);}*offset+=8;
  }}
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->angular_velocity))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->angular_velocity_covariance[i],&bits,8);}*offset+=8;
  }}
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->linear_acceleration))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->linear_acceleration_covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_sensor_msgs_msg_Imu_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_Imu_encode(roswasmc_bounded_2_sensor_msgs_msg_Imu const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_sensor_msgs_msg_Imu_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_sensor_msgs_msg_Imu_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_sensor_msgs_msg_Imu_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_Imu_decode(void const* input,size_t length,roswasmc_bounded_2_sensor_msgs_msg_Imu* value) {
  roswasmc_bounded_2_sensor_msgs_msg_Imu next={0};size_t offset=0;if(!value||(length&&!input)||length<308u||length>436u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_Imu_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for sensor_msgs/msg/JointState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_sensor_msgs_msg_JointState_name_text data[2];} roswasmc_bounded_2_sensor_msgs_msg_JointState_name_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[2];} roswasmc_bounded_2_sensor_msgs_msg_JointState_position_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[2];} roswasmc_bounded_2_sensor_msgs_msg_JointState_velocity_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[2];} roswasmc_bounded_2_sensor_msgs_msg_JointState_effort_sequence;
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
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MAX_WIRE_BYTES 468u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_JOINTSTATE_CODEC_IDENTITY "UWVMROS1_5c4d1ef0128c748e20f6b30c9c3bb5ef6e3c529a0b05b81764ac4774ce765bcf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_JointState_valid(roswasmc_bounded_2_sensor_msgs_msg_JointState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->name.size>2u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(value->name.data[i].size>128u||!uwvm_bounded_utf8(value->name.data[i].data,value->name.data[i].size))return 0;
  }}
  if(value->position.size>2u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->position.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->velocity.size>2u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocity.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->effort.size>2u)return 0;
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
  if(value->name.size>2u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  n+=4+value->name.data[i].size;
  }}
  n+=4;
  if(value->position.size>2u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->velocity.size>2u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->effort.size>2u)return 0;
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
  if(value->name.size>2u)return 0;
  {size_t i;for(i=0;i<value->name.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->name.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.data[i].size>128u||value->name.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->name.data[i].data,p+*offset,value->name.data[i].size);*offset+=value->name.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->position.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->position.size>2u)return 0;
  {size_t i;for(i=0;i<value->position.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->position.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocity.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocity.size>2u)return 0;
  {size_t i;for(i=0;i<value->velocity.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocity.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->effort.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->effort.size>2u)return 0;
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
  roswasmc_bounded_2_sensor_msgs_msg_JointState next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>468u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_JointState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[1024];} roswasmc_bounded_2_sensor_msgs_msg_LaserScan_ranges_sequence;
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[1024];} roswasmc_bounded_2_sensor_msgs_msg_LaserScan_intensities_sequence;
/** Complete ROS2 sensor_msgs/msg/LaserScan value; no field projects away native data. */
typedef struct roswasmc_bounded_2_sensor_msgs_msg_LaserScan {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field angle_min; canonical capacity is part of CODEC_IDENTITY. */
  float angle_min;
  /** Declared field angle_max; canonical capacity is part of CODEC_IDENTITY. */
  float angle_max;
  /** Declared field angle_increment; canonical capacity is part of CODEC_IDENTITY. */
  float angle_increment;
  /** Declared field time_increment; canonical capacity is part of CODEC_IDENTITY. */
  float time_increment;
  /** Declared field scan_time; canonical capacity is part of CODEC_IDENTITY. */
  float scan_time;
  /** Declared field range_min; canonical capacity is part of CODEC_IDENTITY. */
  float range_min;
  /** Declared field range_max; canonical capacity is part of CODEC_IDENTITY. */
  float range_max;
  /** Declared field ranges; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_LaserScan_ranges_sequence ranges;
  /** Declared field intensities; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_sensor_msgs_msg_LaserScan_intensities_sequence intensities;
} roswasmc_bounded_2_sensor_msgs_msg_LaserScan;
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_LASERSCAN_MAX_WIRE_BYTES 8368u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_LASERSCAN_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_LASERSCAN_CODEC_IDENTITY "UWVMROS1_89faa8e14fc7f79a62a576d35c0bb377caf0cbaef6a6f181a5af2da0ead6cce4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_LaserScan_valid(roswasmc_bounded_2_sensor_msgs_msg_LaserScan const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->angle_min,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->angle_max,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->angle_increment,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->time_increment,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->scan_time,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->range_min,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->range_max,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->ranges.size>1024u)return 0;
  {size_t i;for(i=0;i<value->ranges.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ranges.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->intensities.size>1024u)return 0;
  {size_t i;for(i=0;i<value->intensities.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->intensities.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_sensor_msgs_msg_LaserScan_wire_size(roswasmc_bounded_2_sensor_msgs_msg_LaserScan const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  if(value->ranges.size>1024u)return 0;
  {size_t i;for(i=0;i<value->ranges.size;++i){
  n+=4;
  }}
  n+=4;
  if(value->intensities.size>1024u)return 0;
  {size_t i;for(i=0;i<value->intensities.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_sensor_msgs_msg_LaserScan_write_fields(roswasmc_bounded_2_sensor_msgs_msg_LaserScan const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->angle_min,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->angle_max,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->angle_increment,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->time_increment,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->scan_time,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->range_min,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->range_max,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,value->ranges.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->ranges.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ranges.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->intensities.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->intensities.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->intensities.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_LaserScan_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_sensor_msgs_msg_LaserScan* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->angle_min,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->angle_max,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->angle_increment,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->time_increment,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->scan_time,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->range_min,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->range_max,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->ranges.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->ranges.size>1024u)return 0;
  {size_t i;for(i=0;i<value->ranges.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ranges.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->intensities.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->intensities.size>1024u)return 0;
  {size_t i;for(i=0;i<value->intensities.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->intensities.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_sensor_msgs_msg_LaserScan_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_LaserScan_encode(roswasmc_bounded_2_sensor_msgs_msg_LaserScan const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_sensor_msgs_msg_LaserScan_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_sensor_msgs_msg_LaserScan_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_sensor_msgs_msg_LaserScan_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_LaserScan_decode(void const* input,size_t length,roswasmc_bounded_2_sensor_msgs_msg_LaserScan* value) {
  roswasmc_bounded_2_sensor_msgs_msg_LaserScan next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>8368u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_LaserScan_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 sensor_msgs/msg/MagneticField value; no field projects away native data. */
typedef struct roswasmc_bounded_2_sensor_msgs_msg_MagneticField {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field magnetic_field; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 magnetic_field;
  /** Declared field magnetic_field_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double magnetic_field_covariance[9];
} roswasmc_bounded_2_sensor_msgs_msg_MagneticField;
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_MAGNETICFIELD_MAX_WIRE_BYTES 236u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_MAGNETICFIELD_MIN_WIRE_BYTES 108u
#define ROSWASMC_BOUNDED_2_SENSOR_MSGS_MSG_MAGNETICFIELD_CODEC_IDENTITY "UWVMROS1_56ae1437e0fbe7a6722ecf4795483d15f4c616e2e891372c30fa78f18be2cf8b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_MagneticField_valid(roswasmc_bounded_2_sensor_msgs_msg_MagneticField const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->magnetic_field))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->magnetic_field_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_sensor_msgs_msg_MagneticField_wire_size(roswasmc_bounded_2_sensor_msgs_msg_MagneticField const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->magnetic_field);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_sensor_msgs_msg_MagneticField_write_fields(roswasmc_bounded_2_sensor_msgs_msg_MagneticField const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->magnetic_field,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->magnetic_field_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_sensor_msgs_msg_MagneticField_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_sensor_msgs_msg_MagneticField* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->magnetic_field))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->magnetic_field_covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_sensor_msgs_msg_MagneticField_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_MagneticField_encode(roswasmc_bounded_2_sensor_msgs_msg_MagneticField const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_sensor_msgs_msg_MagneticField_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_sensor_msgs_msg_MagneticField_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_sensor_msgs_msg_MagneticField_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_sensor_msgs_msg_MagneticField_decode(void const* input,size_t length,roswasmc_bounded_2_sensor_msgs_msg_MagneticField* value) {
  roswasmc_bounded_2_sensor_msgs_msg_MagneticField next={0};size_t offset=0;if(!value||(length&&!input)||length<108u||length>236u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_sensor_msgs_msg_MagneticField_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 std_srvs/srv/SetBool_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_std_srvs_srv_SetBool_Request {
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t data;
} roswasmc_bounded_2_std_srvs_srv_SetBool_Request;
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_SETBOOL_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_SETBOOL_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_SETBOOL_REQUEST_CODEC_IDENTITY "UWVMROS1_2523e7915bb78fe01b68f2a856611b32a88f624a6e197a6ff312b613e8c65910"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_std_srvs_srv_SetBool_Request_valid(roswasmc_bounded_2_std_srvs_srv_SetBool_Request const* value) {
  if(!value)return 0;
  if(value->data>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_std_srvs_srv_SetBool_Request_wire_size(roswasmc_bounded_2_std_srvs_srv_SetBool_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_std_srvs_srv_SetBool_Request_write_fields(roswasmc_bounded_2_std_srvs_srv_SetBool_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->data,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_std_srvs_srv_SetBool_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_std_srvs_srv_SetBool_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->data,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_std_srvs_srv_SetBool_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_SetBool_Request_encode(roswasmc_bounded_2_std_srvs_srv_SetBool_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_std_srvs_srv_SetBool_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_std_srvs_srv_SetBool_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_std_srvs_srv_SetBool_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_SetBool_Request_decode(void const* input,size_t length,roswasmc_bounded_2_std_srvs_srv_SetBool_Request* value) {
  roswasmc_bounded_2_std_srvs_srv_SetBool_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_std_srvs_srv_SetBool_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for std_srvs/srv/SetBool_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_std_srvs_srv_SetBool_Response_message_text;
/** Complete ROS2 std_srvs/srv/SetBool_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_std_srvs_srv_SetBool_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_srvs_srv_SetBool_Response_message_text message;
} roswasmc_bounded_2_std_srvs_srv_SetBool_Response;
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_SETBOOL_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_SETBOOL_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_SETBOOL_RESPONSE_CODEC_IDENTITY "UWVMROS1_f2d46a8440538094221a151716d8ec44db02455f59845c50aaf4e7964c27ebe4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_std_srvs_srv_SetBool_Response_valid(roswasmc_bounded_2_std_srvs_srv_SetBool_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_std_srvs_srv_SetBool_Response_wire_size(roswasmc_bounded_2_std_srvs_srv_SetBool_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_std_srvs_srv_SetBool_Response_write_fields(roswasmc_bounded_2_std_srvs_srv_SetBool_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_std_srvs_srv_SetBool_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_std_srvs_srv_SetBool_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_std_srvs_srv_SetBool_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_SetBool_Response_encode(roswasmc_bounded_2_std_srvs_srv_SetBool_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_std_srvs_srv_SetBool_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_std_srvs_srv_SetBool_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_std_srvs_srv_SetBool_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_std_srvs_srv_SetBool_Response_decode(void const* input,size_t length,roswasmc_bounded_2_std_srvs_srv_SetBool_Response* value) {
  roswasmc_bounded_2_std_srvs_srv_SetBool_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_std_srvs_srv_SetBool_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_std_srvs_srv_Trigger_Response_message_text;
/** Complete ROS2 std_srvs/srv/Trigger_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_std_srvs_srv_Trigger_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_srvs_srv_Trigger_Response_message_text message;
} roswasmc_bounded_2_std_srvs_srv_Trigger_Response;
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_STD_SRVS_SRV_TRIGGER_RESPONSE_CODEC_IDENTITY "UWVMROS1_58806c592727ad1011b32edb6bca3ca32d8e5dbe7f2fa20c86220daf64a10392"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_std_srvs_srv_Trigger_Response_valid(roswasmc_bounded_2_std_srvs_srv_Trigger_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
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
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
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
  roswasmc_bounded_2_std_srvs_srv_Trigger_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_std_srvs_srv_Trigger_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 turtlebot3_msgs/action/Patrol_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal {
  /** Declared field goal; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 goal;
} roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_GOAL_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_GOAL_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_GOAL_CODEC_IDENTITY "UWVMROS1_929abdbc422698464043e7e8d12c2a5edd418dcd52bbde4a542b33e6862ba96f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_valid(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->goal))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_wire_size(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->goal);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_write_fields(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->goal,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->goal))return 0;
  return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_encode(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal* value) {
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for turtlebot3_msgs/action/Patrol_Result.result; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_result_text;
/** Complete ROS2 turtlebot3_msgs/action/Patrol_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_result_text result;
} roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_RESULT_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_RESULT_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_RESULT_CODEC_IDENTITY "UWVMROS1_1d1fa277be54e1df3bd314a632ff80d79f22a306fc232219a7c23cce7a7703a3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_valid(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result const* value) {
  if(!value)return 0;
  if(value->result.size>256u||!uwvm_bounded_utf8(value->result.data,value->result.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_wire_size(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result const* value) {size_t n=0;
  n+=4+value->result.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_write_fields(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->result.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->result.data,value->result.size);*offset+=value->result.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result* value) {
  if(*offset>n||n-*offset<4)return 0;value->result.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->result.size>256u||value->result.size>n-*offset)return 0;uwvm_bounded_copy(value->result.data,p+*offset,value->result.size);*offset+=value->result.size;
  return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_encode(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result* value) {
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for turtlebot3_msgs/action/Patrol_Feedback.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_state_text;
/** Complete ROS2 turtlebot3_msgs/action/Patrol_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_state_text state;
} roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_FEEDBACK_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_FEEDBACK_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_ACTION_PATROL_FEEDBACK_CODEC_IDENTITY "UWVMROS1_7c2f3824653a3048e1b93fa95584543a01efae8bb67253d3371a2988407a601e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_valid(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_wire_size(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback const* value) {size_t n=0;
  n+=4+value->state.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_write_fields(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  return roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_encode(roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback* value) {
  roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_action_Patrol_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 turtlebot3_msgs/msg/SensorState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field bumper; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t bumper;
  /** Declared field cliff; canonical capacity is part of CODEC_IDENTITY. */
  float cliff;
  /** Declared field sonar; canonical capacity is part of CODEC_IDENTITY. */
  float sonar;
  /** Declared field illumination; canonical capacity is part of CODEC_IDENTITY. */
  float illumination;
  /** Declared field led; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t led;
  /** Declared field button; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t button;
  /** Declared field torque; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t torque;
  /** Declared field left_encoder; canonical capacity is part of CODEC_IDENTITY. */
  int32_t left_encoder;
  /** Declared field right_encoder; canonical capacity is part of CODEC_IDENTITY. */
  int32_t right_encoder;
  /** Declared field battery; canonical capacity is part of CODEC_IDENTITY. */
  float battery;
} roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_MAX_WIRE_BYTES 168u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_MIN_WIRE_BYTES 40u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_CODEC_IDENTITY "UWVMROS1_7974ef7ca70ababf0e3f7a5e4099335a1398933833260798482f155291a62358"
/** Original scalar constant BUMPER_FORWARD=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_BUMPER_FORWARD UINT32_C(1)
/** Original scalar constant BUMPER_BACKWARD=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_BUMPER_BACKWARD UINT32_C(2)
/** Original scalar constant CLIFF=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_CLIFF UINT32_C(1)
/** Original scalar constant SONAR=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_SONAR UINT32_C(1)
/** Original scalar constant ILLUMINATION=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_ILLUMINATION UINT32_C(1)
/** Original scalar constant BUTTON0=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_BUTTON0 UINT32_C(1)
/** Original scalar constant BUTTON1=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_BUTTON1 UINT32_C(2)
/** Original scalar constant ERROR_LEFT_MOTOR=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_ERROR_LEFT_MOTOR UINT32_C(1)
/** Original scalar constant ERROR_RIGHT_MOTOR=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_ERROR_RIGHT_MOTOR UINT32_C(2)
/** Original scalar constant TORQUE_ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_TORQUE_ON UINT32_C(1)
/** Original scalar constant TORQUE_OFF=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SENSORSTATE_TORQUE_OFF UINT32_C(2)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_valid(roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cliff,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->sonar,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->illumination,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->torque>1)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->battery,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_wire_size(roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_write_fields(roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->bumper,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cliff,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->sonar,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->illumination,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->led,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->button,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->torque,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->left_encoder,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->right_encoder,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->battery,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->bumper,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cliff,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sonar,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->illumination,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->led,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->button,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->torque,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->left_encoder,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->right_encoder,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->battery,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_encode(roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState* value) {
  roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState next={0};size_t offset=0;if(!value||(length&&!input)||length<40u||length>168u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_msg_SensorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 turtlebot3_msgs/msg/Sound value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_msg_Sound {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_turtlebot3_msgs_msg_Sound;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_CODEC_IDENTITY "UWVMROS1_e8bd9a5fdf84ad1df0f264bd13a0e8966918de19ab62e91c7524dd8fc17a4e4b"
/** Original scalar constant OFF=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_OFF UINT32_C(0)
/** Original scalar constant ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_ON UINT32_C(1)
/** Original scalar constant LOW_BATTERY=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_LOW_BATTERY UINT32_C(2)
/** Original scalar constant ERROR=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_ERROR UINT32_C(3)
/** Original scalar constant BUTTON1=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_BUTTON1 UINT32_C(4)
/** Original scalar constant BUTTON2=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_SOUND_BUTTON2 UINT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_valid(roswasmc_bounded_2_turtlebot3_msgs_msg_Sound const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_wire_size(roswasmc_bounded_2_turtlebot3_msgs_msg_Sound const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_write_fields(roswasmc_bounded_2_turtlebot3_msgs_msg_Sound const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_msg_Sound* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_encode(roswasmc_bounded_2_turtlebot3_msgs_msg_Sound const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_msg_Sound* value) {
  roswasmc_bounded_2_turtlebot3_msgs_msg_Sound next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_msg_Sound_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for turtlebot3_msgs/msg/VersionInfo.hardware; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_hardware_text;
/** UTF-8 byte storage for turtlebot3_msgs/msg/VersionInfo.firmware; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_firmware_text;
/** UTF-8 byte storage for turtlebot3_msgs/msg/VersionInfo.software; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_software_text;
/** Complete ROS2 turtlebot3_msgs/msg/VersionInfo value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo {
  /** Declared field hardware; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_hardware_text hardware;
  /** Declared field firmware; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_firmware_text firmware;
  /** Declared field software; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_software_text software;
} roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_VERSIONINFO_MAX_WIRE_BYTES 780u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_VERSIONINFO_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_MSG_VERSIONINFO_CODEC_IDENTITY "UWVMROS1_7668c77b01a05eeb8ba3c5f259f3f3a9dfad3d4fdf8aa566d65f6e8f1c0f2d6a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_valid(roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo const* value) {
  if(!value)return 0;
  if(value->hardware.size>256u||!uwvm_bounded_utf8(value->hardware.data,value->hardware.size))return 0;
  if(value->firmware.size>256u||!uwvm_bounded_utf8(value->firmware.data,value->firmware.size))return 0;
  if(value->software.size>256u||!uwvm_bounded_utf8(value->software.data,value->software.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_wire_size(roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo const* value) {size_t n=0;
  n+=4+value->hardware.size;
  n+=4+value->firmware.size;
  n+=4+value->software.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_write_fields(roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->hardware.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->hardware.data,value->hardware.size);*offset+=value->hardware.size;
  uwvm_bounded_put(p+*offset,value->firmware.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->firmware.data,value->firmware.size);*offset+=value->firmware.size;
  uwvm_bounded_put(p+*offset,value->software.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->software.data,value->software.size);*offset+=value->software.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo* value) {
  if(*offset>n||n-*offset<4)return 0;value->hardware.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->hardware.size>256u||value->hardware.size>n-*offset)return 0;uwvm_bounded_copy(value->hardware.data,p+*offset,value->hardware.size);*offset+=value->hardware.size;
  if(*offset>n||n-*offset<4)return 0;value->firmware.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->firmware.size>256u||value->firmware.size>n-*offset)return 0;uwvm_bounded_copy(value->firmware.data,p+*offset,value->firmware.size);*offset+=value->firmware.size;
  if(*offset>n||n-*offset<4)return 0;value->software.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->software.size>256u||value->software.size>n-*offset)return 0;uwvm_bounded_copy(value->software.data,p+*offset,value->software.size);*offset+=value->software.size;
  return roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_encode(roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo* value) {
  roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>780u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_msg_VersionInfo_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 turtlebot3_msgs/srv/Dqn_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request {
  /** Declared field action; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t action;
  /** Declared field init; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t init;
} roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_DQN_REQUEST_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_DQN_REQUEST_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_DQN_REQUEST_CODEC_IDENTITY "UWVMROS1_14042e65db9f4feef7a43bfb44255795470bb0ed91a3cf301ebec98c2d4e2d0e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_valid(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request const* value) {
  if(!value)return 0;
  if(value->init>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_wire_size(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_write_fields(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->action,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->init,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->action,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->init,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_encode(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request* value) {
  roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most128 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[128];} roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_state_sequence;
/** Complete ROS2 turtlebot3_msgs/srv/Dqn_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_state_sequence state;
  /** Declared field reward; canonical capacity is part of CODEC_IDENTITY. */
  float reward;
  /** Declared field done; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t done;
} roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_DQN_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_DQN_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_DQN_RESPONSE_CODEC_IDENTITY "UWVMROS1_5d256e9c87d7959917f07e7cfcc0049fc18f79f547d83b72bd388a63c1417c6e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_valid(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response const* value) {
  if(!value)return 0;
  if(value->state.size>128u)return 0;
  {size_t i;for(i=0;i<value->state.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->reward,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->done>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_wire_size(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response const* value) {size_t n=0;
  n+=4;
  if(value->state.size>128u)return 0;
  {size_t i;for(i=0;i<value->state.size;++i){
  n+=4;
  }}
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_write_fields(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->state.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->reward,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->done,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>128u)return 0;
  {size_t i;for(i=0;i<value->state.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->state.data[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reward,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->done,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_encode(roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response* value) {
  roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Dqn_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 turtlebot3_msgs/srv/Goal_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_GOAL_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_GOAL_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_GOAL_REQUEST_CODEC_IDENTITY "UWVMROS1_f76094f3cf00772bb80b59e846f1160ae6255fc0d931bbcba362104081698ded"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_valid(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_wire_size(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_write_fields(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request* value) {
  return roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_encode(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request* value) {
  roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 turtlebot3_msgs/srv/Goal_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response {
  /** Declared field pose_x; canonical capacity is part of CODEC_IDENTITY. */
  float pose_x;
  /** Declared field pose_y; canonical capacity is part of CODEC_IDENTITY. */
  float pose_y;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_GOAL_RESPONSE_MAX_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_GOAL_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_GOAL_RESPONSE_CODEC_IDENTITY "UWVMROS1_8b9022a7fb8c68a0b063b080cd05c230b653169badbe205bcd8051b8020f7990"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_valid(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pose_x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pose_y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_wire_size(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_write_fields(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pose_x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pose_y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pose_x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pose_y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_encode(roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response* value) {
  roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>9u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Goal_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 turtlebot3_msgs/srv/Sound_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_SOUND_REQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_SOUND_REQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_SOUND_REQUEST_CODEC_IDENTITY "UWVMROS1_0dcba95f5ba646d9a24b0054ba603daf1d1e5e4e9a6ffe7387cd61bff2e4151a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_valid(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_wire_size(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_write_fields(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_encode(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request* value) {
  roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for turtlebot3_msgs/srv/Sound_Response.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_message_text;
/** Complete ROS2 turtlebot3_msgs/srv/Sound_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_message_text message;
} roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response;
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_SOUND_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_SOUND_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_TURTLEBOT3_MSGS_SRV_SOUND_RESPONSE_CODEC_IDENTITY "UWVMROS1_bc5a5fd3e2bf733a278b8a1315c8905b27fff49b4844df238d5574149068998e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_valid(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_wire_size(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_write_fields(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_encode(roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_decode(void const* input,size_t length,roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response* value) {
  roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_turtlebot3_msgs_srv_Sound_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
