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
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_positions_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_velocities_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_accelerations_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_effort_sequence;
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
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_MAX_WIRE_BYTES 216u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORYPOINT_CODEC_IDENTITY "UWVMROS1_c4eafb6efd22d299b5c6fc21844d8bf69789cf27ee5414a1d7d2371f0b1acbbf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value) {
  if(!value)return 0;
  if(value->positions.size>6u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->positions.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->velocities.size>6u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->velocities.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->accelerations.size>6u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->accelerations.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->effort.size>6u)return 0;
  {size_t i;for(i=0;i<value->effort.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->effort.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->time_from_start))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint const* value) {size_t n=0;
  n+=4;
  if(value->positions.size>6u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->velocities.size>6u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->accelerations.size>6u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->effort.size>6u)return 0;
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
  if(value->positions.size>6u)return 0;
  {size_t i;for(i=0;i<value->positions.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->positions.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->velocities.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->velocities.size>6u)return 0;
  {size_t i;for(i=0;i<value->velocities.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->velocities.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->accelerations.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->accelerations.size>6u)return 0;
  {size_t i;for(i=0;i<value->accelerations.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->accelerations.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->effort.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->effort.size>6u)return 0;
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
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>216u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for trajectory_msgs/msg/JointTrajectory.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_text;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_text data[6];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint data[16];} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_points_sequence;
/** Complete ROS2 trajectory_msgs/msg/JointTrajectory value; no field projects away native data. */
typedef struct roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_joint_names_sequence joint_names;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_points_sequence points;
} roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory;
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_MAX_WIRE_BYTES 5164u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_TRAJECTORY_MSGS_MSG_JOINTTRAJECTORY_CODEC_IDENTITY "UWVMROS1_e77c37c75123e442edb6a7f6f2a21dcbe33d8bb8fd2577f9906653a6ad5f8225"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->joint_names.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>256u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(value->points.size>16u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->points.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=4;
  if(value->points.size>16u)return 0;
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
  if(value->joint_names.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>256u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>16u)return 0;
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
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>5164u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/action/SetMode_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal {
  /** Declared field target_robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t target_robot_mode;
  /** Declared field stop_program; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t stop_program;
  /** Declared field play_program; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t play_program;
} roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_GOAL_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_GOAL_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_GOAL_CODEC_IDENTITY "UWVMROS1_8a7484a60bad21933df3f847d3fd554e41098ed596dee270335ce9f092dc313d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_valid(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal const* value) {
  if(!value)return 0;
  if(value->stop_program>1)return 0;
  if(value->play_program>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->target_robot_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->stop_program,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->play_program,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->target_robot_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stop_program,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->play_program,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_encode(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/action/SetMode_Result.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_message_text;
/** Complete ROS2 ur_dashboard_msgs/action/SetMode_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_message_text message;
} roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_RESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_RESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_RESULT_CODEC_IDENTITY "UWVMROS1_ef2a557ff27171319d46170ab16a80f5cba5fbd59e44c21cc1b704d01764b913"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_valid(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_encode(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/action/SetMode_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback {
  /** Declared field current_robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t current_robot_mode;
  /** Declared field current_safety_mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t current_safety_mode;
} roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_FEEDBACK_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_FEEDBACK_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_ACTION_SETMODE_FEEDBACK_CODEC_IDENTITY "UWVMROS1_afd74cd0a79174b53499a40b23b040ba96ec3d3e332129999525dcf6ca287ac6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_valid(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->current_robot_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->current_safety_mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->current_robot_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->current_safety_mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_encode(roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_action_SetMode_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/msg/OperationalMode.mode; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_mode_text;
/** Complete ROS2 ur_dashboard_msgs/msg/OperationalMode value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_mode_text mode;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_OPERATIONALMODE_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_OPERATIONALMODE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_OPERATIONALMODE_CODEC_IDENTITY "UWVMROS1_7a0be8522cdc20e01d382218499030176a0f6565376fadc5bbdd871a01b9316e"
/** Original scalar constant MANUAL=MANUAL; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_OPERATIONALMODE_MANUAL "\115\101\116\125\101\114"
/** Original scalar constant AUTOMATIC=AUTOMATIC; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_OPERATIONALMODE_AUTOMATIC "\101\125\124\117\115\101\124\111\103"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode const* value) {
  if(!value)return 0;
  if(value->mode.size>256u||!uwvm_bounded_utf8(value->mode.data,value->mode.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode const* value) {size_t n=0;
  n+=4+value->mode.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->mode.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->mode.data,value->mode.size);*offset+=value->mode.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode* value) {
  if(*offset>n||n-*offset<4)return 0;value->mode.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->mode.size>256u||value->mode.size>n-*offset)return 0;uwvm_bounded_copy(value->mode.data,p+*offset,value->mode.size);*offset+=value->mode.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/msg/ProgramInformation.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_name_text;
/** UTF-8 byte storage for ur_dashboard_msgs/msg/ProgramInformation.description; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_description_text;
/** UTF-8 byte storage for ur_dashboard_msgs/msg/ProgramInformation.program_state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_program_state_text;
/** Complete ROS2 ur_dashboard_msgs/msg/ProgramInformation value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation {
  /** Declared field name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_name_text name;
  /** Declared field description; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_description_text description;
  /** Declared field created_date; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t created_date;
  /** Declared field last_saved_date; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t last_saved_date;
  /** Declared field last_modified_date; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t last_modified_date;
  /** Declared field program_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_program_state_text program_state;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMINFORMATION_MAX_WIRE_BYTES 676u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMINFORMATION_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMINFORMATION_CODEC_IDENTITY "UWVMROS1_9299ce545acd0b6ff064998334820fbe503b71dc47a475ef93545d2cdb41f965"
/** Original scalar constant PROGRAM_STATE_DRAFT="DRAFT"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMINFORMATION_PROGRAM_STATE_DRAFT "\104\122\101\106\124"
/** Original scalar constant PROGRAM_STATE_FINAL="FINAL"; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMINFORMATION_PROGRAM_STATE_FINAL "\106\111\116\101\114"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation const* value) {
  if(!value)return 0;
  if(value->name.size>128u||!uwvm_bounded_utf8(value->name.data,value->name.size))return 0;
  if(value->description.size>256u||!uwvm_bounded_utf8(value->description.data,value->description.size))return 0;
  if(value->program_state.size>256u||!uwvm_bounded_utf8(value->program_state.data,value->program_state.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation const* value) {size_t n=0;
  n+=4+value->name.size;
  n+=4+value->description.size;
  n+=8;
  n+=8;
  n+=8;
  n+=4+value->program_state.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->name.data,value->name.size);*offset+=value->name.size;
  uwvm_bounded_put(p+*offset,value->description.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->description.data,value->description.size);*offset+=value->description.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->created_date,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->last_saved_date,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->last_modified_date,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->program_state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_state.data,value->program_state.size);*offset+=value->program_state.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation* value) {
  if(*offset>n||n-*offset<4)return 0;value->name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->name.size>128u||value->name.size>n-*offset)return 0;uwvm_bounded_copy(value->name.data,p+*offset,value->name.size);*offset+=value->name.size;
  if(*offset>n||n-*offset<4)return 0;value->description.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->description.size>256u||value->description.size>n-*offset)return 0;uwvm_bounded_copy(value->description.data,p+*offset,value->description.size);*offset+=value->description.size;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->created_date,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->last_saved_date,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->last_modified_date,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->program_state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_state.size>256u||value->program_state.size>n-*offset)return 0;uwvm_bounded_copy(value->program_state.data,p+*offset,value->program_state.size);*offset+=value->program_state.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>676u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/msg/ProgramState.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_state_text;
/** Complete ROS2 ur_dashboard_msgs/msg/ProgramState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_state_text state;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMSTATE_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMSTATE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMSTATE_CODEC_IDENTITY "UWVMROS1_1462a3c2323792afbbfc6d48a600a6ab66a50a6f6f583e57a62da2458ea81f26"
/** Original scalar constant STOPPED=STOPPED; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMSTATE_STOPPED "\123\124\117\120\120\105\104"
/** Original scalar constant PLAYING=PLAYING; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMSTATE_PLAYING "\120\114\101\131\111\116\107"
/** Original scalar constant PAUSED=PAUSED; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_PROGRAMSTATE_PAUSED "\120\101\125\123\105\104"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState const* value) {size_t n=0;
  n+=4+value->state.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/msg/RobotMode value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t mode;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_CODEC_IDENTITY "UWVMROS1_81a79c84083520d911de108a728ad1877de78a57ec9e27b5cc432590be4616bb"
/** Original scalar constant NO_CONTROLLER=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_NO_CONTROLLER -INT32_C(1)
/** Original scalar constant DISCONNECTED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_DISCONNECTED INT32_C(0)
/** Original scalar constant CONFIRM_SAFETY=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_CONFIRM_SAFETY INT32_C(1)
/** Original scalar constant BOOTING=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_BOOTING INT32_C(2)
/** Original scalar constant POWER_OFF=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_POWER_OFF INT32_C(3)
/** Original scalar constant POWER_ON=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_POWER_ON INT32_C(4)
/** Original scalar constant IDLE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_IDLE INT32_C(5)
/** Original scalar constant BACKDRIVE=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_BACKDRIVE INT32_C(6)
/** Original scalar constant RUNNING=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_RUNNING INT32_C(7)
/** Original scalar constant UPDATING_FIRMWARE=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_ROBOTMODE_UPDATING_FIRMWARE INT32_C(8)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/msg/SafetyMode value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_CODEC_IDENTITY "UWVMROS1_2eb3d5075a2d7d1765a604c1d1d198379010867d83a98d0327488ca51f5e1aac"
/** Original scalar constant NORMAL=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_NORMAL UINT32_C(1)
/** Original scalar constant REDUCED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_REDUCED UINT32_C(2)
/** Original scalar constant PROTECTIVE_STOP=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_PROTECTIVE_STOP UINT32_C(3)
/** Original scalar constant RECOVERY=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_RECOVERY UINT32_C(4)
/** Original scalar constant SAFEGUARD_STOP=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_SAFEGUARD_STOP UINT32_C(5)
/** Original scalar constant SYSTEM_EMERGENCY_STOP=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_SYSTEM_EMERGENCY_STOP UINT32_C(6)
/** Original scalar constant ROBOT_EMERGENCY_STOP=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_ROBOT_EMERGENCY_STOP UINT32_C(7)
/** Original scalar constant VIOLATION=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_VIOLATION UINT32_C(8)
/** Original scalar constant FAULT=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_FAULT UINT32_C(9)
/** Original scalar constant VALIDATE_JOINT_ID=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_VALIDATE_JOINT_ID UINT32_C(10)
/** Original scalar constant UNDEFINED_SAFETY_MODE=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_UNDEFINED_SAFETY_MODE UINT32_C(11)
/** Original scalar constant AUTOMATIC_MODE_SAFEGUARD_STOP=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_AUTOMATIC_MODE_SAFEGUARD_STOP UINT32_C(12)
/** Original scalar constant SYSTEM_THREE_POSITION_ENABLING_STOP=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYMODE_SYSTEM_THREE_POSITION_ENABLING_STOP UINT32_C(13)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/msg/SafetyStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  int8_t status;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_CODEC_IDENTITY "UWVMROS1_1b1a7257c40d9c62e0b6291f241825d029fe38f3f3695265a182a270899a79e9"
/** Original scalar constant NORMAL=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_NORMAL INT32_C(1)
/** Original scalar constant REDUCED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_REDUCED INT32_C(2)
/** Original scalar constant PROTECTIVE_STOP=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_PROTECTIVE_STOP INT32_C(3)
/** Original scalar constant RECOVERY=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_RECOVERY INT32_C(4)
/** Original scalar constant SAFEGUARD_STOP=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_SAFEGUARD_STOP INT32_C(5)
/** Original scalar constant SYSTEM_EMERGENCY_STOP=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_SYSTEM_EMERGENCY_STOP INT32_C(6)
/** Original scalar constant ROBOT_EMERGENCY_STOP=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_ROBOT_EMERGENCY_STOP INT32_C(7)
/** Original scalar constant VIOLATION=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_VIOLATION INT32_C(8)
/** Original scalar constant FAULT=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_FAULT INT32_C(9)
/** Original scalar constant VALIDATE_JOINT_ID=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_VALIDATE_JOINT_ID INT32_C(10)
/** Original scalar constant UNDEFINED_SAFETY_MODE=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_UNDEFINED_SAFETY_MODE INT32_C(11)
/** Original scalar constant AUTOMATIC_MODE_SAFEGUARD_STOP=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_AUTOMATIC_MODE_SAFEGUARD_STOP INT32_C(12)
/** Original scalar constant SYSTEM_THREE_POSITION_ENABLING_STOP=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_SYSTEM_THREE_POSITION_ENABLING_STOP INT32_C(13)
/** Original scalar constant TP_THREE_POSITION_ENABLING_STOP=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_TP_THREE_POSITION_ENABLING_STOP INT32_C(14)
/** Original scalar constant IMMI_EMERGENCY_STOP=15; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_IMMI_EMERGENCY_STOP INT32_C(15)
/** Original scalar constant IMMI_SAFEGUARD_STOP=16; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_IMMI_SAFEGUARD_STOP INT32_C(16)
/** Original scalar constant PROFISAFE_WAITING_FOR_PARAMETERS=17; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_PROFISAFE_WAITING_FOR_PARAMETERS INT32_C(17)
/** Original scalar constant PROFISAFE_AUTOMATIC_MODE_SAFEGUARD_STOP=18; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_PROFISAFE_AUTOMATIC_MODE_SAFEGUARD_STOP INT32_C(18)
/** Original scalar constant PROFISAFE_SAFEGUARD_STOP=19; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_PROFISAFE_SAFEGUARD_STOP INT32_C(19)
/** Original scalar constant PROFISAFE_EMERGENCY_STOP=20; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_PROFISAFE_EMERGENCY_STOP INT32_C(20)
/** Original scalar constant SAFETY_API_SAFEGUARD_STOP=22; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_SAFETY_API_SAFEGUARD_STOP INT32_C(22)
/** Original scalar constant IO_PLANE_STOP=23; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_SAFETYSTATUS_IO_PLANE_STOP INT32_C(23)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/msg/UserRole.role; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_role_text;
/** Complete ROS2 ur_dashboard_msgs/msg/UserRole value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole {
  /** Declared field role; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_role_text role;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_CODEC_IDENTITY "UWVMROS1_7568bc9417685a9bc56cfdd2b41ff81ab99bd8e20fbe5f9e537b5b5d0689440a"
/** Original scalar constant PROGRAMMER=PROGRAMMER; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_PROGRAMMER "\120\122\117\107\122\101\115\115\105\122"
/** Original scalar constant OPERATOR=OPERATOR; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_OPERATOR "\117\120\105\122\101\124\117\122"
/** Original scalar constant NONE=NONE; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_NONE "\116\117\116\105"
/** Original scalar constant LOCKED=LOCKED; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_LOCKED "\114\117\103\113\105\104"
/** Original scalar constant RESTRICTED=RESTRICTED; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_USERROLE_RESTRICTED "\122\105\123\124\122\111\103\124\105\104"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole const* value) {
  if(!value)return 0;
  if(value->role.size>256u||!uwvm_bounded_utf8(value->role.data,value->role.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole const* value) {size_t n=0;
  n+=4+value->role.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->role.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->role.data,value->role.size);*offset+=value->role.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole* value) {
  if(*offset>n||n-*offset<4)return 0;value->role.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->role.size>256u||value->role.size>n-*offset)return 0;uwvm_bounded_copy(value->role.data,p+*offset,value->role.size);*offset+=value->role.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/msg/VersionInformation value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation {
  /** Declared field major; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t major;
  /** Declared field minor; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t minor;
  /** Declared field bugfix; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t bugfix;
  /** Declared field build; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t build;
} roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_VERSIONINFORMATION_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_VERSIONINFORMATION_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_MSG_VERSIONINFORMATION_CODEC_IDENTITY "UWVMROS1_450c41c4361f1bca51f864dd9ef596851b17ebc79d4f784e7abb8b721752443d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_valid(roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->major,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->minor,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->bugfix,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->build,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->major,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->minor,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bugfix,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->build,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_encode(roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/AddToLog_Request.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_message_text;
/** Complete ROS2 ur_dashboard_msgs/srv/AddToLog_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request {
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_message_text message;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ADDTOLOG_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ADDTOLOG_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ADDTOLOG_REQUEST_CODEC_IDENTITY "UWVMROS1_ecfe5bb0d0db92bc5dfdec9bf82fbe730e3b3b4d65680812d0540b9055590142"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request const* value) {
  if(!value)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request const* value) {size_t n=0;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/AddToLog_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/AddToLog_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ADDTOLOG_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ADDTOLOG_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ADDTOLOG_RESPONSE_CODEC_IDENTITY "UWVMROS1_57b0c150a353d3fca6e96c4dc86e72f71b452663ede1eeb69e51069992d07e4c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_AddToLog_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/DownloadProgram_Request.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_program_name_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/DownloadProgram_Request.target_path; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_target_path_text;
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadProgram_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request {
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_program_name_text program_name;
  /** Declared field target_path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_target_path_text target_path;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADPROGRAM_REQUEST_MAX_WIRE_BYTES 520u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADPROGRAM_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADPROGRAM_REQUEST_CODEC_IDENTITY "UWVMROS1_5c84420393aae13ea448d171a7b2956a55a39f3321f504b9f3bd5b4a38960c2f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request const* value) {
  if(!value)return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->target_path.size>256u||!uwvm_bounded_utf8(value->target_path.data,value->target_path.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request const* value) {size_t n=0;
  n+=4+value->program_name.size;
  n+=4+value->target_path.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,value->target_path.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->target_path.data,value->target_path.size);*offset+=value->target_path.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<4)return 0;value->target_path.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->target_path.size>256u||value->target_path.size>n-*offset)return 0;uwvm_bounded_copy(value->target_path.data,p+*offset,value->target_path.size);*offset+=value->target_path.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>520u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/DownloadProgram_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadProgram_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADPROGRAM_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADPROGRAM_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADPROGRAM_RESPONSE_CODEC_IDENTITY "UWVMROS1_46b0a73805282bad5f819f712275b7dd2ca4a6bb8c73c445ea8ae0a3d0f9e8e0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadProgram_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/DownloadSupportFile_Request.target_path; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_target_path_text;
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadSupportFile_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request {
  /** Declared field target_path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_target_path_text target_path;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADSUPPORTFILE_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADSUPPORTFILE_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADSUPPORTFILE_REQUEST_CODEC_IDENTITY "UWVMROS1_d919563629f81d5a8c10f1d2921ea16f3c912a49b8d3bcf65d17b8daa0a38140"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request const* value) {
  if(!value)return 0;
  if(value->target_path.size>256u||!uwvm_bounded_utf8(value->target_path.data,value->target_path.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request const* value) {size_t n=0;
  n+=4+value->target_path.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->target_path.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->target_path.data,value->target_path.size);*offset+=value->target_path.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->target_path.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->target_path.size>256u||value->target_path.size>n-*offset)return 0;uwvm_bounded_copy(value->target_path.data,p+*offset,value->target_path.size);*offset+=value->target_path.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/DownloadSupportFile_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/DownloadSupportFile_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field support_files_present; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t support_files_present;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADSUPPORTFILE_RESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADSUPPORTFILE_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_DOWNLOADSUPPORTFILE_RESPONSE_CODEC_IDENTITY "UWVMROS1_76e190e6e9f1ddfff3082ea2adb0463d21ba9fb6795153fe6bbacee8a3334667"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  if(value->support_files_present>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->support_files_present,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->support_files_present,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_DownloadSupportFile_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GenerateFlightReport_Request.report_type; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_report_type_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateFlightReport_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request {
  /** Declared field report_type; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_report_type_text report_type;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_REQUEST_CODEC_IDENTITY "UWVMROS1_7246d97690c0c827e9cfab2f8297cf27a10e60e41ebb3ba345f361a109057099"
/** Original scalar constant CONTROLLER=controller; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_REQUEST_CONTROLLER "\143\157\156\164\162\157\154\154\145\162"
/** Original scalar constant SOFTWARE=software; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_REQUEST_SOFTWARE "\163\157\146\164\167\141\162\145"
/** Original scalar constant SYSTEM=system; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_REQUEST_SYSTEM "\163\171\163\164\145\155"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_default(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.report_type.size=6u;uwvm_bounded_copy(next.report_type.data,"\123\131\123\124\105\115",6u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request const* value) {
  if(!value)return 0;
  if(value->report_type.size>256u||!uwvm_bounded_utf8(value->report_type.data,value->report_type.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request const* value) {size_t n=0;
  n+=4+value->report_type.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->report_type.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->report_type.data,value->report_type.size);*offset+=value->report_type.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->report_type.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->report_type.size>256u||value->report_type.size>n-*offset)return 0;uwvm_bounded_copy(value->report_type.data,p+*offset,value->report_type.size);*offset+=value->report_type.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GenerateFlightReport_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GenerateFlightReport_Response.report_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_report_id_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateFlightReport_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_answer_text answer;
  /** Declared field report_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_report_id_text report_id;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATEFLIGHTREPORT_RESPONSE_CODEC_IDENTITY "UWVMROS1_adebf67876b5ae3377cec0aa04138c2ab73ef7c0d7942c77c6c06b248a836445"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->report_id.size>256u||!uwvm_bounded_utf8(value->report_id.data,value->report_id.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->answer.size;
  n+=4+value->report_id.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->report_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->report_id.data,value->report_id.size);*offset+=value->report_id.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->report_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->report_id.size>256u||value->report_id.size>n-*offset)return 0;uwvm_bounded_copy(value->report_id.data,p+*offset,value->report_id.size);*offset+=value->report_id.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateFlightReport_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GenerateSupportFile_Request.dir_path; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_dir_path_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateSupportFile_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request {
  /** Declared field dir_path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_dir_path_text dir_path;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATESUPPORTFILE_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATESUPPORTFILE_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATESUPPORTFILE_REQUEST_CODEC_IDENTITY "UWVMROS1_10b0371b77db32d75a77dcb0605b325caa1915ccfb7d0917896fd75234e6ada7"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_default(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.dir_path.size=1u;uwvm_bounded_copy(next.dir_path.data,"\056",1u);
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request const* value) {
  if(!value)return 0;
  if(value->dir_path.size>256u||!uwvm_bounded_utf8(value->dir_path.data,value->dir_path.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request const* value) {size_t n=0;
  n+=4+value->dir_path.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->dir_path.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->dir_path.data,value->dir_path.size);*offset+=value->dir_path.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->dir_path.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->dir_path.size>256u||value->dir_path.size>n-*offset)return 0;uwvm_bounded_copy(value->dir_path.data,p+*offset,value->dir_path.size);*offset+=value->dir_path.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GenerateSupportFile_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GenerateSupportFile_Response.generated_file_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_generated_file_name_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GenerateSupportFile_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_answer_text answer;
  /** Declared field generated_file_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_generated_file_name_text generated_file_name;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATESUPPORTFILE_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATESUPPORTFILE_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GENERATESUPPORTFILE_RESPONSE_CODEC_IDENTITY "UWVMROS1_f370623fe690f218073f8538aa9fcf41c94d4abd3e0507e09607ef11e10edf38"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->generated_file_name.size>256u||!uwvm_bounded_utf8(value->generated_file_name.data,value->generated_file_name.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->answer.size;
  n+=4+value->generated_file_name.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->generated_file_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->generated_file_name.data,value->generated_file_name.size);*offset+=value->generated_file_name.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->generated_file_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->generated_file_name.size>256u||value->generated_file_name.size>n-*offset)return 0;uwvm_bounded_copy(value->generated_file_name.data,p+*offset,value->generated_file_name.size);*offset+=value->generated_file_name.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GenerateSupportFile_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetLoadedProgram_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETLOADEDPROGRAM_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETLOADEDPROGRAM_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETLOADEDPROGRAM_REQUEST_CODEC_IDENTITY "UWVMROS1_030e5746e6a4da8f266e37d11cc3826715099ff1aa1c845a97042aec987707e3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetLoadedProgram_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetLoadedProgram_Response.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_program_name_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetLoadedProgram_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_answer_text answer;
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_program_name_text program_name;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETLOADEDPROGRAM_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETLOADEDPROGRAM_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETLOADEDPROGRAM_RESPONSE_CODEC_IDENTITY "UWVMROS1_3e8c7f3c8711faea01de37b9a011f34a9c6dd660cc222e46177b2c1898ec2f7e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=4+value->program_name.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetLoadedProgram_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetOperationalMode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETOPERATIONALMODE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETOPERATIONALMODE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETOPERATIONALMODE_REQUEST_CODEC_IDENTITY "UWVMROS1_3f348b5d9e04a8ba01f5277ad259c34a6816ae0693f47c49e348a5497a90a325"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetOperationalMode_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetOperationalMode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_answer_text answer;
  /** Declared field operational_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode operational_mode;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETOPERATIONALMODE_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETOPERATIONALMODE_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETOPERATIONALMODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_6c2da335fd551162020eb394b91e99aaeca17e34e7c2e98de935c81912efb5f6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_valid(&value->operational_mode))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->answer.size;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_wire_size(&value->operational_mode);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_write_fields(&value->operational_mode,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_read_fields(p,n,offset,&value->operational_mode))return 0;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetOperationalMode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetPolyScopeVersion_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPOLYSCOPEVERSION_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPOLYSCOPEVERSION_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPOLYSCOPEVERSION_REQUEST_CODEC_IDENTITY "UWVMROS1_88193ae202f0cb02746bc5556e8e8ee277fb4fae72ee4b96a6539c980dc86f13"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetPolyScopeVersion_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetPolyScopeVersion_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation version;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPOLYSCOPEVERSION_RESPONSE_MAX_WIRE_BYTES 277u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPOLYSCOPEVERSION_RESPONSE_MIN_WIRE_BYTES 21u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPOLYSCOPEVERSION_RESPONSE_CODEC_IDENTITY "UWVMROS1_c8d53d0a920806e86ceb6d2ac5a692a92e62b057432554bed9dcf57bed0ff9bd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_valid(&value->version))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_wire_size(&value->version);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_write_fields(&value->version,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_VersionInformation_read_fields(p,n,offset,&value->version))return 0;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<21u||length>277u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPolyScopeVersion_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetProgramState_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMSTATE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMSTATE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMSTATE_REQUEST_CODEC_IDENTITY "UWVMROS1_fe42d1d1aa3d7bdbaa410bce045120d6dd841213ef10c5dd158c2b76fa157f65"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetProgramState_Response.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_program_name_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetProgramState_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetProgramState_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState state;
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_program_name_text program_name;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMSTATE_RESPONSE_MAX_WIRE_BYTES 781u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMSTATE_RESPONSE_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMSTATE_RESPONSE_CODEC_IDENTITY "UWVMROS1_b20731b8552f323837c5939f390060a6c7c92e9447df5e620891708cfce51ff4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_valid(&value->state))return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_wire_size(&value->state);
  n+=4+value->program_name.size;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_write_fields(&value->state,p,offset);
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response* value) {
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramState_read_fields(p,n,offset,&value->state))return 0;
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>781u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetProgramState_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetPrograms_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMS_REQUEST_CODEC_IDENTITY "UWVMROS1_8d7b4f3fb9a15854b59c85c5205bf20d3e0f6a1a972ccbdf77ee8377c9e40d6d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation data[16];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_programs_sequence;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetPrograms_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetPrograms_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response {
  /** Declared field programs; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_programs_sequence programs;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMS_RESPONSE_MAX_WIRE_BYTES 11081u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMS_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETPROGRAMS_RESPONSE_CODEC_IDENTITY "UWVMROS1_111b67c562ba0c77c2924b97058e4126055ad6deb7f99049b8caf8990b3f979c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response const* value) {
  if(!value)return 0;
  if(value->programs.size>16u)return 0;
  {size_t i;for(i=0;i<value->programs.size;++i){
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_valid(&value->programs.data[i]))return 0;
  }}
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response const* value) {size_t n=0;
  n+=4;
  if(value->programs.size>16u)return 0;
  {size_t i;for(i=0;i<value->programs.size;++i){
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_wire_size(&value->programs.data[i]);
  }}
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->programs.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->programs.size;++i){
  roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_write_fields(&value->programs.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->programs.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->programs.size>16u)return 0;
  {size_t i;for(i=0;i<value->programs.size;++i){
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_ProgramInformation_read_fields(p,n,offset,&value->programs.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>11081u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetPrograms_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotMode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODE_REQUEST_CODEC_IDENTITY "UWVMROS1_d1d9cd94a443b20ec0d41a8db1a6d87291657597abc014e7da0f46a58c6c5614"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetRobotMode_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotMode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response {
  /** Declared field robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode robot_mode;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODE_RESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODE_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_67db750ad95b3b207d61b18513a117c868040c22c93638f07fa74645d87bcfba"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_valid(&value->robot_mode))return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_wire_size(&value->robot_mode);
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_write_fields(&value->robot_mode,p,offset);
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response* value) {
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_RobotMode_read_fields(p,n,offset,&value->robot_mode))return 0;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotMode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotModel_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODEL_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODEL_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODEL_REQUEST_CODEC_IDENTITY "UWVMROS1_9db08f07993bd5786aea5b6bd567f9e9f49329883326ea16e6c49eef1586dc43"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetRobotModel_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetRobotModel_Response.robot_model; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_robot_model_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetRobotModel_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_answer_text answer;
  /** Declared field robot_model; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_robot_model_text robot_model;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODEL_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODEL_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETROBOTMODEL_RESPONSE_CODEC_IDENTITY "UWVMROS1_faf0124c542ef7420915527c27e99a7d26c5d6274f55d869bc9416e6b7d9b3c9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->robot_model.size>256u||!uwvm_bounded_utf8(value->robot_model.data,value->robot_model.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->answer.size;
  n+=4+value->robot_model.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->robot_model.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->robot_model.data,value->robot_model.size);*offset+=value->robot_model.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->robot_model.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->robot_model.size>256u||value->robot_model.size>n-*offset)return 0;uwvm_bounded_copy(value->robot_model.data,p+*offset,value->robot_model.size);*offset+=value->robot_model.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetRobotModel_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyMode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYMODE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYMODE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYMODE_REQUEST_CODEC_IDENTITY "UWVMROS1_5866cc4b0e10508eeb45ceecf9f9b6036ba55d04c9613622e189ccde2781dcb0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetSafetyMode_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyMode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response {
  /** Declared field safety_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode safety_mode;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYMODE_RESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYMODE_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYMODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_f65d7ff83dc97f891032d8ab16bf71a9c2326c6ae6a033fe1d6e06f011c5a40b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_valid(&value->safety_mode))return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_wire_size(&value->safety_mode);
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_write_fields(&value->safety_mode,p,offset);
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response* value) {
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyMode_read_fields(p,n,offset,&value->safety_mode))return 0;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyMode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyStatus_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYSTATUS_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYSTATUS_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYSTATUS_REQUEST_CODEC_IDENTITY "UWVMROS1_0e2958bcd7b121682361c94b2926bcc4552d7ef07dc2af4609d04b1bb7054602"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetSafetyStatus_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetSafetyStatus_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field safety_status; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus safety_status;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYSTATUS_RESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYSTATUS_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSAFETYSTATUS_RESPONSE_CODEC_IDENTITY "UWVMROS1_bb96aaa96f5c78b97ad80a14eb96865dc10b5a1d12524fcbcc75fc6d481c53e4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_valid(&value->safety_status))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_wire_size(&value->safety_status);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_write_fields(&value->safety_status,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_SafetyStatus_read_fields(p,n,offset,&value->safety_status))return 0;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSafetyStatus_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetSerialNumber_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSERIALNUMBER_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSERIALNUMBER_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSERIALNUMBER_REQUEST_CODEC_IDENTITY "UWVMROS1_bbb1ca066c77ed459352de69d566f3ea87b94fe593c31c911193416f92a73c9b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetSerialNumber_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetSerialNumber_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response {
  /** Declared field serial_number; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t serial_number;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSERIALNUMBER_RESPONSE_MAX_WIRE_BYTES 269u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSERIALNUMBER_RESPONSE_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETSERIALNUMBER_RESPONSE_CODEC_IDENTITY "UWVMROS1_f389641d089fa6fd421348f047732de2122cd714bdb2bd4d1e611592aca349e7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response const* value) {size_t n=0;
  n+=8;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->serial_number,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->serial_number,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>269u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetSerialNumber_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/GetUserRole_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETUSERROLE_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETUSERROLE_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETUSERROLE_REQUEST_CODEC_IDENTITY "UWVMROS1_a331be09dd2093e4584e6dc80f83caaadd3a8e43cca6a1c8fcfa1e2ea632c9f0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/GetUserRole_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/GetUserRole_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response {
  /** Declared field user_role; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole user_role;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETUSERROLE_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETUSERROLE_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_GETUSERROLE_RESPONSE_CODEC_IDENTITY "UWVMROS1_ed4f53e59fbe4cd33cfbf5d190b60fbe8371dda7da9d7af6e270a882eb142df8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_valid(&value->user_role))return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_wire_size(&value->user_role);
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_write_fields(&value->user_role,p,offset);
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response* value) {
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_read_fields(p,n,offset,&value->user_role))return 0;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_GetUserRole_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/IsInRemoteControl_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISINREMOTECONTROL_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISINREMOTECONTROL_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISINREMOTECONTROL_REQUEST_CODEC_IDENTITY "UWVMROS1_4b6b86fc4337c36589e8781b46ddbe6992f750b565ff413ea0b069982d87a463"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/IsInRemoteControl_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/IsInRemoteControl_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_answer_text answer;
  /** Declared field remote_control; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t remote_control;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISINREMOTECONTROL_RESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISINREMOTECONTROL_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISINREMOTECONTROL_RESPONSE_CODEC_IDENTITY "UWVMROS1_f2705c1be319c7c667f4f2224851b06c24b15b4211c2828fab89f09455e4c816"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->remote_control>1)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->remote_control,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->remote_control,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsInRemoteControl_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramRunning_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMRUNNING_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMRUNNING_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMRUNNING_REQUEST_CODEC_IDENTITY "UWVMROS1_f28274c0dc01d8aa2f981f969c05a614e6b3922601d578f99c6321a4b20fc80f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/IsProgramRunning_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramRunning_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_answer_text answer;
  /** Declared field program_running; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t program_running;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMRUNNING_RESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMRUNNING_RESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMRUNNING_RESPONSE_CODEC_IDENTITY "UWVMROS1_c57cc7ecd7e81f7359824b43884a58f65748a4c77cacbcc45be17aa9147cf499"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->program_running>1)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->program_running,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->program_running,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramRunning_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramSaved_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMSAVED_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMSAVED_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMSAVED_REQUEST_CODEC_IDENTITY "UWVMROS1_c53d57cffc0480d66257150a8450ba90563fda5153742784831d38abcbc0b099"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request* value) {
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/IsProgramSaved_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/IsProgramSaved_Response.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_program_name_text;
/** Complete ROS2 ur_dashboard_msgs/srv/IsProgramSaved_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_answer_text answer;
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_program_name_text program_name;
  /** Declared field program_saved; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t program_saved;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMSAVED_RESPONSE_MAX_WIRE_BYTES 522u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMSAVED_RESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_ISPROGRAMSAVED_RESPONSE_CODEC_IDENTITY "UWVMROS1_aa171247f31fd00a1b8820ee21cf20d159bd10d0d2887dc96b3a4dd4d372d115"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->program_saved>1)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=4+value->program_name.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->program_saved,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->program_saved,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>522u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_IsProgramSaved_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/Load_Request.filename; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_filename_text;
/** Complete ROS2 ur_dashboard_msgs/srv/Load_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request {
  /** Declared field filename; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_filename_text filename;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_LOAD_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_LOAD_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_LOAD_REQUEST_CODEC_IDENTITY "UWVMROS1_1d0cd6cdeaaafce999a7e9924721246b9f4954f22da40fd0a27fb7b657324876"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request const* value) {
  if(!value)return 0;
  if(value->filename.size>256u||!uwvm_bounded_utf8(value->filename.data,value->filename.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request const* value) {size_t n=0;
  n+=4+value->filename.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->filename.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->filename.data,value->filename.size);*offset+=value->filename.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->filename.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->filename.size>256u||value->filename.size>n-*offset)return 0;uwvm_bounded_copy(value->filename.data,p+*offset,value->filename.size);*offset+=value->filename.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/Load_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/Load_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_LOAD_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_LOAD_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_LOAD_RESPONSE_CODEC_IDENTITY "UWVMROS1_4e694de8079133a8e789add3640dfe19805cad92991a650167c2aae7067da415"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Load_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/Popup_Request.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_message_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/Popup_Request.title; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_title_text;
/** Complete ROS2 ur_dashboard_msgs/srv/Popup_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request {
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_message_text message;
  /** Declared field title; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_title_text title;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_POPUP_REQUEST_MAX_WIRE_BYTES 520u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_POPUP_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_POPUP_REQUEST_CODEC_IDENTITY "UWVMROS1_369e6eab4f689a846c52fcc5cb539da63fab3939fab254dd1e04c7b274d5f770"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request const* value) {
  if(!value)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  if(value->title.size>256u||!uwvm_bounded_utf8(value->title.data,value->title.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request const* value) {size_t n=0;
  n+=4+value->message.size;
  n+=4+value->title.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
  uwvm_bounded_put(p+*offset,value->title.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->title.data,value->title.size);*offset+=value->title.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  if(*offset>n||n-*offset<4)return 0;value->title.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->title.size>256u||value->title.size>n-*offset)return 0;uwvm_bounded_copy(value->title.data,p+*offset,value->title.size);*offset+=value->title.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>520u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/Popup_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/Popup_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_POPUP_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_POPUP_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_POPUP_RESPONSE_CODEC_IDENTITY "UWVMROS1_0b54f7c477c1ddf8ccb18d12d50eaf308225d4927ee377377ed354426e58f984"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_Popup_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/RawRequest_Request.query; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_query_text;
/** Complete ROS2 ur_dashboard_msgs/srv/RawRequest_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request {
  /** Declared field query; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_query_text query;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_RAWREQUEST_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_RAWREQUEST_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_RAWREQUEST_REQUEST_CODEC_IDENTITY "UWVMROS1_2bf66d87b20ca2de9ed94c04c79042a50239728320b43b27c9942405795b4555"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request const* value) {
  if(!value)return 0;
  if(value->query.size>256u||!uwvm_bounded_utf8(value->query.data,value->query.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request const* value) {size_t n=0;
  n+=4+value->query.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->query.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->query.data,value->query.size);*offset+=value->query.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->query.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->query.size>256u||value->query.size>n-*offset)return 0;uwvm_bounded_copy(value->query.data,p+*offset,value->query.size);*offset+=value->query.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/RawRequest_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/RawRequest_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_answer_text answer;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_RAWREQUEST_RESPONSE_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_RAWREQUEST_RESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_RAWREQUEST_RESPONSE_CODEC_IDENTITY "UWVMROS1_7c37f54149bab54f6e7bbfed41992eb54416b31346bc889a9632d5497111267f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_RawRequest_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/SetOperationalMode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request {
  /** Declared field operational_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode operational_mode;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETOPERATIONALMODE_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETOPERATIONALMODE_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETOPERATIONALMODE_REQUEST_CODEC_IDENTITY "UWVMROS1_8cc61f7c64211c46e01e52eee2c05f6e709f4f0900cf24fdfd4079b0f7581a00"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_valid(&value->operational_mode))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_wire_size(&value->operational_mode);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_write_fields(&value->operational_mode,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request* value) {
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_OperationalMode_read_fields(p,n,offset,&value->operational_mode))return 0;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/SetOperationalMode_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/SetOperationalMode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETOPERATIONALMODE_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETOPERATIONALMODE_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETOPERATIONALMODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_f5009de077224371e9d2f6c0aecbabd24c80148f2c7e1b1383a64f6f214b202a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetOperationalMode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_dashboard_msgs/srv/SetUserRole_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request {
  /** Declared field user_role; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole user_role;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETUSERROLE_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETUSERROLE_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETUSERROLE_REQUEST_CODEC_IDENTITY "UWVMROS1_7bd499947803efca81401a2de496d24e7e54ba97f4f86ec2b3089146a0655f5e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_valid(&value->user_role))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_wire_size(&value->user_role);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_write_fields(&value->user_role,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request* value) {
  if(!roswasmc_bounded_2_ur_dashboard_msgs_msg_UserRole_read_fields(p,n,offset,&value->user_role))return 0;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/SetUserRole_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_answer_text;
/** Complete ROS2 ur_dashboard_msgs/srv/SetUserRole_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_answer_text answer;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETUSERROLE_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETUSERROLE_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_SETUSERROLE_RESPONSE_CODEC_IDENTITY "UWVMROS1_4cab2bba5030f4e4b9e90252d64ec2e02d6eb87070b75cf43190fecd21da59fd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->answer.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_SetUserRole_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/UploadProgram_Request.file_path; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_file_path_text;
/** Complete ROS2 ur_dashboard_msgs/srv/UploadProgram_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request {
  /** Declared field file_path; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_file_path_text file_path;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_UPLOADPROGRAM_REQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_UPLOADPROGRAM_REQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_UPLOADPROGRAM_REQUEST_CODEC_IDENTITY "UWVMROS1_f7501d0115f8d36b1942ce610b1bbc197d0276de508ac8175d539f40494f437b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request const* value) {
  if(!value)return 0;
  if(value->file_path.size>256u||!uwvm_bounded_utf8(value->file_path.data,value->file_path.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request const* value) {size_t n=0;
  n+=4+value->file_path.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->file_path.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->file_path.data,value->file_path.size);*offset+=value->file_path.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request* value) {
  if(*offset>n||n-*offset<4)return 0;value->file_path.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->file_path.size>256u||value->file_path.size>n-*offset)return 0;uwvm_bounded_copy(value->file_path.data,p+*offset,value->file_path.size);*offset+=value->file_path.size;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/srv/UploadProgram_Response.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/srv/UploadProgram_Response.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_program_name_text;
/** Complete ROS2 ur_dashboard_msgs/srv/UploadProgram_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_answer_text answer;
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_program_name_text program_name;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response;
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_UPLOADPROGRAM_RESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_UPLOADPROGRAM_RESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UR_DASHBOARD_MSGS_SRV_UPLOADPROGRAM_RESPONSE_CODEC_IDENTITY "UWVMROS1_ae9a28c1cd4729f54968d18a16e2f8f417bdc5e9bc15809de7481f1f20e6b398"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_valid(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_wire_size(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=4+value->program_name.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_write_fields(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_encode(roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response* value) {
  roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_dashboard_msgs_srv_UploadProgram_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointTolerance data[6];} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_path_tolerance_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_control_msgs_msg_JointTolerance data[6];} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_goal_tolerance_sequence;
/** Complete ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal {
  /** Declared field trajectory; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory trajectory;
  /** Declared field path_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_path_tolerance_sequence path_tolerance;
  /** Declared field goal_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_goal_tolerance_sequence goal_tolerance;
  /** Declared field goal_time_tolerance; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration goal_time_tolerance;
  /** Declared field until_type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t until_type;
} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_GOAL_MAX_WIRE_BYTES 7053u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_GOAL_MIN_WIRE_BYTES 37u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_GOAL_CODEC_IDENTITY "UWVMROS1_78fda8f2d20c502e24b1ad4fe55f34bba41de20ff88e71b56d7a0631d7a3905a"
/** Original scalar constant TOOL_CONTACT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_GOAL_TOOL_CONTACT UINT32_C(0)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_valid(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_valid(&value->trajectory))return 0;
  if(value->path_tolerance.size>6u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value->path_tolerance.data[i]))return 0;
  }}
  if(value->goal_tolerance.size>6u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_valid(&value->goal_tolerance.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->goal_time_tolerance))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_wire_size(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal const* value) {size_t n=0;
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_wire_size(&value->trajectory);
  n+=4;
  if(value->path_tolerance.size>6u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value->path_tolerance.data[i]);
  }}
  n+=4;
  if(value->goal_tolerance.size>6u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  n+=roswasmc_bounded_2_control_msgs_msg_JointTolerance_wire_size(&value->goal_tolerance.data[i]);
  }}
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->goal_time_tolerance);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_write_fields(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_write_fields(&value->trajectory,p,offset);
  uwvm_bounded_put(p+*offset,value->path_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  roswasmc_bounded_2_control_msgs_msg_JointTolerance_write_fields(&value->path_tolerance.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->goal_tolerance.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  roswasmc_bounded_2_control_msgs_msg_JointTolerance_write_fields(&value->goal_tolerance.data[i],p,offset);
  }}
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->goal_time_tolerance,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->until_type,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal* value) {
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectory_read_fields(p,n,offset,&value->trajectory))return 0;
  if(*offset>n||n-*offset<4)return 0;value->path_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->path_tolerance.size>6u)return 0;
  {size_t i;for(i=0;i<value->path_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields(p,n,offset,&value->path_tolerance.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->goal_tolerance.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->goal_tolerance.size>6u)return 0;
  {size_t i;for(i=0;i<value->goal_tolerance.size;++i){
  if(!roswasmc_bounded_2_control_msgs_msg_JointTolerance_read_fields(p,n,offset,&value->goal_tolerance.data[i]))return 0;
  }}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->goal_time_tolerance))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->until_type,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_encode(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal* value) {
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<37u||length>7053u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_msgs/action/FollowJointTrajectoryUntil_Result.error_string; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_error_string_text;
/** Complete ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result {
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  int32_t error_code;
  /** Declared field until_condition_result; canonical capacity is part of CODEC_IDENTITY. */
  int32_t until_condition_result;
  /** Declared field error_string; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_error_string_text error_string;
} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_MAX_WIRE_BYTES 268u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_CODEC_IDENTITY "UWVMROS1_de92b9ccfb9c93f3afae9da5e8e31df9a6d6a606479fe27b9bcf14ff329bc869"
/** Original scalar constant SUCCESSFUL=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_SUCCESSFUL INT32_C(0)
/** Original scalar constant INVALID_GOAL=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_INVALID_GOAL -INT32_C(1)
/** Original scalar constant INVALID_JOINTS=-2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_INVALID_JOINTS -INT32_C(2)
/** Original scalar constant OLD_HEADER_TIMESTAMP=-3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_OLD_HEADER_TIMESTAMP -INT32_C(3)
/** Original scalar constant PATH_TOLERANCE_VIOLATED=-4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_PATH_TOLERANCE_VIOLATED -INT32_C(4)
/** Original scalar constant GOAL_TOLERANCE_VIOLATED=-5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_GOAL_TOLERANCE_VIOLATED -INT32_C(5)
/** Original scalar constant TRIGGERED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_TRIGGERED INT32_C(0)
/** Original scalar constant NOT_TRIGGERED=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_RESULT_NOT_TRIGGERED INT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_valid(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result const* value) {
  if(!value)return 0;
  if(value->error_string.size>256u||!uwvm_bounded_utf8(value->error_string.data,value->error_string.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_wire_size(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4+value->error_string.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_write_fields(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_code,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->until_condition_result,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->error_string.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->error_string.data,value->error_string.size);*offset+=value->error_string.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error_code,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->until_condition_result,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->error_string.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->error_string.size>256u||value->error_string.size>n-*offset)return 0;uwvm_bounded_copy(value->error_string.data,p+*offset,value->error_string.size);*offset+=value->error_string.size;
  return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_encode(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result* value) {
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>268u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_msgs/action/FollowJointTrajectoryUntil_Feedback.joint_names; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_joint_names_text;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_joint_names_text data[6];} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_joint_names_sequence;
/** Complete ROS2 ur_msgs/action/FollowJointTrajectoryUntil_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_std_msgs_msg_Header header;
  /** Declared field joint_names; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_joint_names_sequence joint_names;
  /** Declared field desired; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint desired;
  /** Declared field actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint actual;
  /** Declared field error; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint error;
} roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_FEEDBACK_MAX_WIRE_BYTES 2352u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_FEEDBACK_MIN_WIRE_BYTES 88u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_FOLLOWJOINTTRAJECTORYUNTIL_FEEDBACK_CODEC_IDENTITY "UWVMROS1_5a719f991f5c73784f9dc9e962ecfa0541e00daef7f747c312d4b633d63040ac"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_valid(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->joint_names.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(value->joint_names.data[i].size>256u||!uwvm_bounded_utf8(value->joint_names.data[i].data,value->joint_names.data[i].size))return 0;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_valid(&value->error))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_wire_size(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4;
  if(value->joint_names.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  n+=4+value->joint_names.data[i].size;
  }}
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->desired);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->actual);
  n+=roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_wire_size(&value->error);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_write_fields(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->joint_names.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  uwvm_bounded_put(p+*offset,value->joint_names.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->joint_names.data[i].data,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->desired,p,offset);
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->actual,p,offset);
  roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_write_fields(&value->error,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback* value) {
  if(!roswasmc_bounded_2_std_msgs_msg_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->joint_names.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_names.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->joint_names.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_names.data[i].size>256u||value->joint_names.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->joint_names.data[i].data,p+*offset,value->joint_names.data[i].size);*offset+=value->joint_names.data[i].size;
  }}
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->desired))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->actual))return 0;
  if(!roswasmc_bounded_2_trajectory_msgs_msg_JointTrajectoryPoint_read_fields(p,n,offset,&value->error))return 0;
  return roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_encode(roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback* value) {
  roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<88u||length>2352u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_FollowJointTrajectoryUntil_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_msgs/action/SendScript_Goal.program; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[8192];} roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_program_text;
/** UTF-8 byte storage for ur_msgs/action/SendScript_Goal.script_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_script_name_text;
/** Complete ROS2 ur_msgs/action/SendScript_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_SendScript_Goal {
  /** Declared field program; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_program_text program;
  /** Declared field script_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_script_name_text script_name;
  /** Declared field start_timeout; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration start_timeout;
  /** Declared field fail_on_warnings; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t fail_on_warnings;
} roswasmc_bounded_2_ur_msgs_action_SendScript_Goal;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_GOAL_MAX_WIRE_BYTES 8465u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_GOAL_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_GOAL_CODEC_IDENTITY "UWVMROS1_c277cd00663786f95201e61baa658b92def3becc1f37e54aa6093bb11135819c"
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_default(roswasmc_bounded_2_ur_msgs_action_SendScript_Goal* value) {
  roswasmc_bounded_2_ur_msgs_action_SendScript_Goal next={0};if(!value)return UWVM_BOUNDED_INVALID;
  next.script_name.size=0u;uwvm_bounded_copy(next.script_name.data,"",0u);
  next.fail_on_warnings=1;
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_valid(roswasmc_bounded_2_ur_msgs_action_SendScript_Goal const* value) {
  if(!value)return 0;
  if(value->program.size>8192u||!uwvm_bounded_utf8(value->program.data,value->program.size))return 0;
  if(value->script_name.size>256u||!uwvm_bounded_utf8(value->script_name.data,value->script_name.size))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->start_timeout))return 0;
  if(value->fail_on_warnings>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_wire_size(roswasmc_bounded_2_ur_msgs_action_SendScript_Goal const* value) {size_t n=0;
  n+=4+value->program.size;
  n+=4+value->script_name.size;
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->start_timeout);
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_write_fields(roswasmc_bounded_2_ur_msgs_action_SendScript_Goal const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->program.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program.data,value->program.size);*offset+=value->program.size;
  uwvm_bounded_put(p+*offset,value->script_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->script_name.data,value->script_name.size);*offset+=value->script_name.size;
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->start_timeout,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->fail_on_warnings,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_SendScript_Goal* value) {
  if(*offset>n||n-*offset<4)return 0;value->program.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program.size>8192u||value->program.size>n-*offset)return 0;uwvm_bounded_copy(value->program.data,p+*offset,value->program.size);*offset+=value->program.size;
  if(*offset>n||n-*offset<4)return 0;value->script_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->script_name.size>256u||value->script_name.size>n-*offset)return 0;uwvm_bounded_copy(value->script_name.data,p+*offset,value->script_name.size);*offset+=value->script_name.size;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->start_timeout))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->fail_on_warnings,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_encode(roswasmc_bounded_2_ur_msgs_action_SendScript_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_SendScript_Goal* value) {
  roswasmc_bounded_2_ur_msgs_action_SendScript_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>8465u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_SendScript_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_msgs/action/SendScript_Result.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_msgs_action_SendScript_Result_message_text;
/** Complete ROS2 ur_msgs/action/SendScript_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_SendScript_Result {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_action_SendScript_Result_message_text message;
} roswasmc_bounded_2_ur_msgs_action_SendScript_Result;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_RESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_RESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_RESULT_CODEC_IDENTITY "UWVMROS1_d4c4d9721c3a576f590ba4967b2444a82969d6ccba94ddbf8c4a974535ee17d2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_SendScript_Result_valid(roswasmc_bounded_2_ur_msgs_action_SendScript_Result const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_SendScript_Result_wire_size(roswasmc_bounded_2_ur_msgs_action_SendScript_Result const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_SendScript_Result_write_fields(roswasmc_bounded_2_ur_msgs_action_SendScript_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_SendScript_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_SendScript_Result* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_2_ur_msgs_action_SendScript_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_SendScript_Result_encode(roswasmc_bounded_2_ur_msgs_action_SendScript_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_SendScript_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_SendScript_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_SendScript_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_SendScript_Result_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_SendScript_Result* value) {
  roswasmc_bounded_2_ur_msgs_action_SendScript_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_SendScript_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/action/SendScript_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_FEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_FEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_SENDSCRIPT_FEEDBACK_CODEC_IDENTITY "UWVMROS1_094876d6467bd2b34f74cc6b574393117753add5254285e4e95d85b07328ef2c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_valid(roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_wire_size(roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_write_fields(roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback* value) {
  return roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_encode(roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback* value) {
  roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_SendScript_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/action/ToolContact_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_GOAL_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_GOAL_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_GOAL_CODEC_IDENTITY "UWVMROS1_150298ad16b1bc37d47bd3324cd4ec301ad8402c07779e8a435ce61daa7578dd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_valid(roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_wire_size(roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_write_fields(roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal* value) {
  return roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_encode(roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal* value) {
  roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_ToolContact_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/action/ToolContact_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_ToolContact_Result {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_msgs_action_ToolContact_Result;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_RESULT_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_RESULT_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_RESULT_CODEC_IDENTITY "UWVMROS1_b79f6080ac18b67c25131458a2af0a3c235d624c00fc5c13da534cc546c94578"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_valid(roswasmc_bounded_2_ur_msgs_action_ToolContact_Result const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_wire_size(roswasmc_bounded_2_ur_msgs_action_ToolContact_Result const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_write_fields(roswasmc_bounded_2_ur_msgs_action_ToolContact_Result const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_ToolContact_Result* value) {
  return roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_encode(roswasmc_bounded_2_ur_msgs_action_ToolContact_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_ToolContact_Result* value) {
  roswasmc_bounded_2_ur_msgs_action_ToolContact_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_ToolContact_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/action/ToolContact_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback;
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_FEEDBACK_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_FEEDBACK_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_ACTION_TOOLCONTACT_FEEDBACK_CODEC_IDENTITY "UWVMROS1_c6a11fd615f72a70a98514703de4c007a2bb161fbd8a953c5e8b899f471f4c5b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_valid(roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_wire_size(roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_write_fields(roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback* value) {
  return roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_encode(roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback* value) {
  roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_action_ToolContact_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/msg/Analog value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_Analog {
  /** Declared field pin; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t pin;
  /** Declared field domain; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t domain;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  float state;
} roswasmc_bounded_2_ur_msgs_msg_Analog;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ANALOG_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ANALOG_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ANALOG_CODEC_IDENTITY "UWVMROS1_68873e85836e1a65d7b67a52f5db8b3c3be082f889d2df4342386b282988f2a4"
/** Original scalar constant CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ANALOG_CURRENT UINT32_C(0)
/** Original scalar constant VOLTAGE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ANALOG_VOLTAGE UINT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_Analog_valid(roswasmc_bounded_2_ur_msgs_msg_Analog const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_Analog_wire_size(roswasmc_bounded_2_ur_msgs_msg_Analog const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_msg_Analog_write_fields(roswasmc_bounded_2_ur_msgs_msg_Analog const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->domain,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_Analog_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_Analog* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->domain,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->state,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_ur_msgs_msg_Analog_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_Analog_encode(roswasmc_bounded_2_ur_msgs_msg_Analog const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_Analog_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_Analog_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_Analog_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_Analog_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_Analog* value) {
  roswasmc_bounded_2_ur_msgs_msg_Analog next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_Analog_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/msg/Digital value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_Digital {
  /** Declared field pin; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t pin;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t state;
} roswasmc_bounded_2_ur_msgs_msg_Digital;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_DIGITAL_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_DIGITAL_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_DIGITAL_CODEC_IDENTITY "UWVMROS1_e5ebd0a17ee528cff1e0a636eb0be9c7e4128d3e77be722c5f1be73df4f4c11e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_Digital_valid(roswasmc_bounded_2_ur_msgs_msg_Digital const* value) {
  if(!value)return 0;
  if(value->state>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_Digital_wire_size(roswasmc_bounded_2_ur_msgs_msg_Digital const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_msg_Digital_write_fields(roswasmc_bounded_2_ur_msgs_msg_Digital const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->state,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_Digital_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_Digital* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->state,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_msg_Digital_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_Digital_encode(roswasmc_bounded_2_ur_msgs_msg_Digital const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_Digital_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_Digital_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_Digital_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_Digital_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_Digital* value) {
  roswasmc_bounded_2_ur_msgs_msg_Digital next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_Digital_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_viscous_scale_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_coulomb_scale_sequence;
/** Complete ROS2 ur_msgs/msg/FrictionModelParameters value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters {
  /** Declared field viscous_scale; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_viscous_scale_sequence viscous_scale;
  /** Declared field coulomb_scale; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_coulomb_scale_sequence coulomb_scale;
} roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_FRICTIONMODELPARAMETERS_MAX_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_FRICTIONMODELPARAMETERS_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_FRICTIONMODELPARAMETERS_CODEC_IDENTITY "UWVMROS1_7cc9c7af77340f56b0559b4e2ad32972ff65f2a6cf89ebfbe2c48c645f8e9c89"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_valid(roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters const* value) {
  if(!value)return 0;
  if(value->viscous_scale.size>6u)return 0;
  {size_t i;for(i=0;i<value->viscous_scale.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->viscous_scale.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->coulomb_scale.size>6u)return 0;
  {size_t i;for(i=0;i<value->coulomb_scale.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->coulomb_scale.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_wire_size(roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters const* value) {size_t n=0;
  n+=4;
  if(value->viscous_scale.size>6u)return 0;
  {size_t i;for(i=0;i<value->viscous_scale.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->coulomb_scale.size>6u)return 0;
  {size_t i;for(i=0;i<value->coulomb_scale.size;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_write_fields(roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->viscous_scale.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->viscous_scale.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->viscous_scale.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->coulomb_scale.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->coulomb_scale.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->coulomb_scale.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters* value) {
  if(*offset>n||n-*offset<4)return 0;value->viscous_scale.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->viscous_scale.size>6u)return 0;
  {size_t i;for(i=0;i<value->viscous_scale.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->viscous_scale.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->coulomb_scale.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->coulomb_scale.size>6u)return 0;
  {size_t i;for(i=0;i<value->coulomb_scale.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->coulomb_scale.data[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_encode(roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters* value) {
  roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_ur_msgs_msg_Digital data[32];} roswasmc_bounded_2_ur_msgs_msg_IOStates_digital_in_states_sequence;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_ur_msgs_msg_Digital data[32];} roswasmc_bounded_2_ur_msgs_msg_IOStates_digital_out_states_sequence;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_ur_msgs_msg_Digital data[32];} roswasmc_bounded_2_ur_msgs_msg_IOStates_flag_states_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_ur_msgs_msg_Analog data[16];} roswasmc_bounded_2_ur_msgs_msg_IOStates_analog_in_states_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_ur_msgs_msg_Analog data[16];} roswasmc_bounded_2_ur_msgs_msg_IOStates_analog_out_states_sequence;
/** Complete ROS2 ur_msgs/msg/IOStates value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_IOStates {
  /** Declared field digital_in_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_IOStates_digital_in_states_sequence digital_in_states;
  /** Declared field digital_out_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_IOStates_digital_out_states_sequence digital_out_states;
  /** Declared field flag_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_IOStates_flag_states_sequence flag_states;
  /** Declared field analog_in_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_IOStates_analog_in_states_sequence analog_in_states;
  /** Declared field analog_out_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_IOStates_analog_out_states_sequence analog_out_states;
} roswasmc_bounded_2_ur_msgs_msg_IOStates;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_IOSTATES_MAX_WIRE_BYTES 404u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_IOSTATES_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_IOSTATES_CODEC_IDENTITY "UWVMROS1_c22f9f784c89b6854fa84df416ed3704e3a312b15c8fc41162e7f5f53223fef5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_IOStates_valid(roswasmc_bounded_2_ur_msgs_msg_IOStates const* value) {
  if(!value)return 0;
  if(value->digital_in_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Digital_valid(&value->digital_in_states.data[i]))return 0;
  }}
  if(value->digital_out_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Digital_valid(&value->digital_out_states.data[i]))return 0;
  }}
  if(value->flag_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Digital_valid(&value->flag_states.data[i]))return 0;
  }}
  if(value->analog_in_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Analog_valid(&value->analog_in_states.data[i]))return 0;
  }}
  if(value->analog_out_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Analog_valid(&value->analog_out_states.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_IOStates_wire_size(roswasmc_bounded_2_ur_msgs_msg_IOStates const* value) {size_t n=0;
  n+=4;
  if(value->digital_in_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  n+=roswasmc_bounded_2_ur_msgs_msg_Digital_wire_size(&value->digital_in_states.data[i]);
  }}
  n+=4;
  if(value->digital_out_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  n+=roswasmc_bounded_2_ur_msgs_msg_Digital_wire_size(&value->digital_out_states.data[i]);
  }}
  n+=4;
  if(value->flag_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  n+=roswasmc_bounded_2_ur_msgs_msg_Digital_wire_size(&value->flag_states.data[i]);
  }}
  n+=4;
  if(value->analog_in_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  n+=roswasmc_bounded_2_ur_msgs_msg_Analog_wire_size(&value->analog_in_states.data[i]);
  }}
  n+=4;
  if(value->analog_out_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  n+=roswasmc_bounded_2_ur_msgs_msg_Analog_wire_size(&value->analog_out_states.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_msg_IOStates_write_fields(roswasmc_bounded_2_ur_msgs_msg_IOStates const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->digital_in_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  roswasmc_bounded_2_ur_msgs_msg_Digital_write_fields(&value->digital_in_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->digital_out_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  roswasmc_bounded_2_ur_msgs_msg_Digital_write_fields(&value->digital_out_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->flag_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  roswasmc_bounded_2_ur_msgs_msg_Digital_write_fields(&value->flag_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->analog_in_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  roswasmc_bounded_2_ur_msgs_msg_Analog_write_fields(&value->analog_in_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->analog_out_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  roswasmc_bounded_2_ur_msgs_msg_Analog_write_fields(&value->analog_out_states.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_IOStates_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_IOStates* value) {
  if(*offset>n||n-*offset<4)return 0;value->digital_in_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->digital_in_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Digital_read_fields(p,n,offset,&value->digital_in_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->digital_out_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->digital_out_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Digital_read_fields(p,n,offset,&value->digital_out_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->flag_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->flag_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Digital_read_fields(p,n,offset,&value->flag_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->analog_in_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->analog_in_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Analog_read_fields(p,n,offset,&value->analog_in_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->analog_out_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->analog_out_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  if(!roswasmc_bounded_2_ur_msgs_msg_Analog_read_fields(p,n,offset,&value->analog_out_states.data[i]))return 0;
  }}
  return roswasmc_bounded_2_ur_msgs_msg_IOStates_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_IOStates_encode(roswasmc_bounded_2_ur_msgs_msg_IOStates const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_IOStates_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_IOStates_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_IOStates_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_IOStates_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_IOStates* value) {
  roswasmc_bounded_2_ur_msgs_msg_IOStates next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>404u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_IOStates_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/msg/MasterboardDataMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg {
  /** Declared field digital_input_bits; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t digital_input_bits;
  /** Declared field digital_output_bits; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t digital_output_bits;
  /** Declared field analog_input_range0; canonical capacity is part of CODEC_IDENTITY. */
  int8_t analog_input_range0;
  /** Declared field analog_input_range1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t analog_input_range1;
  /** Declared field analog_input0; canonical capacity is part of CODEC_IDENTITY. */
  double analog_input0;
  /** Declared field analog_input1; canonical capacity is part of CODEC_IDENTITY. */
  double analog_input1;
  /** Declared field analog_output_domain0; canonical capacity is part of CODEC_IDENTITY. */
  int8_t analog_output_domain0;
  /** Declared field analog_output_domain1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t analog_output_domain1;
  /** Declared field analog_output0; canonical capacity is part of CODEC_IDENTITY. */
  double analog_output0;
  /** Declared field analog_output1; canonical capacity is part of CODEC_IDENTITY. */
  double analog_output1;
  /** Declared field masterboard_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float masterboard_temperature;
  /** Declared field robot_voltage_48v; canonical capacity is part of CODEC_IDENTITY. */
  float robot_voltage_48v;
  /** Declared field robot_current; canonical capacity is part of CODEC_IDENTITY. */
  float robot_current;
  /** Declared field master_io_current; canonical capacity is part of CODEC_IDENTITY. */
  float master_io_current;
  /** Declared field master_safety_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t master_safety_state;
  /** Declared field master_onoff_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t master_onoff_state;
} roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_MASTERBOARDDATAMSG_MAX_WIRE_BYTES 62u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_MASTERBOARDDATAMSG_MIN_WIRE_BYTES 62u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_MASTERBOARDDATAMSG_CODEC_IDENTITY "UWVMROS1_eb3b0aab27bb5683859356ca7cf9648e53d311c1fd32cac97f68ce1d4622a9e5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_valid(roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input0,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input1,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_output0,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_output1,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->masterboard_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_voltage_48v,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->master_io_current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_wire_size(roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  n+=8;
  n+=8;
  n+=1;
  n+=1;
  n+=8;
  n+=8;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_write_fields(roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->digital_input_bits,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->digital_output_bits,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->analog_input_range0,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->analog_input_range1,1);*offset+=1;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input0,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input1,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->analog_output_domain0,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->analog_output_domain1,1);*offset+=1;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_output0,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_output1,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->masterboard_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_voltage_48v,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->master_io_current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->master_safety_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->master_onoff_state,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->digital_input_bits,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->digital_output_bits,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_input_range0,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_input_range1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_input0,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_input1,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_output_domain0,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_output_domain1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_output0,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_output1,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->masterboard_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->robot_voltage_48v,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->robot_current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->master_io_current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->master_safety_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->master_onoff_state,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_encode(roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg* value) {
  roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<62u||length>62u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_MasterboardDataMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/msg/RobotModeDataMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg {
  /** Declared field timestamp; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t timestamp;
  /** Declared field is_robot_connected; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_robot_connected;
  /** Declared field is_real_robot_enabled; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_real_robot_enabled;
  /** Declared field is_power_on_robot; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_power_on_robot;
  /** Declared field is_emergency_stopped; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_emergency_stopped;
  /** Declared field is_protective_stopped; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_protective_stopped;
  /** Declared field is_program_running; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_program_running;
  /** Declared field is_program_paused; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t is_program_paused;
} roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ROBOTMODEDATAMSG_MAX_WIRE_BYTES 15u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ROBOTMODEDATAMSG_MIN_WIRE_BYTES 15u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ROBOTMODEDATAMSG_CODEC_IDENTITY "UWVMROS1_0181d85868dc6251289c6f7bc723757fa573c1345b6502497e335655d328bb31"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_valid(roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg const* value) {
  if(!value)return 0;
  if(value->is_robot_connected>1)return 0;
  if(value->is_real_robot_enabled>1)return 0;
  if(value->is_power_on_robot>1)return 0;
  if(value->is_emergency_stopped>1)return 0;
  if(value->is_protective_stopped>1)return 0;
  if(value->is_program_running>1)return 0;
  if(value->is_program_paused>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_wire_size(roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg const* value) {size_t n=0;
  n+=8;
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
static inline void roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_write_fields(roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->timestamp,8);*offset+=8;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_robot_connected,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_real_robot_enabled,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_power_on_robot,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_emergency_stopped,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_protective_stopped,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_program_running,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->is_program_paused,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->timestamp,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_robot_connected,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_real_robot_enabled,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_power_on_robot,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_emergency_stopped,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_protective_stopped,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_program_running,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_program_paused,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_encode(roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg* value) {
  roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<15u||length>15u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_RobotModeDataMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_q_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_qd_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_qdd_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_i_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_m_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_q_actual_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_qd_actual_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_i_actual_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tool_acc_values_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tcp_force_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tool_vector_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tcp_speed_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_motor_temperatures_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_joint_modes_sequence;
/** Complete ROS2 ur_msgs/msg/RobotStateRTMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg {
  /** Declared field time; canonical capacity is part of CODEC_IDENTITY. */
  double time;
  /** Declared field q_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_q_target_sequence q_target;
  /** Declared field qd_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_qd_target_sequence qd_target;
  /** Declared field qdd_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_qdd_target_sequence qdd_target;
  /** Declared field i_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_i_target_sequence i_target;
  /** Declared field m_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_m_target_sequence m_target;
  /** Declared field q_actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_q_actual_sequence q_actual;
  /** Declared field qd_actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_qd_actual_sequence qd_actual;
  /** Declared field i_actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_i_actual_sequence i_actual;
  /** Declared field tool_acc_values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tool_acc_values_sequence tool_acc_values;
  /** Declared field tcp_force; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tcp_force_sequence tcp_force;
  /** Declared field tool_vector; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tool_vector_sequence tool_vector;
  /** Declared field tcp_speed; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_tcp_speed_sequence tcp_speed;
  /** Declared field digital_input_bits; canonical capacity is part of CODEC_IDENTITY. */
  double digital_input_bits;
  /** Declared field motor_temperatures; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_motor_temperatures_sequence motor_temperatures;
  /** Declared field controller_timer; canonical capacity is part of CODEC_IDENTITY. */
  double controller_timer;
  /** Declared field test_value; canonical capacity is part of CODEC_IDENTITY. */
  double test_value;
  /** Declared field robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  double robot_mode;
  /** Declared field joint_modes; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_joint_modes_sequence joint_modes;
} roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ROBOTSTATERTMSG_MAX_WIRE_BYTES 768u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ROBOTSTATERTMSG_MIN_WIRE_BYTES 96u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_ROBOTSTATERTMSG_CODEC_IDENTITY "UWVMROS1_a30600c42e98b89da49d63073a5d30b0989aa0cb21f2bafec229dee10591d9cd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_valid(roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->time,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->q_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->q_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q_target.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->qd_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->qd_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->qd_target.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->qdd_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->qdd_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->qdd_target.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->i_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->i_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->i_target.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->m_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->m_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_target.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->q_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->q_actual.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q_actual.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->qd_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->qd_actual.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->qd_actual.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->i_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->i_actual.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->i_actual.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->tool_acc_values.size>6u)return 0;
  {size_t i;for(i=0;i<value->tool_acc_values.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tool_acc_values.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->tcp_force.size>6u)return 0;
  {size_t i;for(i=0;i<value->tcp_force.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tcp_force.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->tool_vector.size>6u)return 0;
  {size_t i;for(i=0;i<value->tool_vector.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tool_vector.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(value->tcp_speed.size>6u)return 0;
  {size_t i;for(i=0;i<value->tcp_speed.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tcp_speed.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->digital_input_bits,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->motor_temperatures.size>6u)return 0;
  {size_t i;for(i=0;i<value->motor_temperatures.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->motor_temperatures.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->controller_timer,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->test_value,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->robot_mode,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->joint_modes.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_modes.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_modes.data[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_wire_size(roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg const* value) {size_t n=0;
  n+=8;
  n+=4;
  if(value->q_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->q_target.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->qd_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->qd_target.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->qdd_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->qdd_target.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->i_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->i_target.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->m_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->m_target.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->q_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->q_actual.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->qd_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->qd_actual.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->i_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->i_actual.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->tool_acc_values.size>6u)return 0;
  {size_t i;for(i=0;i<value->tool_acc_values.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->tcp_force.size>6u)return 0;
  {size_t i;for(i=0;i<value->tcp_force.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->tool_vector.size>6u)return 0;
  {size_t i;for(i=0;i<value->tool_vector.size;++i){
  n+=8;
  }}
  n+=4;
  if(value->tcp_speed.size>6u)return 0;
  {size_t i;for(i=0;i<value->tcp_speed.size;++i){
  n+=8;
  }}
  n+=8;
  n+=4;
  if(value->motor_temperatures.size>6u)return 0;
  {size_t i;for(i=0;i<value->motor_temperatures.size;++i){
  n+=8;
  }}
  n+=8;
  n+=8;
  n+=8;
  n+=4;
  if(value->joint_modes.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_modes.size;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_write_fields(roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->time,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,value->q_target.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->q_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q_target.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->qd_target.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->qd_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->qd_target.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->qdd_target.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->qdd_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->qdd_target.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->i_target.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->i_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->i_target.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->m_target.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->m_target.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->m_target.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->q_actual.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->q_actual.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->q_actual.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->qd_actual.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->qd_actual.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->qd_actual.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->i_actual.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->i_actual.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->i_actual.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->tool_acc_values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->tool_acc_values.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tool_acc_values.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->tcp_force.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->tcp_force.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tcp_force.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->tool_vector.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->tool_vector.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tool_vector.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  uwvm_bounded_put(p+*offset,value->tcp_speed.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->tcp_speed.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->tcp_speed.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->digital_input_bits,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,value->motor_temperatures.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->motor_temperatures.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->motor_temperatures.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->controller_timer,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->test_value,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->robot_mode,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,value->joint_modes.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->joint_modes.size;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->joint_modes.data[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->time,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->q_target.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->q_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->q_target.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->q_target.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->qd_target.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->qd_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->qd_target.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->qd_target.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->qdd_target.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->qdd_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->qdd_target.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->qdd_target.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->i_target.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->i_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->i_target.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->i_target.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->m_target.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->m_target.size>6u)return 0;
  {size_t i;for(i=0;i<value->m_target.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->m_target.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->q_actual.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->q_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->q_actual.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->q_actual.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->qd_actual.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->qd_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->qd_actual.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->qd_actual.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->i_actual.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->i_actual.size>6u)return 0;
  {size_t i;for(i=0;i<value->i_actual.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->i_actual.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->tool_acc_values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->tool_acc_values.size>6u)return 0;
  {size_t i;for(i=0;i<value->tool_acc_values.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tool_acc_values.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->tcp_force.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->tcp_force.size>6u)return 0;
  {size_t i;for(i=0;i<value->tcp_force.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tcp_force.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->tool_vector.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->tool_vector.size>6u)return 0;
  {size_t i;for(i=0;i<value->tool_vector.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tool_vector.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<4)return 0;value->tcp_speed.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->tcp_speed.size>6u)return 0;
  {size_t i;for(i=0;i<value->tcp_speed.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->tcp_speed.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->digital_input_bits,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->motor_temperatures.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->motor_temperatures.size>6u)return 0;
  {size_t i;for(i=0;i<value->motor_temperatures.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->motor_temperatures.data[i],&bits,8);}*offset+=8;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->controller_timer,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->test_value,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->robot_mode,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->joint_modes.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->joint_modes.size>6u)return 0;
  {size_t i;for(i=0;i<value->joint_modes.size;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->joint_modes.data[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_encode(roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg* value) {
  roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<96u||length>768u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_RobotStateRTMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/msg/ToolDataMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg {
  /** Declared field analog_input_range2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t analog_input_range2;
  /** Declared field analog_input_range3; canonical capacity is part of CODEC_IDENTITY. */
  int8_t analog_input_range3;
  /** Declared field analog_input2; canonical capacity is part of CODEC_IDENTITY. */
  double analog_input2;
  /** Declared field analog_input3; canonical capacity is part of CODEC_IDENTITY. */
  double analog_input3;
  /** Declared field tool_voltage_48v; canonical capacity is part of CODEC_IDENTITY. */
  float tool_voltage_48v;
  /** Declared field tool_output_voltage; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t tool_output_voltage;
  /** Declared field tool_current; canonical capacity is part of CODEC_IDENTITY. */
  float tool_current;
  /** Declared field tool_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float tool_temperature;
  /** Declared field tool_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t tool_mode;
} roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg;
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_CODEC_IDENTITY "UWVMROS1_1eec69666515ffbdca162a10967fce03ee4c25f0c27d56c5a812b87c9a229aa0"
/** Original scalar constant ANALOG_INPUT_RANGE_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_ANALOG_INPUT_RANGE_CURRENT INT32_C(0)
/** Original scalar constant ANALOG_INPUT_RANGE_VOLTAGE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_ANALOG_INPUT_RANGE_VOLTAGE INT32_C(1)
/** Original scalar constant TOOL_BOOTLOADER_MODE=249; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_TOOL_BOOTLOADER_MODE UINT32_C(249)
/** Original scalar constant TOOL_RUNNING_MODE=253; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_TOOL_RUNNING_MODE UINT32_C(253)
/** Original scalar constant TOOL_IDLE_MODE=255; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_MSG_TOOLDATAMSG_TOOL_IDLE_MODE UINT32_C(255)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_valid(roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input2,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input3,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_voltage_48v,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_wire_size(roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=8;
  n+=8;
  n+=4;
  n+=1;
  n+=4;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_write_fields(roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->analog_input_range2,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->analog_input_range3,1);*offset+=1;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input2,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input3,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_voltage_48v,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->tool_output_voltage,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->tool_mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_input_range2,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_input_range3,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_input2,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_input3,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tool_voltage_48v,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tool_output_voltage,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tool_current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tool_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tool_mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_encode(roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg* value) {
  roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_msg_ToolDataMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/GetRobotSoftwareVersion_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_GETROBOTSOFTWAREVERSION_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_GETROBOTSOFTWAREVERSION_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_GETROBOTSOFTWAREVERSION_REQUEST_CODEC_IDENTITY "UWVMROS1_c2d78972cb5b63138d1e780e8ff0c082dd77d5b76b09642d2c655e9f16780101"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_valid(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request* value) {
  return roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_encode(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/GetRobotSoftwareVersion_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response {
  /** Declared field major; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t major;
  /** Declared field minor; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t minor;
  /** Declared field bugfix; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t bugfix;
  /** Declared field build; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t build;
} roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_GETROBOTSOFTWAREVERSION_RESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_GETROBOTSOFTWAREVERSION_RESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_GETROBOTSOFTWAREVERSION_RESPONSE_CODEC_IDENTITY "UWVMROS1_bbe9984c34d69f93ad8dbfe6059c6a2e1bf43dbd27eabd97cf0a9507308d814a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_valid(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->major,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->minor,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->bugfix,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->build,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->major,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->minor,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bugfix,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->build,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_encode(roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_GetRobotSoftwareVersion_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetAnalogOutput_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request {
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_Analog data;
} roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETANALOGOUTPUT_REQUEST_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETANALOGOUTPUT_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETANALOGOUTPUT_REQUEST_CODEC_IDENTITY "UWVMROS1_71c3eca5c6c90758f084e275fc495feb5d1c08a467c97d941fb5864c54309bea"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_valid(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_msgs_msg_Analog_valid(&value->data))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_msgs_msg_Analog_wire_size(&value->data);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_msgs_msg_Analog_write_fields(&value->data,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request* value) {
  if(!roswasmc_bounded_2_ur_msgs_msg_Analog_read_fields(p,n,offset,&value->data))return 0;
  return roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_encode(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetAnalogOutput_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETANALOGOUTPUT_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETANALOGOUTPUT_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETANALOGOUTPUT_RESPONSE_CODEC_IDENTITY "UWVMROS1_71a5f3580657cb3ca702dbb63b7af3330c922624c9e9774e47e6533e2fc0a05a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_valid(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_encode(roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetAnalogOutput_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetForceMode_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request {
  /** Declared field task_frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped task_frame;
  /** Declared field selection_vector_x; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t selection_vector_x;
  /** Declared field selection_vector_y; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t selection_vector_y;
  /** Declared field selection_vector_z; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t selection_vector_z;
  /** Declared field selection_vector_rx; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t selection_vector_rx;
  /** Declared field selection_vector_ry; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t selection_vector_ry;
  /** Declared field selection_vector_rz; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t selection_vector_rz;
  /** Declared field wrench; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Wrench wrench;
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t type;
  /** Declared field speed_limits; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Twist speed_limits;
  /** Declared field deviation_limits; canonical capacity is part of CODEC_IDENTITY. */
  float deviation_limits[6];
  /** Declared field damping_factor; canonical capacity is part of CODEC_IDENTITY. */
  float damping_factor;
  /** Declared field gain_scaling; canonical capacity is part of CODEC_IDENTITY. */
  float gain_scaling;
} roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_REQUEST_MAX_WIRE_BYTES 331u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_REQUEST_MIN_WIRE_BYTES 203u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_REQUEST_CODEC_IDENTITY "UWVMROS1_e43a6c22510a2078de3cb2dc17ba1b7184080a6aa29a1653b7986723c0cfef6e"
/** Original scalar constant TCP_TO_ORIGIN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_REQUEST_TCP_TO_ORIGIN UINT32_C(1)
/** Original scalar constant NO_TRANSFORM=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_REQUEST_NO_TRANSFORM UINT32_C(2)
/** Original scalar constant TCP_VELOCITY_TO_X_Y=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_REQUEST_TCP_VELOCITY_TO_X_Y UINT32_C(3)
/** Explicit declared-default initialization; ordinary C/C++ zero aggregates stay zero.
 * Nested/fixed elements are initialized independently; sequence lengths stay zero.
 * This initializer performs no I/O, registration, allocation or capability operation. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_default(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request next={0};if(!value)return UWVM_BOUNDED_INVALID;
  (void)roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_default(&next.task_frame);
  next.selection_vector_x=0;
  next.selection_vector_y=0;
  next.selection_vector_z=0;
  next.selection_vector_rx=0;
  next.selection_vector_ry=0;
  next.selection_vector_rz=0;
  next.type=UINT32_C(2);
  next.deviation_limits[0]=0x1.47ae140000000p-7F;
  next.deviation_limits[1]=0x1.47ae140000000p-7F;
  next.deviation_limits[2]=0x1.47ae140000000p-7F;
  next.deviation_limits[3]=0x1.47ae140000000p-7F;
  next.deviation_limits[4]=0x1.47ae140000000p-7F;
  next.deviation_limits[5]=0x1.47ae140000000p-7F;
  next.damping_factor=0x1.99999a0000000p-6F;
  next.gain_scaling=0x1.0000000000000p-1F;
  *value=next;return 0;
}
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_valid(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_valid(&value->task_frame))return 0;
  if(value->selection_vector_x>1)return 0;
  if(value->selection_vector_y>1)return 0;
  if(value->selection_vector_z>1)return 0;
  if(value->selection_vector_rx>1)return 0;
  if(value->selection_vector_ry>1)return 0;
  if(value->selection_vector_rz>1)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Wrench_valid(&value->wrench))return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_valid(&value->speed_limits))return 0;
  {size_t i;for(i=0;i<6u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->deviation_limits[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->damping_factor,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gain_scaling,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_wire_size(&value->task_frame);
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Wrench_wire_size(&value->wrench);
  n+=1;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Twist_wire_size(&value->speed_limits);
  {size_t i;for(i=0;i<6u;++i){
  n+=4;
  }}
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_write_fields(&value->task_frame,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_x,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_y,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_z,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_rx,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_ry,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_rz,1);*offset+=1;
  roswasmc_bounded_2_geometry_msgs_msg_Wrench_write_fields(&value->wrench,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->type,1);*offset+=1;
  roswasmc_bounded_2_geometry_msgs_msg_Twist_write_fields(&value->speed_limits,p,offset);
  {size_t i;for(i=0;i<6u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->deviation_limits[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->damping_factor,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gain_scaling,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_PoseStamped_read_fields(p,n,offset,&value->task_frame))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_x,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_y,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_z,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_rx,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_ry,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_rz,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Wrench_read_fields(p,n,offset,&value->wrench))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->type,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Twist_read_fields(p,n,offset,&value->speed_limits))return 0;
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->deviation_limits[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->damping_factor,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gain_scaling,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_encode(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<203u||length>331u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetForceMode_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFORCEMODE_RESPONSE_CODEC_IDENTITY "UWVMROS1_3f5438d426b7b68557ac8f43dc8efc2e2106782ec0cd4fd0dee7cdd69ddc7de8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_valid(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_encode(roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetForceMode_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetFrictionModelParameters_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request {
  /** Declared field parameters; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters parameters;
} roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFRICTIONMODELPARAMETERS_REQUEST_MAX_WIRE_BYTES 104u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFRICTIONMODELPARAMETERS_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFRICTIONMODELPARAMETERS_REQUEST_CODEC_IDENTITY "UWVMROS1_e44ad15f9c45d5a65eb13e6bc6e973bb20eebdafa04a5f2e8bcf4b0b54ce91c2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_valid(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_valid(&value->parameters))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_wire_size(&value->parameters);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_write_fields(&value->parameters,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request* value) {
  if(!roswasmc_bounded_2_ur_msgs_msg_FrictionModelParameters_read_fields(p,n,offset,&value->parameters))return 0;
  return roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_encode(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>104u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetFrictionModelParameters_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFRICTIONMODELPARAMETERS_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFRICTIONMODELPARAMETERS_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETFRICTIONMODELPARAMETERS_RESPONSE_CODEC_IDENTITY "UWVMROS1_1caca29dda3dc32360fd010dfb352b6feb54bba66ee16edd2f4353e3382207b3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_valid(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_encode(roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetFrictionModelParameters_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetGravity_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request {
  /** Declared field gravity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped gravity;
} roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETGRAVITY_REQUEST_MAX_WIRE_BYTES 164u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETGRAVITY_REQUEST_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETGRAVITY_REQUEST_CODEC_IDENTITY "UWVMROS1_3d7fed6ae8accbddd5b09f93c3e49f2b9f99cae4cf80a951eb31ace30ec5a834"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_valid(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_valid(&value->gravity))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_wire_size(&value->gravity);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_write_fields(&value->gravity,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request* value) {
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3Stamped_read_fields(p,n,offset,&value->gravity))return 0;
  return roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_encode(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>164u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetGravity_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_msgs/srv/SetGravity_Response.status; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_status_text;
/** Complete ROS2 ur_msgs/srv/SetGravity_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_status_text status;
} roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETGRAVITY_RESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETGRAVITY_RESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETGRAVITY_RESPONSE_CODEC_IDENTITY "UWVMROS1_84694df7fbb4fd15bfecaf1aae683ef1795b0c4941d1987d951467448da5f793"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_valid(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->status.size>256u||!uwvm_bounded_utf8(value->status.data,value->status.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response const* value) {size_t n=0;
  n+=1;
  n+=4+value->status.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->status.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->status.data,value->status.size);*offset+=value->status.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->status.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->status.size>256u||value->status.size>n-*offset)return 0;uwvm_bounded_copy(value->status.data,p+*offset,value->status.size);*offset+=value->status.size;
  return roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_encode(roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetGravity_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetIO_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetIO_Request {
  /** Declared field fun; canonical capacity is part of CODEC_IDENTITY. */
  int8_t fun;
  /** Declared field pin; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  float state;
} roswasmc_bounded_2_ur_msgs_srv_SetIO_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_CODEC_IDENTITY "UWVMROS1_c8ac4dddeb474521b4c701a0229e99af954416b7b8d7f32d2a19629d38b378c7"
/** Original scalar constant PIN_AOUT0=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_AOUT0 INT32_C(0)
/** Original scalar constant PIN_AOUT1=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_AOUT1 INT32_C(1)
/** Original scalar constant PIN_DOUT0=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT0 INT32_C(0)
/** Original scalar constant PIN_DOUT1=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT1 INT32_C(1)
/** Original scalar constant PIN_DOUT2=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT2 INT32_C(2)
/** Original scalar constant PIN_DOUT3=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT3 INT32_C(3)
/** Original scalar constant PIN_DOUT4=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT4 INT32_C(4)
/** Original scalar constant PIN_DOUT5=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT5 INT32_C(5)
/** Original scalar constant PIN_DOUT6=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT6 INT32_C(6)
/** Original scalar constant PIN_DOUT7=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_DOUT7 INT32_C(7)
/** Original scalar constant PIN_CONF_OUT0=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT0 INT32_C(8)
/** Original scalar constant PIN_CONF_OUT1=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT1 INT32_C(9)
/** Original scalar constant PIN_CONF_OUT2=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT2 INT32_C(10)
/** Original scalar constant PIN_CONF_OUT3=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT3 INT32_C(11)
/** Original scalar constant PIN_CONF_OUT4=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT4 INT32_C(12)
/** Original scalar constant PIN_CONF_OUT5=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT5 INT32_C(13)
/** Original scalar constant PIN_CONF_OUT6=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT6 INT32_C(14)
/** Original scalar constant PIN_CONF_OUT7=15; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_CONF_OUT7 INT32_C(15)
/** Original scalar constant PIN_TOOL_DOUT0=16; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_TOOL_DOUT0 INT32_C(16)
/** Original scalar constant PIN_TOOL_DOUT1=17; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_PIN_TOOL_DOUT1 INT32_C(17)
/** Original scalar constant FUN_SET_DIGITAL_OUT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_FUN_SET_DIGITAL_OUT INT32_C(1)
/** Original scalar constant FUN_SET_FLAG=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_FUN_SET_FLAG INT32_C(2)
/** Original scalar constant FUN_SET_ANALOG_OUT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_FUN_SET_ANALOG_OUT INT32_C(3)
/** Original scalar constant FUN_SET_TOOL_VOLTAGE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_FUN_SET_TOOL_VOLTAGE INT32_C(4)
/** Original scalar constant STATE_OFF=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_STATE_OFF INT32_C(0)
/** Original scalar constant STATE_ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_STATE_ON INT32_C(1)
/** Original scalar constant STATE_TOOL_VOLTAGE_0V=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_STATE_TOOL_VOLTAGE_0V INT32_C(0)
/** Original scalar constant STATE_TOOL_VOLTAGE_12V=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_STATE_TOOL_VOLTAGE_12V INT32_C(12)
/** Original scalar constant STATE_TOOL_VOLTAGE_24V=24; no instance/wire field. */
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_REQUEST_STATE_TOOL_VOLTAGE_24V INT32_C(24)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_valid(roswasmc_bounded_2_ur_msgs_srv_SetIO_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetIO_Request const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetIO_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->fun,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetIO_Request* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->fun,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->state,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_encode(roswasmc_bounded_2_ur_msgs_srv_SetIO_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetIO_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetIO_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetIO_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetIO_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetIO_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_msgs_srv_SetIO_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETIO_RESPONSE_CODEC_IDENTITY "UWVMROS1_bd32373034093ccdab140a5ba8ce8a420ef2ce441430e8306b2484f0ca535090"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_valid(roswasmc_bounded_2_ur_msgs_srv_SetIO_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetIO_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetIO_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetIO_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_encode(roswasmc_bounded_2_ur_msgs_srv_SetIO_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetIO_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetIO_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetIO_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetPayload_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field center_of_gravity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_geometry_msgs_msg_Vector3 center_of_gravity;
  /** Declared field ixx; canonical capacity is part of CODEC_IDENTITY. */
  double ixx;
  /** Declared field iyy; canonical capacity is part of CODEC_IDENTITY. */
  double iyy;
  /** Declared field izz; canonical capacity is part of CODEC_IDENTITY. */
  double izz;
  /** Declared field ixy; canonical capacity is part of CODEC_IDENTITY. */
  double ixy;
  /** Declared field ixz; canonical capacity is part of CODEC_IDENTITY. */
  double ixz;
  /** Declared field iyz; canonical capacity is part of CODEC_IDENTITY. */
  double iyz;
  /** Declared field transition_time; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_builtin_interfaces_msg_Duration transition_time;
} roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETPAYLOAD_REQUEST_MAX_WIRE_BYTES 84u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETPAYLOAD_REQUEST_MIN_WIRE_BYTES 84u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETPAYLOAD_REQUEST_CODEC_IDENTITY "UWVMROS1_e76e256e1accf2d742c4cf529e5af1e830db75f3db703a1f368d28077a87e735"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_valid(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_valid(&value->center_of_gravity))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixx,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyy,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->izz,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixy,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixz,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyz,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->transition_time))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request const* value) {size_t n=0;
  n+=4;
  n+=roswasmc_bounded_2_geometry_msgs_msg_Vector3_wire_size(&value->center_of_gravity);
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->transition_time);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_2_geometry_msgs_msg_Vector3_write_fields(&value->center_of_gravity,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixx,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyy,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->izz,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixy,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->ixz,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->iyz,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->transition_time,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_geometry_msgs_msg_Vector3_read_fields(p,n,offset,&value->center_of_gravity))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ixx,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->iyy,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->izz,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ixy,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->ixz,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->iyz,&bits,8);}*offset+=8;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_read_fields(p,n,offset,&value->transition_time))return 0;
  return roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_encode(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<84u||length>84u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetPayload_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetPayload_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETPAYLOAD_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETPAYLOAD_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETPAYLOAD_RESPONSE_CODEC_IDENTITY "UWVMROS1_036dbaa7d42b6cba6ad29a082e078401d507292483d260ba532f7d6947c1624c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_valid(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_encode(roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetPayload_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetSpeedSliderFraction_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request {
  /** Declared field speed_slider_fraction; canonical capacity is part of CODEC_IDENTITY. */
  double speed_slider_fraction;
} roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETSPEEDSLIDERFRACTION_REQUEST_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETSPEEDSLIDERFRACTION_REQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETSPEEDSLIDERFRACTION_REQUEST_CODEC_IDENTITY "UWVMROS1_d8f19fb240e4b70cad74c5111cdaebad14b480f21da953c95d474064b091abd8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_valid(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed_slider_fraction,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed_slider_fraction,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->speed_slider_fraction,&bits,8);}*offset+=8;
  return roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_encode(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 ur_msgs/srv/SetSpeedSliderFraction_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response;
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETSPEEDSLIDERFRACTION_RESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETSPEEDSLIDERFRACTION_RESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UR_MSGS_SRV_SETSPEEDSLIDERFRACTION_RESPONSE_CODEC_IDENTITY "UWVMROS1_1ab1ad5ab4b1ed302710ec055dff7644cc6e966af919ff311eaea6cddcf7dbb9"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_valid(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_wire_size(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_write_fields(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_encode(roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_decode(void const* input,size_t length,roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response* value) {
  roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_ur_msgs_srv_SetSpeedSliderFraction_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
