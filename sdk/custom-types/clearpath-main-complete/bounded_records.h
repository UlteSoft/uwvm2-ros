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
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_FANS_CODEC_IDENTITY "UWVMROS1_addc15a9b6db4f7a0eba3545f02ece0800e48796f71e387b4d249af861f95317"
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
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_LIGHTS_CODEC_IDENTITY "UWVMROS1_14da254234c9c897e39bf4ee0db8faad0798d23fe7017195f35f88fb6e0b3332"
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
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_POWER_CODEC_IDENTITY "UWVMROS1_e97ba89dea149ff43d0134038985ea45d2f072af592ea128aa5502a053478d22"
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
  /** Declared field pcb_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float pcb_temperature;
  /** Declared field mcu_temperature; canonical capacity is part of CODEC_IDENTITY. */
  float mcu_temperature;
} roswasmc_bounded_2_clearpath_platform_msgs_msg_Status;
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_MAX_WIRE_BYTES 684u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_2_CLEARPATH_PLATFORM_MSGS_MSG_STATUS_CODEC_IDENTITY "UWVMROS1_331afb70728e70e0391da1b2a0f2b97fdc8742a363a2b007606121ad778dd5c1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_valid(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_std_msgs_msg_Header_valid(&value->header))return 0;
  if(value->hardware_id.size>256u||!uwvm_bounded_utf8(value->hardware_id.data,value->hardware_id.size))return 0;
  if(value->firmware_version.size>256u||!uwvm_bounded_utf8(value->firmware_version.data,value->firmware_version.size))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->mcu_uptime))return 0;
  if(!roswasmc_bounded_2_builtin_interfaces_msg_Duration_valid(&value->connection_uptime))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pcb_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mcu_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_wire_size(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status const* value) {size_t n=0;
  n+=roswasmc_bounded_2_std_msgs_msg_Header_wire_size(&value->header);
  n+=4+value->hardware_id.size;
  n+=4+value->firmware_version.size;
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->mcu_uptime);
  n+=roswasmc_bounded_2_builtin_interfaces_msg_Duration_wire_size(&value->connection_uptime);
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_clearpath_platform_msgs_msg_Status_write_fields(roswasmc_bounded_2_clearpath_platform_msgs_msg_Status const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_std_msgs_msg_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->hardware_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->hardware_id.data,value->hardware_id.size);*offset+=value->hardware_id.size;
  uwvm_bounded_put(p+*offset,value->firmware_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->firmware_version.data,value->firmware_version.size);*offset+=value->firmware_version.size;
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->mcu_uptime,p,offset);
  roswasmc_bounded_2_builtin_interfaces_msg_Duration_write_fields(&value->connection_uptime,p,offset);
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pcb_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mcu_temperature,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
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
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pcb_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mcu_temperature,&bits,4);}*offset+=4;
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
  roswasmc_bounded_2_clearpath_platform_msgs_msg_Status next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>684u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
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
#undef UWVM_BOUNDED_ASSERT
#endif
