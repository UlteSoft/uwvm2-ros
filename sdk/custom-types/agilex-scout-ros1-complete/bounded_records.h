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
/** Complete ROS1 geometry_msgs/Point value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_Point {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  double x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  double y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  double z;
} roswasmc_bounded_1_geometry_msgs_Point;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POINT_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POINT_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POINT_CODEC_IDENTITY "UWVMROS1_b55e2bde60f6cedb426cada54bc7baefc536c7bc731bd49fb152d89a892de62b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_Point_valid(roswasmc_bounded_1_geometry_msgs_Point const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_Point_wire_size(roswasmc_bounded_1_geometry_msgs_Point const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_Point_write_fields(roswasmc_bounded_1_geometry_msgs_Point const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_Point_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_Point* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->x,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->y,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->z,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_geometry_msgs_Point_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Point_encode(roswasmc_bounded_1_geometry_msgs_Point const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_Point_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_Point_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_Point_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Point_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_Point* value) {
  roswasmc_bounded_1_geometry_msgs_Point next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_Point_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 geometry_msgs/Quaternion value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_Quaternion {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  double x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  double y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  double z;
  /** Declared field w; canonical capacity is part of CODEC_IDENTITY. */
  double w;
} roswasmc_bounded_1_geometry_msgs_Quaternion;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_QUATERNION_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_QUATERNION_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_QUATERNION_CODEC_IDENTITY "UWVMROS1_48fe20d5d29a8ca1a630605a58f80baacc52d9a65b1ca87efd50ffdd52882934"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_Quaternion_valid(roswasmc_bounded_1_geometry_msgs_Quaternion const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->w,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_Quaternion_wire_size(roswasmc_bounded_1_geometry_msgs_Quaternion const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_Quaternion_write_fields(roswasmc_bounded_1_geometry_msgs_Quaternion const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->w,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_Quaternion_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_Quaternion* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->x,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->y,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->z,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->w,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_geometry_msgs_Quaternion_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Quaternion_encode(roswasmc_bounded_1_geometry_msgs_Quaternion const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_Quaternion_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_Quaternion_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_Quaternion_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Quaternion_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_Quaternion* value) {
  roswasmc_bounded_1_geometry_msgs_Quaternion next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_Quaternion_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 geometry_msgs/Pose value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_Pose {
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Point position;
  /** Declared field orientation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Quaternion orientation;
} roswasmc_bounded_1_geometry_msgs_Pose;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSE_MAX_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSE_MIN_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSE_CODEC_IDENTITY "UWVMROS1_e49ed287cd0de31a16c8cd49ad8f41f81d44e23306b8dcb61da6e0e789595471"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_Pose_valid(roswasmc_bounded_1_geometry_msgs_Pose const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Point_valid(&value->position))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Quaternion_valid(&value->orientation))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_Pose_wire_size(roswasmc_bounded_1_geometry_msgs_Pose const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_Point_wire_size(&value->position);
  n+=roswasmc_bounded_1_geometry_msgs_Quaternion_wire_size(&value->orientation);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_Pose_write_fields(roswasmc_bounded_1_geometry_msgs_Pose const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_Point_write_fields(&value->position,p,offset);
  roswasmc_bounded_1_geometry_msgs_Quaternion_write_fields(&value->orientation,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_Pose_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_Pose* value) {
  if(!roswasmc_bounded_1_geometry_msgs_Point_read_fields(p,n,offset,&value->position))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Quaternion_read_fields(p,n,offset,&value->orientation))return 0;
  return roswasmc_bounded_1_geometry_msgs_Pose_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Pose_encode(roswasmc_bounded_1_geometry_msgs_Pose const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_Pose_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_Pose_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_Pose_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Pose_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_Pose* value) {
  roswasmc_bounded_1_geometry_msgs_Pose next={0};size_t offset=0;if(!value||(length&&!input)||length<56u||length>56u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_Pose_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 geometry_msgs/PoseWithCovariance value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_PoseWithCovariance {
  /** Declared field pose; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Pose pose;
  /** Declared field covariance; canonical capacity is part of CODEC_IDENTITY. */
  double covariance[36];
} roswasmc_bounded_1_geometry_msgs_PoseWithCovariance;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSEWITHCOVARIANCE_MAX_WIRE_BYTES 344u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSEWITHCOVARIANCE_MIN_WIRE_BYTES 344u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSEWITHCOVARIANCE_CODEC_IDENTITY "UWVMROS1_17b25963aafb3adfd0432febe040c9b38a531a7a667949b89e0f3937585ef8cf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_valid(roswasmc_bounded_1_geometry_msgs_PoseWithCovariance const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Pose_valid(&value->pose))return 0;
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_wire_size(roswasmc_bounded_1_geometry_msgs_PoseWithCovariance const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_Pose_wire_size(&value->pose);
  {size_t i;for(i=0;i<36u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_write_fields(roswasmc_bounded_1_geometry_msgs_PoseWithCovariance const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_Pose_write_fields(&value->pose,p,offset);
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_PoseWithCovariance* value) {
  if(!roswasmc_bounded_1_geometry_msgs_Pose_read_fields(p,n,offset,&value->pose))return 0;
  {size_t i;for(i=0;i<36u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_encode(roswasmc_bounded_1_geometry_msgs_PoseWithCovariance const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_PoseWithCovariance* value) {
  roswasmc_bounded_1_geometry_msgs_PoseWithCovariance next={0};size_t offset=0;if(!value||(length&&!input)||length<344u||length>344u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 geometry_msgs/Vector3 value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_Vector3 {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  double x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  double y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  double z;
} roswasmc_bounded_1_geometry_msgs_Vector3;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_VECTOR3_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_VECTOR3_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_VECTOR3_CODEC_IDENTITY "UWVMROS1_e90efcf4803d82763e0b83f8ef5502b5d042158873fcc91be32543d25d7632bc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_Vector3_valid(roswasmc_bounded_1_geometry_msgs_Vector3 const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(roswasmc_bounded_1_geometry_msgs_Vector3 const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(roswasmc_bounded_1_geometry_msgs_Vector3 const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->x,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->y,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->z,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_Vector3* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->x,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->y,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->z,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_geometry_msgs_Vector3_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Vector3_encode(roswasmc_bounded_1_geometry_msgs_Vector3 const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_Vector3_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Vector3_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_Vector3* value) {
  roswasmc_bounded_1_geometry_msgs_Vector3 next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 geometry_msgs/Twist value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_Twist {
  /** Declared field linear; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 linear;
  /** Declared field angular; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 angular;
} roswasmc_bounded_1_geometry_msgs_Twist;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWIST_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWIST_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWIST_CODEC_IDENTITY "UWVMROS1_d5a69cfff4380fcf3e9c131ddb3dfb6def041cd7308a7ebdbc150b6483dfd921"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_Twist_valid(roswasmc_bounded_1_geometry_msgs_Twist const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->linear))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->angular))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_Twist_wire_size(roswasmc_bounded_1_geometry_msgs_Twist const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->linear);
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->angular);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_Twist_write_fields(roswasmc_bounded_1_geometry_msgs_Twist const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->linear,p,offset);
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->angular,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_Twist_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_Twist* value) {
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->linear))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->angular))return 0;
  return roswasmc_bounded_1_geometry_msgs_Twist_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Twist_encode(roswasmc_bounded_1_geometry_msgs_Twist const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_Twist_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_Twist_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_Twist_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Twist_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_Twist* value) {
  roswasmc_bounded_1_geometry_msgs_Twist next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_Twist_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 geometry_msgs/TwistWithCovariance value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_TwistWithCovariance {
  /** Declared field twist; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Twist twist;
  /** Declared field covariance; canonical capacity is part of CODEC_IDENTITY. */
  double covariance[36];
} roswasmc_bounded_1_geometry_msgs_TwistWithCovariance;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWISTWITHCOVARIANCE_MAX_WIRE_BYTES 336u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWISTWITHCOVARIANCE_MIN_WIRE_BYTES 336u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWISTWITHCOVARIANCE_CODEC_IDENTITY "UWVMROS1_ff6790d181503b424d84726c8f4a7852cef14c6dd93e6d32cf3c37023f74bcdf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_valid(roswasmc_bounded_1_geometry_msgs_TwistWithCovariance const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Twist_valid(&value->twist))return 0;
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_wire_size(roswasmc_bounded_1_geometry_msgs_TwistWithCovariance const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_Twist_wire_size(&value->twist);
  {size_t i;for(i=0;i<36u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_write_fields(roswasmc_bounded_1_geometry_msgs_TwistWithCovariance const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_Twist_write_fields(&value->twist,p,offset);
  {size_t i;for(i=0;i<36u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_TwistWithCovariance* value) {
  if(!roswasmc_bounded_1_geometry_msgs_Twist_read_fields(p,n,offset,&value->twist))return 0;
  {size_t i;for(i=0;i<36u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_encode(roswasmc_bounded_1_geometry_msgs_TwistWithCovariance const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_TwistWithCovariance* value) {
  roswasmc_bounded_1_geometry_msgs_TwistWithCovariance next={0};size_t offset=0;if(!value||(length&&!input)||length<336u||length>336u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_1_std_msgs_Header_frame_id_text;
/** Complete ROS1 std_msgs/Header value; no field projects away native data. */
typedef struct roswasmc_bounded_1_std_msgs_Header {
  /** Declared field seq; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t seq;
  /** Declared field stamp; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_time stamp;
  /** Declared field frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header_frame_id_text frame_id;
} roswasmc_bounded_1_std_msgs_Header;
#define ROSWASMC_BOUNDED_1_STD_MSGS_HEADER_MAX_WIRE_BYTES 144u
#define ROSWASMC_BOUNDED_1_STD_MSGS_HEADER_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_STD_MSGS_HEADER_CODEC_IDENTITY "UWVMROS1_ee2b4e736d36ebcdb562d3ef7832777233d39d366837e9148a76aa2c36756435"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_std_msgs_Header_valid(roswasmc_bounded_1_std_msgs_Header const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_time_valid(&value->stamp))return 0;
  if(value->frame_id.size>128u||!uwvm_bounded_utf8(value->frame_id.data,value->frame_id.size))return 0;
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
  if(value->frame_id.size>128u||value->frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->frame_id.data,p+*offset,value->frame_id.size);*offset+=value->frame_id.size;
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
  roswasmc_bounded_1_std_msgs_Header next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>144u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for nav_msgs/Odometry.child_frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_1_nav_msgs_Odometry_child_frame_id_text;
/** Complete ROS1 nav_msgs/Odometry value; no field projects away native data. */
typedef struct roswasmc_bounded_1_nav_msgs_Odometry {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field child_frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_nav_msgs_Odometry_child_frame_id_text child_frame_id;
  /** Declared field pose; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_PoseWithCovariance pose;
  /** Declared field twist; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_TwistWithCovariance twist;
} roswasmc_bounded_1_nav_msgs_Odometry;
#define ROSWASMC_BOUNDED_1_NAV_MSGS_ODOMETRY_MAX_WIRE_BYTES 956u
#define ROSWASMC_BOUNDED_1_NAV_MSGS_ODOMETRY_MIN_WIRE_BYTES 700u
#define ROSWASMC_BOUNDED_1_NAV_MSGS_ODOMETRY_CODEC_IDENTITY "UWVMROS1_c10d3ce1f2824c33d728d0006fb83134330d0332af122f179cf2dac32181a2ca"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_nav_msgs_Odometry_valid(roswasmc_bounded_1_nav_msgs_Odometry const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->child_frame_id.size>128u||!uwvm_bounded_utf8(value->child_frame_id.data,value->child_frame_id.size))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_valid(&value->pose))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_valid(&value->twist))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_nav_msgs_Odometry_wire_size(roswasmc_bounded_1_nav_msgs_Odometry const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=4+value->child_frame_id.size;
  n+=roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_wire_size(&value->pose);
  n+=roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_wire_size(&value->twist);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_nav_msgs_Odometry_write_fields(roswasmc_bounded_1_nav_msgs_Odometry const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->child_frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->child_frame_id.data,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_write_fields(&value->pose,p,offset);
  roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_write_fields(&value->twist,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_nav_msgs_Odometry_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_nav_msgs_Odometry* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->child_frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->child_frame_id.size>128u||value->child_frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->child_frame_id.data,p+*offset,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  if(!roswasmc_bounded_1_geometry_msgs_PoseWithCovariance_read_fields(p,n,offset,&value->pose))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_TwistWithCovariance_read_fields(p,n,offset,&value->twist))return 0;
  return roswasmc_bounded_1_nav_msgs_Odometry_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_nav_msgs_Odometry_encode(roswasmc_bounded_1_nav_msgs_Odometry const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_nav_msgs_Odometry_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_nav_msgs_Odometry_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_nav_msgs_Odometry_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_nav_msgs_Odometry_decode(void const* input,size_t length,roswasmc_bounded_1_nav_msgs_Odometry* value) {
  roswasmc_bounded_1_nav_msgs_Odometry next={0};size_t offset=0;if(!value||(length&&!input)||length<700u||length>956u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_nav_msgs_Odometry_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 scout_msgs/ScoutBmsStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_scout_msgs_ScoutBmsStatus {
  /** Declared field SOC; canonical capacity is part of CODEC_IDENTITY. */
  double SOC;
  /** Declared field SOH; canonical capacity is part of CODEC_IDENTITY. */
  double SOH;
  /** Declared field battery_voltage; canonical capacity is part of CODEC_IDENTITY. */
  double battery_voltage;
  /** Declared field battery_current; canonical capacity is part of CODEC_IDENTITY. */
  double battery_current;
  /** Declared field battery_temperature; canonical capacity is part of CODEC_IDENTITY. */
  double battery_temperature;
  /** Declared field Alarm_Status_1; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Alarm_Status_1;
  /** Declared field Alarm_Status_2; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Alarm_Status_2;
  /** Declared field Warning_Status_1; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Warning_Status_1;
  /** Declared field Warning_Status_2; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t Warning_Status_2;
} roswasmc_bounded_1_scout_msgs_ScoutBmsStatus;
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTBMSSTATUS_MAX_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTBMSSTATUS_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTBMSSTATUS_CODEC_IDENTITY "UWVMROS1_6921538c08e86ab5b9ecdbf974c6cb5b580488f4ed96ccbdb8b7824d51e9cf95"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_valid(roswasmc_bounded_1_scout_msgs_ScoutBmsStatus const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->SOC,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->SOH,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_voltage,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_current,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_temperature,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_wire_size(roswasmc_bounded_1_scout_msgs_ScoutBmsStatus const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_write_fields(roswasmc_bounded_1_scout_msgs_ScoutBmsStatus const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->SOC,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->SOH,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_voltage,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_current,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_temperature,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->Alarm_Status_1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Alarm_Status_2,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Warning_Status_1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->Warning_Status_2,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_scout_msgs_ScoutBmsStatus* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->SOC,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->SOH,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->battery_voltage,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->battery_current,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->battery_temperature,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Alarm_Status_1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Alarm_Status_2,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Warning_Status_1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->Warning_Status_2,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_encode(roswasmc_bounded_1_scout_msgs_ScoutBmsStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_decode(void const* input,size_t length,roswasmc_bounded_1_scout_msgs_ScoutBmsStatus* value) {
  roswasmc_bounded_1_scout_msgs_ScoutBmsStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>44u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_scout_msgs_ScoutBmsStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 scout_msgs/ScoutDriverState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_scout_msgs_ScoutDriverState {
  /** Declared field driver_voltage; canonical capacity is part of CODEC_IDENTITY. */
  double driver_voltage;
  /** Declared field driver_temperature; canonical capacity is part of CODEC_IDENTITY. */
  double driver_temperature;
  /** Declared field driver_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t driver_state;
} roswasmc_bounded_1_scout_msgs_ScoutDriverState;
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTDRIVERSTATE_MAX_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTDRIVERSTATE_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTDRIVERSTATE_CODEC_IDENTITY "UWVMROS1_4c56c71ae621b2b8822d742894d19946492002453c93b72faccb4f2abc9d4275"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutDriverState_valid(roswasmc_bounded_1_scout_msgs_ScoutDriverState const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->driver_voltage,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->driver_temperature,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_scout_msgs_ScoutDriverState_wire_size(roswasmc_bounded_1_scout_msgs_ScoutDriverState const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_scout_msgs_ScoutDriverState_write_fields(roswasmc_bounded_1_scout_msgs_ScoutDriverState const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->driver_voltage,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->driver_temperature,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->driver_state,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutDriverState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_scout_msgs_ScoutDriverState* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->driver_voltage,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->driver_temperature,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->driver_state,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_scout_msgs_ScoutDriverState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutDriverState_encode(roswasmc_bounded_1_scout_msgs_ScoutDriverState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_scout_msgs_ScoutDriverState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_scout_msgs_ScoutDriverState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_scout_msgs_ScoutDriverState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutDriverState_decode(void const* input,size_t length,roswasmc_bounded_1_scout_msgs_ScoutDriverState* value) {
  roswasmc_bounded_1_scout_msgs_ScoutDriverState next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>17u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_scout_msgs_ScoutDriverState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 scout_msgs/ScoutLightCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_1_scout_msgs_ScoutLightCmd {
  /** Declared field enable_cmd_light_control; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t enable_cmd_light_control;
  /** Declared field front_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t front_mode;
  /** Declared field front_custom_value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t front_custom_value;
  /** Declared field rear_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rear_mode;
  /** Declared field rear_custom_value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t rear_custom_value;
} roswasmc_bounded_1_scout_msgs_ScoutLightCmd;
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTCMD_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTCMD_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTCMD_CODEC_IDENTITY "UWVMROS1_cf08cccf454f88111d5cc4a4b1ace08de72cf1ae9a9400c23a148126da95942a"
/** Original scalar constant LIGHT_CONST_OFF=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTCMD_LIGHT_CONST_OFF UINT32_C(0)
/** Original scalar constant LIGHT_CONST_ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTCMD_LIGHT_CONST_ON UINT32_C(1)
/** Original scalar constant LIGHT_BREATH=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTCMD_LIGHT_BREATH UINT32_C(2)
/** Original scalar constant LIGHT_CUSTOM=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTCMD_LIGHT_CUSTOM UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutLightCmd_valid(roswasmc_bounded_1_scout_msgs_ScoutLightCmd const* value) {
  if(!value)return 0;
  if(value->enable_cmd_light_control>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_scout_msgs_ScoutLightCmd_wire_size(roswasmc_bounded_1_scout_msgs_ScoutLightCmd const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_scout_msgs_ScoutLightCmd_write_fields(roswasmc_bounded_1_scout_msgs_ScoutLightCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->enable_cmd_light_control,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->front_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->front_custom_value,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rear_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->rear_custom_value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutLightCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_scout_msgs_ScoutLightCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->enable_cmd_light_control,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->front_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->front_custom_value,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rear_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->rear_custom_value,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_scout_msgs_ScoutLightCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutLightCmd_encode(roswasmc_bounded_1_scout_msgs_ScoutLightCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_scout_msgs_ScoutLightCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_scout_msgs_ScoutLightCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_scout_msgs_ScoutLightCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutLightCmd_decode(void const* input,size_t length,roswasmc_bounded_1_scout_msgs_ScoutLightCmd* value) {
  roswasmc_bounded_1_scout_msgs_ScoutLightCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_scout_msgs_ScoutLightCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 scout_msgs/ScoutLightState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_scout_msgs_ScoutLightState {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field custom_value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t custom_value;
} roswasmc_bounded_1_scout_msgs_ScoutLightState;
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTSTATE_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTSTATE_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTLIGHTSTATE_CODEC_IDENTITY "UWVMROS1_af55048f9200cfa80fc419664204a515f628cc610a352ab3ed8443dd7ac0ae51"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutLightState_valid(roswasmc_bounded_1_scout_msgs_ScoutLightState const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_scout_msgs_ScoutLightState_wire_size(roswasmc_bounded_1_scout_msgs_ScoutLightState const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_scout_msgs_ScoutLightState_write_fields(roswasmc_bounded_1_scout_msgs_ScoutLightState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->custom_value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutLightState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_scout_msgs_ScoutLightState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->custom_value,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_scout_msgs_ScoutLightState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutLightState_encode(roswasmc_bounded_1_scout_msgs_ScoutLightState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_scout_msgs_ScoutLightState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_scout_msgs_ScoutLightState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_scout_msgs_ScoutLightState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutLightState_decode(void const* input,size_t length,roswasmc_bounded_1_scout_msgs_ScoutLightState* value) {
  roswasmc_bounded_1_scout_msgs_ScoutLightState next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_scout_msgs_ScoutLightState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 scout_msgs/ScoutMotorState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_scout_msgs_ScoutMotorState {
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  double current;
  /** Declared field rpm; canonical capacity is part of CODEC_IDENTITY. */
  double rpm;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  double temperature;
  /** Declared field motor_pose; canonical capacity is part of CODEC_IDENTITY. */
  double motor_pose;
} roswasmc_bounded_1_scout_msgs_ScoutMotorState;
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTMOTORSTATE_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTMOTORSTATE_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTMOTORSTATE_CODEC_IDENTITY "UWVMROS1_ca67a704d7bb4ba6b2839db1fdb8a75bc753868ec871d101c24c85d8202a79b7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutMotorState_valid(roswasmc_bounded_1_scout_msgs_ScoutMotorState const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->rpm,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->temperature,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->motor_pose,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_scout_msgs_ScoutMotorState_wire_size(roswasmc_bounded_1_scout_msgs_ScoutMotorState const* value) {size_t n=0;
  n+=8;
  n+=8;
  n+=8;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_scout_msgs_ScoutMotorState_write_fields(roswasmc_bounded_1_scout_msgs_ScoutMotorState const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->current,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->rpm,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->temperature,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->motor_pose,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutMotorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_scout_msgs_ScoutMotorState* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->current,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->rpm,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->temperature,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->motor_pose,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_scout_msgs_ScoutMotorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutMotorState_encode(roswasmc_bounded_1_scout_msgs_ScoutMotorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_scout_msgs_ScoutMotorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_scout_msgs_ScoutMotorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_scout_msgs_ScoutMotorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutMotorState_decode(void const* input,size_t length,roswasmc_bounded_1_scout_msgs_ScoutMotorState* value) {
  roswasmc_bounded_1_scout_msgs_ScoutMotorState next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_scout_msgs_ScoutMotorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 scout_msgs/ScoutRsStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_scout_msgs_ScoutRsStatus {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field stick_left_h; canonical capacity is part of CODEC_IDENTITY. */
  int8_t stick_left_h;
  /** Declared field stick_left_v; canonical capacity is part of CODEC_IDENTITY. */
  int8_t stick_left_v;
  /** Declared field stick_right_h; canonical capacity is part of CODEC_IDENTITY. */
  int8_t stick_right_h;
  /** Declared field stick_right_v; canonical capacity is part of CODEC_IDENTITY. */
  int8_t stick_right_v;
  /** Declared field swa; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t swa;
  /** Declared field swb; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t swb;
  /** Declared field swc; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t swc;
  /** Declared field swd; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t swd;
  /** Declared field var_a; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t var_a;
} roswasmc_bounded_1_scout_msgs_ScoutRsStatus;
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTRSSTATUS_MAX_WIRE_BYTES 153u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTRSSTATUS_MIN_WIRE_BYTES 25u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTRSSTATUS_CODEC_IDENTITY "UWVMROS1_deb068b75fb629df3724c141ed3be9f6ee8a4aac7727cdaa28769cb61211f7de"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutRsStatus_valid(roswasmc_bounded_1_scout_msgs_ScoutRsStatus const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_scout_msgs_ScoutRsStatus_wire_size(roswasmc_bounded_1_scout_msgs_ScoutRsStatus const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
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
static inline void roswasmc_bounded_1_scout_msgs_ScoutRsStatus_write_fields(roswasmc_bounded_1_scout_msgs_ScoutRsStatus const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->stick_left_h,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->stick_left_v,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->stick_right_h,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->stick_right_v,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->swa,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->swb,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->swc,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->swd,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->var_a,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutRsStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_scout_msgs_ScoutRsStatus* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stick_left_h,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stick_left_v,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stick_right_h,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stick_right_v,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->swa,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->swb,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->swc,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->swd,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->var_a,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_scout_msgs_ScoutRsStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutRsStatus_encode(roswasmc_bounded_1_scout_msgs_ScoutRsStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_scout_msgs_ScoutRsStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_scout_msgs_ScoutRsStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_scout_msgs_ScoutRsStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutRsStatus_decode(void const* input,size_t length,roswasmc_bounded_1_scout_msgs_ScoutRsStatus* value) {
  roswasmc_bounded_1_scout_msgs_ScoutRsStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<25u||length>153u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_scout_msgs_ScoutRsStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 scout_msgs/ScoutStatus value; no field projects away native data. */
typedef struct roswasmc_bounded_1_scout_msgs_ScoutStatus {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field linear_velocity; canonical capacity is part of CODEC_IDENTITY. */
  double linear_velocity;
  /** Declared field angular_velocity; canonical capacity is part of CODEC_IDENTITY. */
  double angular_velocity;
  /** Declared field lateral_velocity; canonical capacity is part of CODEC_IDENTITY. */
  double lateral_velocity;
  /** Declared field base_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t base_state;
  /** Declared field control_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t control_mode;
  /** Declared field fault_code; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t fault_code;
  /** Declared field battery_voltage; canonical capacity is part of CODEC_IDENTITY. */
  double battery_voltage;
  /** Declared field motor_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_scout_msgs_ScoutMotorState motor_states[4];
  /** Declared field driver_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_scout_msgs_ScoutDriverState driver_states[4];
  /** Declared field light_control_enabled; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t light_control_enabled;
  /** Declared field front_light_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_scout_msgs_ScoutLightState front_light_state;
  /** Declared field rear_light_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_scout_msgs_ScoutLightState rear_light_state;
} roswasmc_bounded_1_scout_msgs_ScoutStatus;
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_MAX_WIRE_BYTES 381u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_MIN_WIRE_BYTES 253u
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_CODEC_IDENTITY "UWVMROS1_ff62854d4de0aae3dfbbc312822c6f912294f0feb04a7fff943df602d40bcbe2"
/** Original scalar constant MOTOR_ID_FRONT_RIGHT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_MOTOR_ID_FRONT_RIGHT INT32_C(0)
/** Original scalar constant MOTOR_ID_FRONT_LEFT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_MOTOR_ID_FRONT_LEFT INT32_C(1)
/** Original scalar constant MOTOR_ID_REAR_RIGHT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_MOTOR_ID_REAR_RIGHT INT32_C(2)
/** Original scalar constant MOTOR_ID_REAR_LEFT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_MOTOR_ID_REAR_LEFT INT32_C(3)
/** Original scalar constant LIGHT_ID_FRONT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_LIGHT_ID_FRONT INT32_C(0)
/** Original scalar constant LIGHT_ID_REAR=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SCOUT_MSGS_SCOUTSTATUS_LIGHT_ID_REAR INT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutStatus_valid(roswasmc_bounded_1_scout_msgs_ScoutStatus const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->linear_velocity,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->angular_velocity,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lateral_velocity,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_voltage,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_scout_msgs_ScoutMotorState_valid(&value->motor_states[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_scout_msgs_ScoutDriverState_valid(&value->driver_states[i]))return 0;
  }}
  if(value->light_control_enabled>1)return 0;
  if(!roswasmc_bounded_1_scout_msgs_ScoutLightState_valid(&value->front_light_state))return 0;
  if(!roswasmc_bounded_1_scout_msgs_ScoutLightState_valid(&value->rear_light_state))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_scout_msgs_ScoutStatus_wire_size(roswasmc_bounded_1_scout_msgs_ScoutStatus const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=8;
  n+=8;
  n+=8;
  n+=1;
  n+=1;
  n+=2;
  n+=8;
  {size_t i;for(i=0;i<4u;++i){
  n+=roswasmc_bounded_1_scout_msgs_ScoutMotorState_wire_size(&value->motor_states[i]);
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=roswasmc_bounded_1_scout_msgs_ScoutDriverState_wire_size(&value->driver_states[i]);
  }}
  n+=1;
  n+=roswasmc_bounded_1_scout_msgs_ScoutLightState_wire_size(&value->front_light_state);
  n+=roswasmc_bounded_1_scout_msgs_ScoutLightState_wire_size(&value->rear_light_state);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_scout_msgs_ScoutStatus_write_fields(roswasmc_bounded_1_scout_msgs_ScoutStatus const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->linear_velocity,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->angular_velocity,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->lateral_velocity,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->base_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->control_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->fault_code,2);*offset+=2;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->battery_voltage,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  {size_t i;for(i=0;i<4u;++i){
  roswasmc_bounded_1_scout_msgs_ScoutMotorState_write_fields(&value->motor_states[i],p,offset);
  }}
  {size_t i;for(i=0;i<4u;++i){
  roswasmc_bounded_1_scout_msgs_ScoutDriverState_write_fields(&value->driver_states[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->light_control_enabled,1);*offset+=1;
  roswasmc_bounded_1_scout_msgs_ScoutLightState_write_fields(&value->front_light_state,p,offset);
  roswasmc_bounded_1_scout_msgs_ScoutLightState_write_fields(&value->rear_light_state,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_scout_msgs_ScoutStatus_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_scout_msgs_ScoutStatus* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->linear_velocity,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->angular_velocity,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->lateral_velocity,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->base_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->control_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->fault_code,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->battery_voltage,&bits,8);}*offset+=8;
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_scout_msgs_ScoutMotorState_read_fields(p,n,offset,&value->motor_states[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_scout_msgs_ScoutDriverState_read_fields(p,n,offset,&value->driver_states[i]))return 0;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->light_control_enabled,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_1_scout_msgs_ScoutLightState_read_fields(p,n,offset,&value->front_light_state))return 0;
  if(!roswasmc_bounded_1_scout_msgs_ScoutLightState_read_fields(p,n,offset,&value->rear_light_state))return 0;
  return roswasmc_bounded_1_scout_msgs_ScoutStatus_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutStatus_encode(roswasmc_bounded_1_scout_msgs_ScoutStatus const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_scout_msgs_ScoutStatus_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_scout_msgs_ScoutStatus_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_scout_msgs_ScoutStatus_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_scout_msgs_ScoutStatus_decode(void const* input,size_t length,roswasmc_bounded_1_scout_msgs_ScoutStatus* value) {
  roswasmc_bounded_1_scout_msgs_ScoutStatus next={0};size_t offset=0;if(!value||(length&&!input)||length<253u||length>381u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_scout_msgs_ScoutStatus_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
