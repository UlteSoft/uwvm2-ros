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
/** Complete ROS1 geometry_msgs/Transform value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_Transform {
  /** Declared field translation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 translation;
  /** Declared field rotation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Quaternion rotation;
} roswasmc_bounded_1_geometry_msgs_Transform;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TRANSFORM_MAX_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TRANSFORM_MIN_WIRE_BYTES 56u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TRANSFORM_CODEC_IDENTITY "UWVMROS1_61aabab03e8f766ea05949c9e5f4989186d72a26f793b8d11752e53f1f300085"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_Transform_valid(roswasmc_bounded_1_geometry_msgs_Transform const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->translation))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Quaternion_valid(&value->rotation))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_Transform_wire_size(roswasmc_bounded_1_geometry_msgs_Transform const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->translation);
  n+=roswasmc_bounded_1_geometry_msgs_Quaternion_wire_size(&value->rotation);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_Transform_write_fields(roswasmc_bounded_1_geometry_msgs_Transform const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->translation,p,offset);
  roswasmc_bounded_1_geometry_msgs_Quaternion_write_fields(&value->rotation,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_Transform_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_Transform* value) {
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->translation))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Quaternion_read_fields(p,n,offset,&value->rotation))return 0;
  return roswasmc_bounded_1_geometry_msgs_Transform_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Transform_encode(roswasmc_bounded_1_geometry_msgs_Transform const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_Transform_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_Transform_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_Transform_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Transform_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_Transform* value) {
  roswasmc_bounded_1_geometry_msgs_Transform next={0};size_t offset=0;if(!value||(length&&!input)||length<56u||length>56u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_Transform_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** UTF-8 byte storage for geometry_msgs/TransformStamped.child_frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_geometry_msgs_TransformStamped_child_frame_id_text;
/** Complete ROS1 geometry_msgs/TransformStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_TransformStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field child_frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_TransformStamped_child_frame_id_text child_frame_id;
  /** Declared field transform; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Transform transform;
} roswasmc_bounded_1_geometry_msgs_TransformStamped;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TRANSFORMSTAMPED_MAX_WIRE_BYTES 460u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TRANSFORMSTAMPED_MIN_WIRE_BYTES 76u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TRANSFORMSTAMPED_CODEC_IDENTITY "UWVMROS1_9237b5dc5d3beacf4310244ce746a4210b9864855b611d1bf54cf23c28d7b79a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_TransformStamped_valid(roswasmc_bounded_1_geometry_msgs_TransformStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->child_frame_id.size>256u||!uwvm_bounded_utf8(value->child_frame_id.data,value->child_frame_id.size))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Transform_valid(&value->transform))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_TransformStamped_wire_size(roswasmc_bounded_1_geometry_msgs_TransformStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=4+value->child_frame_id.size;
  n+=roswasmc_bounded_1_geometry_msgs_Transform_wire_size(&value->transform);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_TransformStamped_write_fields(roswasmc_bounded_1_geometry_msgs_TransformStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  uwvm_bounded_put(p+*offset,value->child_frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->child_frame_id.data,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  roswasmc_bounded_1_geometry_msgs_Transform_write_fields(&value->transform,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_TransformStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_TransformStamped* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(*offset>n||n-*offset<4)return 0;value->child_frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->child_frame_id.size>256u||value->child_frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->child_frame_id.data,p+*offset,value->child_frame_id.size);*offset+=value->child_frame_id.size;
  if(!roswasmc_bounded_1_geometry_msgs_Transform_read_fields(p,n,offset,&value->transform))return 0;
  return roswasmc_bounded_1_geometry_msgs_TransformStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_TransformStamped_encode(roswasmc_bounded_1_geometry_msgs_TransformStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_TransformStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_TransformStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_TransformStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_TransformStamped_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_TransformStamped* value) {
  roswasmc_bounded_1_geometry_msgs_TransformStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<76u||length>460u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_TransformStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS1 geometry_msgs/TwistStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_TwistStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field twist; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Twist twist;
} roswasmc_bounded_1_geometry_msgs_TwistStamped;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWISTSTAMPED_MAX_WIRE_BYTES 192u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWISTSTAMPED_MIN_WIRE_BYTES 64u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_TWISTSTAMPED_CODEC_IDENTITY "UWVMROS1_27a31bcca6a50fbc6c6259b53d229872336a75628d6afd9de174eee2481946c7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_TwistStamped_valid(roswasmc_bounded_1_geometry_msgs_TwistStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Twist_valid(&value->twist))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_TwistStamped_wire_size(roswasmc_bounded_1_geometry_msgs_TwistStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=roswasmc_bounded_1_geometry_msgs_Twist_wire_size(&value->twist);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_TwistStamped_write_fields(roswasmc_bounded_1_geometry_msgs_TwistStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_1_geometry_msgs_Twist_write_fields(&value->twist,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_TwistStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_TwistStamped* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Twist_read_fields(p,n,offset,&value->twist))return 0;
  return roswasmc_bounded_1_geometry_msgs_TwistStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_TwistStamped_encode(roswasmc_bounded_1_geometry_msgs_TwistStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_TwistStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_TwistStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_TwistStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_TwistStamped_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_TwistStamped* value) {
  roswasmc_bounded_1_geometry_msgs_TwistStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<64u||length>192u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_TwistStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** UTF-8 byte storage for nav_msgs/Odometry.child_frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_nav_msgs_Odometry_child_frame_id_text;
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
#define ROSWASMC_BOUNDED_1_NAV_MSGS_ODOMETRY_MAX_WIRE_BYTES 1084u
#define ROSWASMC_BOUNDED_1_NAV_MSGS_ODOMETRY_MIN_WIRE_BYTES 700u
#define ROSWASMC_BOUNDED_1_NAV_MSGS_ODOMETRY_CODEC_IDENTITY "UWVMROS1_e0ff5eb7aef868891846ac7618249b12c18fcf0969653f24fdeaa7644e636ca0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_nav_msgs_Odometry_valid(roswasmc_bounded_1_nav_msgs_Odometry const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(value->child_frame_id.size>256u||!uwvm_bounded_utf8(value->child_frame_id.data,value->child_frame_id.size))return 0;
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
  if(value->child_frame_id.size>256u||value->child_frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->child_frame_id.data,p+*offset,value->child_frame_id.size);*offset+=value->child_frame_id.size;
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
  roswasmc_bounded_1_nav_msgs_Odometry next={0};size_t offset=0;if(!value||(length&&!input)||length<700u||length>1084u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_nav_msgs_Odometry_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[16];} roswasmc_bounded_1_sensor_msgs_BatteryState_cell_voltage_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[16];} roswasmc_bounded_1_sensor_msgs_BatteryState_cell_temperature_sequence;
/** UTF-8 byte storage for sensor_msgs/BatteryState.location; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_sensor_msgs_BatteryState_location_text;
/** UTF-8 byte storage for sensor_msgs/BatteryState.serial_number; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_sensor_msgs_BatteryState_serial_number_text;
/** Complete ROS1 sensor_msgs/BatteryState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_sensor_msgs_BatteryState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
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
  roswasmc_bounded_1_sensor_msgs_BatteryState_cell_voltage_sequence cell_voltage;
  /** Declared field cell_temperature; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_BatteryState_cell_temperature_sequence cell_temperature;
  /** Declared field location; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_BatteryState_location_text location;
  /** Declared field serial_number; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_BatteryState_serial_number_text serial_number;
} roswasmc_bounded_1_sensor_msgs_BatteryState;
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_MAX_WIRE_BYTES 832u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_MIN_WIRE_BYTES 64u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_CODEC_IDENTITY "UWVMROS1_4b176902362348cd3b7eec6ae21e2035d33df6990b4682ddf5123ef6e0339441"
/** Original scalar constant POWER_SUPPLY_STATUS_UNKNOWN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_STATUS_UNKNOWN UINT32_C(0)
/** Original scalar constant POWER_SUPPLY_STATUS_CHARGING=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_STATUS_CHARGING UINT32_C(1)
/** Original scalar constant POWER_SUPPLY_STATUS_DISCHARGING=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_STATUS_DISCHARGING UINT32_C(2)
/** Original scalar constant POWER_SUPPLY_STATUS_NOT_CHARGING=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_STATUS_NOT_CHARGING UINT32_C(3)
/** Original scalar constant POWER_SUPPLY_STATUS_FULL=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_STATUS_FULL UINT32_C(4)
/** Original scalar constant POWER_SUPPLY_HEALTH_UNKNOWN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_UNKNOWN UINT32_C(0)
/** Original scalar constant POWER_SUPPLY_HEALTH_GOOD=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_GOOD UINT32_C(1)
/** Original scalar constant POWER_SUPPLY_HEALTH_OVERHEAT=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_OVERHEAT UINT32_C(2)
/** Original scalar constant POWER_SUPPLY_HEALTH_DEAD=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_DEAD UINT32_C(3)
/** Original scalar constant POWER_SUPPLY_HEALTH_OVERVOLTAGE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_OVERVOLTAGE UINT32_C(4)
/** Original scalar constant POWER_SUPPLY_HEALTH_UNSPEC_FAILURE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_UNSPEC_FAILURE UINT32_C(5)
/** Original scalar constant POWER_SUPPLY_HEALTH_COLD=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_COLD UINT32_C(6)
/** Original scalar constant POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE UINT32_C(7)
/** Original scalar constant POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE UINT32_C(8)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_UNKNOWN=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_UNKNOWN UINT32_C(0)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_NIMH=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_NIMH UINT32_C(1)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LION=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LION UINT32_C(2)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LIPO=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LIPO UINT32_C(3)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LIFE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LIFE UINT32_C(4)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_NICD=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_NICD UINT32_C(5)
/** Original scalar constant POWER_SUPPLY_TECHNOLOGY_LIMN=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_BATTERYSTATE_POWER_SUPPLY_TECHNOLOGY_LIMN UINT32_C(6)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_sensor_msgs_BatteryState_valid(roswasmc_bounded_1_sensor_msgs_BatteryState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
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
static inline size_t roswasmc_bounded_1_sensor_msgs_BatteryState_wire_size(roswasmc_bounded_1_sensor_msgs_BatteryState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
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
static inline void roswasmc_bounded_1_sensor_msgs_BatteryState_write_fields(roswasmc_bounded_1_sensor_msgs_BatteryState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
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
static inline int roswasmc_bounded_1_sensor_msgs_BatteryState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_sensor_msgs_BatteryState* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
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
  return roswasmc_bounded_1_sensor_msgs_BatteryState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_BatteryState_encode(roswasmc_bounded_1_sensor_msgs_BatteryState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_sensor_msgs_BatteryState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_sensor_msgs_BatteryState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_sensor_msgs_BatteryState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_BatteryState_decode(void const* input,size_t length,roswasmc_bounded_1_sensor_msgs_BatteryState* value) {
  roswasmc_bounded_1_sensor_msgs_BatteryState next={0};size_t offset=0;if(!value||(length&&!input)||length<64u||length>832u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_sensor_msgs_BatteryState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 sensor_msgs/Imu value; no field projects away native data. */
typedef struct roswasmc_bounded_1_sensor_msgs_Imu {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field orientation; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Quaternion orientation;
  /** Declared field orientation_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double orientation_covariance[9];
  /** Declared field angular_velocity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 angular_velocity;
  /** Declared field angular_velocity_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double angular_velocity_covariance[9];
  /** Declared field linear_acceleration; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 linear_acceleration;
  /** Declared field linear_acceleration_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double linear_acceleration_covariance[9];
} roswasmc_bounded_1_sensor_msgs_Imu;
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_IMU_MAX_WIRE_BYTES 440u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_IMU_MIN_WIRE_BYTES 312u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_IMU_CODEC_IDENTITY "UWVMROS1_de9c3f5bb37e1dc20be73cb44f614a9ff8645c4cfbc305dc18425d8d9f130bc3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_sensor_msgs_Imu_valid(roswasmc_bounded_1_sensor_msgs_Imu const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Quaternion_valid(&value->orientation))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->orientation_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->angular_velocity))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->angular_velocity_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->linear_acceleration))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->linear_acceleration_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_sensor_msgs_Imu_wire_size(roswasmc_bounded_1_sensor_msgs_Imu const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=roswasmc_bounded_1_geometry_msgs_Quaternion_wire_size(&value->orientation);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->angular_velocity);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->linear_acceleration);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_sensor_msgs_Imu_write_fields(roswasmc_bounded_1_sensor_msgs_Imu const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_1_geometry_msgs_Quaternion_write_fields(&value->orientation,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->orientation_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->angular_velocity,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->angular_velocity_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->linear_acceleration,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->linear_acceleration_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_sensor_msgs_Imu_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_sensor_msgs_Imu* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Quaternion_read_fields(p,n,offset,&value->orientation))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->orientation_covariance[i],&bits,8);}*offset+=8;
  }}
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->angular_velocity))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->angular_velocity_covariance[i],&bits,8);}*offset+=8;
  }}
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->linear_acceleration))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->linear_acceleration_covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_sensor_msgs_Imu_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_Imu_encode(roswasmc_bounded_1_sensor_msgs_Imu const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_sensor_msgs_Imu_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_sensor_msgs_Imu_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_sensor_msgs_Imu_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_Imu_decode(void const* input,size_t length,roswasmc_bounded_1_sensor_msgs_Imu* value) {
  roswasmc_bounded_1_sensor_msgs_Imu next={0};size_t offset=0;if(!value||(length&&!input)||length<312u||length>440u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_sensor_msgs_Imu_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for sensor_msgs/JointState.name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_1_sensor_msgs_JointState_name_text;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_sensor_msgs_JointState_name_text data[2];} roswasmc_bounded_1_sensor_msgs_JointState_name_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[2];} roswasmc_bounded_1_sensor_msgs_JointState_position_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[2];} roswasmc_bounded_1_sensor_msgs_JointState_velocity_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[2];} roswasmc_bounded_1_sensor_msgs_JointState_effort_sequence;
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
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_JOINTSTATE_MAX_WIRE_BYTES 472u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_JOINTSTATE_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_JOINTSTATE_CODEC_IDENTITY "UWVMROS1_f27093d33cca1405ec35a9e525a95aa4002421a579cb78c6da3be3719f3cb66e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_sensor_msgs_JointState_valid(roswasmc_bounded_1_sensor_msgs_JointState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
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
static inline size_t roswasmc_bounded_1_sensor_msgs_JointState_wire_size(roswasmc_bounded_1_sensor_msgs_JointState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
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
  roswasmc_bounded_1_sensor_msgs_JointState next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>472u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_sensor_msgs_JointState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[1024];} roswasmc_bounded_1_sensor_msgs_LaserScan_ranges_sequence;
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[1024];} roswasmc_bounded_1_sensor_msgs_LaserScan_intensities_sequence;
/** Complete ROS1 sensor_msgs/LaserScan value; no field projects away native data. */
typedef struct roswasmc_bounded_1_sensor_msgs_LaserScan {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
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
  roswasmc_bounded_1_sensor_msgs_LaserScan_ranges_sequence ranges;
  /** Declared field intensities; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_sensor_msgs_LaserScan_intensities_sequence intensities;
} roswasmc_bounded_1_sensor_msgs_LaserScan;
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_LASERSCAN_MAX_WIRE_BYTES 8372u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_LASERSCAN_MIN_WIRE_BYTES 52u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_LASERSCAN_CODEC_IDENTITY "UWVMROS1_f7aee00335bf7d165004957149192f58abe5a27b5c4884bdf8d1ae9be06b366b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_sensor_msgs_LaserScan_valid(roswasmc_bounded_1_sensor_msgs_LaserScan const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
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
static inline size_t roswasmc_bounded_1_sensor_msgs_LaserScan_wire_size(roswasmc_bounded_1_sensor_msgs_LaserScan const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
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
static inline void roswasmc_bounded_1_sensor_msgs_LaserScan_write_fields(roswasmc_bounded_1_sensor_msgs_LaserScan const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
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
static inline int roswasmc_bounded_1_sensor_msgs_LaserScan_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_sensor_msgs_LaserScan* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
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
  return roswasmc_bounded_1_sensor_msgs_LaserScan_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_LaserScan_encode(roswasmc_bounded_1_sensor_msgs_LaserScan const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_sensor_msgs_LaserScan_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_sensor_msgs_LaserScan_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_sensor_msgs_LaserScan_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_LaserScan_decode(void const* input,size_t length,roswasmc_bounded_1_sensor_msgs_LaserScan* value) {
  roswasmc_bounded_1_sensor_msgs_LaserScan next={0};size_t offset=0;if(!value||(length&&!input)||length<52u||length>8372u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_sensor_msgs_LaserScan_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 sensor_msgs/MagneticField value; no field projects away native data. */
typedef struct roswasmc_bounded_1_sensor_msgs_MagneticField {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field magnetic_field; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 magnetic_field;
  /** Declared field magnetic_field_covariance; canonical capacity is part of CODEC_IDENTITY. */
  double magnetic_field_covariance[9];
} roswasmc_bounded_1_sensor_msgs_MagneticField;
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_MAGNETICFIELD_MAX_WIRE_BYTES 240u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_MAGNETICFIELD_MIN_WIRE_BYTES 112u
#define ROSWASMC_BOUNDED_1_SENSOR_MSGS_MAGNETICFIELD_CODEC_IDENTITY "UWVMROS1_1817b7ce8903797288ccf293373f81890c75af59d03d948339943a531ab754fe"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_sensor_msgs_MagneticField_valid(roswasmc_bounded_1_sensor_msgs_MagneticField const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->magnetic_field))return 0;
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->magnetic_field_covariance[i],8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_sensor_msgs_MagneticField_wire_size(roswasmc_bounded_1_sensor_msgs_MagneticField const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->magnetic_field);
  {size_t i;for(i=0;i<9u;++i){
  n+=8;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_sensor_msgs_MagneticField_write_fields(roswasmc_bounded_1_sensor_msgs_MagneticField const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->magnetic_field,p,offset);
  {size_t i;for(i=0;i<9u;++i){
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->magnetic_field_covariance[i],8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_sensor_msgs_MagneticField_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_sensor_msgs_MagneticField* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->magnetic_field))return 0;
  {size_t i;for(i=0;i<9u;++i){
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->magnetic_field_covariance[i],&bits,8);}*offset+=8;
  }}
  return roswasmc_bounded_1_sensor_msgs_MagneticField_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_MagneticField_encode(roswasmc_bounded_1_sensor_msgs_MagneticField const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_sensor_msgs_MagneticField_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_sensor_msgs_MagneticField_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_sensor_msgs_MagneticField_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_sensor_msgs_MagneticField_decode(void const* input,size_t length,roswasmc_bounded_1_sensor_msgs_MagneticField* value) {
  roswasmc_bounded_1_sensor_msgs_MagneticField next={0};size_t offset=0;if(!value||(length&&!input)||length<112u||length>240u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_sensor_msgs_MagneticField_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 std_srvs/SetBoolRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_std_srvs_SetBoolRequest {
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t data;
} roswasmc_bounded_1_std_srvs_SetBoolRequest;
#define ROSWASMC_BOUNDED_1_STD_SRVS_SETBOOLREQUEST_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_STD_SRVS_SETBOOLREQUEST_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_STD_SRVS_SETBOOLREQUEST_CODEC_IDENTITY "UWVMROS1_1621dbb16e2455a199cc871a0e784642a73bc57e76a4d524778163fab97f6aac"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_std_srvs_SetBoolRequest_valid(roswasmc_bounded_1_std_srvs_SetBoolRequest const* value) {
  if(!value)return 0;
  if(value->data>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_std_srvs_SetBoolRequest_wire_size(roswasmc_bounded_1_std_srvs_SetBoolRequest const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_std_srvs_SetBoolRequest_write_fields(roswasmc_bounded_1_std_srvs_SetBoolRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->data,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_std_srvs_SetBoolRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_std_srvs_SetBoolRequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->data,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_std_srvs_SetBoolRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_std_srvs_SetBoolRequest_encode(roswasmc_bounded_1_std_srvs_SetBoolRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_std_srvs_SetBoolRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_std_srvs_SetBoolRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_std_srvs_SetBoolRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_std_srvs_SetBoolRequest_decode(void const* input,size_t length,roswasmc_bounded_1_std_srvs_SetBoolRequest* value) {
  roswasmc_bounded_1_std_srvs_SetBoolRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_std_srvs_SetBoolRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for std_srvs/SetBoolResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_std_srvs_SetBoolResponse_message_text;
/** Complete ROS1 std_srvs/SetBoolResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_std_srvs_SetBoolResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_srvs_SetBoolResponse_message_text message;
} roswasmc_bounded_1_std_srvs_SetBoolResponse;
#define ROSWASMC_BOUNDED_1_STD_SRVS_SETBOOLRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_STD_SRVS_SETBOOLRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_STD_SRVS_SETBOOLRESPONSE_CODEC_IDENTITY "UWVMROS1_1e12b91618f540070e34e170e12db7659f8686cdf5fb8e1bf098b8d483522cd6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_std_srvs_SetBoolResponse_valid(roswasmc_bounded_1_std_srvs_SetBoolResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_std_srvs_SetBoolResponse_wire_size(roswasmc_bounded_1_std_srvs_SetBoolResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_std_srvs_SetBoolResponse_write_fields(roswasmc_bounded_1_std_srvs_SetBoolResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_std_srvs_SetBoolResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_std_srvs_SetBoolResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_std_srvs_SetBoolResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_std_srvs_SetBoolResponse_encode(roswasmc_bounded_1_std_srvs_SetBoolResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_std_srvs_SetBoolResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_std_srvs_SetBoolResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_std_srvs_SetBoolResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_std_srvs_SetBoolResponse_decode(void const* input,size_t length,roswasmc_bounded_1_std_srvs_SetBoolResponse* value) {
  roswasmc_bounded_1_std_srvs_SetBoolResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_std_srvs_SetBoolResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 std_srvs/TriggerRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_std_srvs_TriggerRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_std_srvs_TriggerRequest;
#define ROSWASMC_BOUNDED_1_STD_SRVS_TRIGGERREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_STD_SRVS_TRIGGERREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_STD_SRVS_TRIGGERREQUEST_CODEC_IDENTITY "UWVMROS1_d4e98dca9dc9a031d6b0f51937613f4f29c36156f0f05475339149794d47db1e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_std_srvs_TriggerRequest_valid(roswasmc_bounded_1_std_srvs_TriggerRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_std_srvs_TriggerRequest_wire_size(roswasmc_bounded_1_std_srvs_TriggerRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_std_srvs_TriggerRequest_write_fields(roswasmc_bounded_1_std_srvs_TriggerRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_std_srvs_TriggerRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_std_srvs_TriggerRequest* value) {
  return roswasmc_bounded_1_std_srvs_TriggerRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_std_srvs_TriggerRequest_encode(roswasmc_bounded_1_std_srvs_TriggerRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_std_srvs_TriggerRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_std_srvs_TriggerRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_std_srvs_TriggerRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_std_srvs_TriggerRequest_decode(void const* input,size_t length,roswasmc_bounded_1_std_srvs_TriggerRequest* value) {
  roswasmc_bounded_1_std_srvs_TriggerRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_std_srvs_TriggerRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for std_srvs/TriggerResponse.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_std_srvs_TriggerResponse_message_text;
/** Complete ROS1 std_srvs/TriggerResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_std_srvs_TriggerResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_srvs_TriggerResponse_message_text message;
} roswasmc_bounded_1_std_srvs_TriggerResponse;
#define ROSWASMC_BOUNDED_1_STD_SRVS_TRIGGERRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_STD_SRVS_TRIGGERRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_STD_SRVS_TRIGGERRESPONSE_CODEC_IDENTITY "UWVMROS1_e625281ddea64aff38d8647e2673b418b77474cae0501500b280cabb3d247d1d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_std_srvs_TriggerResponse_valid(roswasmc_bounded_1_std_srvs_TriggerResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_std_srvs_TriggerResponse_wire_size(roswasmc_bounded_1_std_srvs_TriggerResponse const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_std_srvs_TriggerResponse_write_fields(roswasmc_bounded_1_std_srvs_TriggerResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_std_srvs_TriggerResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_std_srvs_TriggerResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_std_srvs_TriggerResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_std_srvs_TriggerResponse_encode(roswasmc_bounded_1_std_srvs_TriggerResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_std_srvs_TriggerResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_std_srvs_TriggerResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_std_srvs_TriggerResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_std_srvs_TriggerResponse_decode(void const* input,size_t length,roswasmc_bounded_1_std_srvs_TriggerResponse* value) {
  roswasmc_bounded_1_std_srvs_TriggerResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_std_srvs_TriggerResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 turtlebot3_example/Turtlebot3Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal {
  /** Declared field goal; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 goal;
} roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal;
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3GOAL_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3GOAL_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3GOAL_CODEC_IDENTITY "UWVMROS1_f6eb642301e239005c1790cfd5b9ff00388d98d27a17cbb0edb103b6ac7bfad5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_valid(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->goal))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_wire_size(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->goal);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_write_fields(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->goal,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal* value) {
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->goal))return 0;
  return roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_encode(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_decode(void const* input,size_t length,roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal* value) {
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for turtlebot3_example/Turtlebot3Result.result; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_result_text;
/** Complete ROS1 turtlebot3_example/Turtlebot3Result value; no field projects away native data. */
typedef struct roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result {
  /** Declared field result; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_result_text result;
} roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result;
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3RESULT_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3RESULT_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3RESULT_CODEC_IDENTITY "UWVMROS1_c749a56ed9a0c74d1b167f69cc9b4435f9549d49144ef2951a4c3db501bad528"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_valid(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result const* value) {
  if(!value)return 0;
  if(value->result.size>256u||!uwvm_bounded_utf8(value->result.data,value->result.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_wire_size(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result const* value) {size_t n=0;
  n+=4+value->result.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_write_fields(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->result.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->result.data,value->result.size);*offset+=value->result.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result* value) {
  if(*offset>n||n-*offset<4)return 0;value->result.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->result.size>256u||value->result.size>n-*offset)return 0;uwvm_bounded_copy(value->result.data,p+*offset,value->result.size);*offset+=value->result.size;
  return roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_encode(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_decode(void const* input,size_t length,roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result* value) {
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for turtlebot3_example/Turtlebot3Feedback.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_state_text;
/** Complete ROS1 turtlebot3_example/Turtlebot3Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_state_text state;
} roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback;
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3FEEDBACK_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3FEEDBACK_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_EXAMPLE_TURTLEBOT3FEEDBACK_CODEC_IDENTITY "UWVMROS1_077b5593a1e4c23c89c4d6a87dc1d67d8fa1bf04fa3aa440f54c8eda60b688f8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_valid(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_wire_size(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback const* value) {size_t n=0;
  n+=4+value->state.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_write_fields(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  return roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_encode(roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_decode(void const* input,size_t length,roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback* value) {
  roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_turtlebot3_example_Turtlebot3Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 turtlebot3_msgs/SensorState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_turtlebot3_msgs_SensorState {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
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
} roswasmc_bounded_1_turtlebot3_msgs_SensorState;
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_MAX_WIRE_BYTES 172u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_CODEC_IDENTITY "UWVMROS1_0ef7dd1a4b965ae027b081a6b34f111731a3adfbabcc6fd547d4da7378fcb81f"
/** Original scalar constant BUMPER_FORWARD=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_BUMPER_FORWARD UINT32_C(1)
/** Original scalar constant BUMPER_BACKWARD=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_BUMPER_BACKWARD UINT32_C(2)
/** Original scalar constant CLIFF=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_CLIFF UINT32_C(1)
/** Original scalar constant SONAR=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_SONAR UINT32_C(1)
/** Original scalar constant ILLUMINATION=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_ILLUMINATION UINT32_C(1)
/** Original scalar constant BUTTON0=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_BUTTON0 UINT32_C(1)
/** Original scalar constant BUTTON1=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_BUTTON1 UINT32_C(2)
/** Original scalar constant ERROR_LEFT_MOTOR=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_ERROR_LEFT_MOTOR UINT32_C(1)
/** Original scalar constant ERROR_RIGHT_MOTOR=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_ERROR_RIGHT_MOTOR UINT32_C(2)
/** Original scalar constant TORQUE_ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_TORQUE_ON UINT32_C(1)
/** Original scalar constant TORQUE_OFF=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SENSORSTATE_TORQUE_OFF UINT32_C(2)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_turtlebot3_msgs_SensorState_valid(roswasmc_bounded_1_turtlebot3_msgs_SensorState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cliff,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->sonar,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->illumination,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(value->torque>1)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->battery,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_turtlebot3_msgs_SensorState_wire_size(roswasmc_bounded_1_turtlebot3_msgs_SensorState const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
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
static inline void roswasmc_bounded_1_turtlebot3_msgs_SensorState_write_fields(roswasmc_bounded_1_turtlebot3_msgs_SensorState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
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
static inline int roswasmc_bounded_1_turtlebot3_msgs_SensorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_turtlebot3_msgs_SensorState* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
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
  return roswasmc_bounded_1_turtlebot3_msgs_SensorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_msgs_SensorState_encode(roswasmc_bounded_1_turtlebot3_msgs_SensorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_turtlebot3_msgs_SensorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_turtlebot3_msgs_SensorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_turtlebot3_msgs_SensorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_msgs_SensorState_decode(void const* input,size_t length,roswasmc_bounded_1_turtlebot3_msgs_SensorState* value) {
  roswasmc_bounded_1_turtlebot3_msgs_SensorState next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>172u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_turtlebot3_msgs_SensorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 turtlebot3_msgs/Sound value; no field projects away native data. */
typedef struct roswasmc_bounded_1_turtlebot3_msgs_Sound {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
} roswasmc_bounded_1_turtlebot3_msgs_Sound;
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_CODEC_IDENTITY "UWVMROS1_0e5af3d5a2acbfff5bf5ba94c1520490fe281e70aff02cfb6ad31acd89e7536a"
/** Original scalar constant OFF=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_OFF UINT32_C(0)
/** Original scalar constant ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_ON UINT32_C(1)
/** Original scalar constant LOW_BATTERY=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_LOW_BATTERY UINT32_C(2)
/** Original scalar constant ERROR=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_ERROR UINT32_C(3)
/** Original scalar constant BUTTON1=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_BUTTON1 UINT32_C(4)
/** Original scalar constant BUTTON2=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_SOUND_BUTTON2 UINT32_C(5)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_turtlebot3_msgs_Sound_valid(roswasmc_bounded_1_turtlebot3_msgs_Sound const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_turtlebot3_msgs_Sound_wire_size(roswasmc_bounded_1_turtlebot3_msgs_Sound const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_turtlebot3_msgs_Sound_write_fields(roswasmc_bounded_1_turtlebot3_msgs_Sound const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_turtlebot3_msgs_Sound_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_turtlebot3_msgs_Sound* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_turtlebot3_msgs_Sound_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_msgs_Sound_encode(roswasmc_bounded_1_turtlebot3_msgs_Sound const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_turtlebot3_msgs_Sound_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_turtlebot3_msgs_Sound_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_turtlebot3_msgs_Sound_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_msgs_Sound_decode(void const* input,size_t length,roswasmc_bounded_1_turtlebot3_msgs_Sound* value) {
  roswasmc_bounded_1_turtlebot3_msgs_Sound next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_turtlebot3_msgs_Sound_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for turtlebot3_msgs/VersionInfo.hardware; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_hardware_text;
/** UTF-8 byte storage for turtlebot3_msgs/VersionInfo.firmware; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_firmware_text;
/** UTF-8 byte storage for turtlebot3_msgs/VersionInfo.software; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_software_text;
/** Complete ROS1 turtlebot3_msgs/VersionInfo value; no field projects away native data. */
typedef struct roswasmc_bounded_1_turtlebot3_msgs_VersionInfo {
  /** Declared field hardware; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_hardware_text hardware;
  /** Declared field firmware; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_firmware_text firmware;
  /** Declared field software; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_software_text software;
} roswasmc_bounded_1_turtlebot3_msgs_VersionInfo;
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_VERSIONINFO_MAX_WIRE_BYTES 780u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_VERSIONINFO_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_TURTLEBOT3_MSGS_VERSIONINFO_CODEC_IDENTITY "UWVMROS1_8f41d37cf43fda2e3649d85fcca83ad735dc5a8122f7829a98b18659bcad8b3a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_valid(roswasmc_bounded_1_turtlebot3_msgs_VersionInfo const* value) {
  if(!value)return 0;
  if(value->hardware.size>256u||!uwvm_bounded_utf8(value->hardware.data,value->hardware.size))return 0;
  if(value->firmware.size>256u||!uwvm_bounded_utf8(value->firmware.data,value->firmware.size))return 0;
  if(value->software.size>256u||!uwvm_bounded_utf8(value->software.data,value->software.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_wire_size(roswasmc_bounded_1_turtlebot3_msgs_VersionInfo const* value) {size_t n=0;
  n+=4+value->hardware.size;
  n+=4+value->firmware.size;
  n+=4+value->software.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_write_fields(roswasmc_bounded_1_turtlebot3_msgs_VersionInfo const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->hardware.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->hardware.data,value->hardware.size);*offset+=value->hardware.size;
  uwvm_bounded_put(p+*offset,value->firmware.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->firmware.data,value->firmware.size);*offset+=value->firmware.size;
  uwvm_bounded_put(p+*offset,value->software.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->software.data,value->software.size);*offset+=value->software.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_turtlebot3_msgs_VersionInfo* value) {
  if(*offset>n||n-*offset<4)return 0;value->hardware.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->hardware.size>256u||value->hardware.size>n-*offset)return 0;uwvm_bounded_copy(value->hardware.data,p+*offset,value->hardware.size);*offset+=value->hardware.size;
  if(*offset>n||n-*offset<4)return 0;value->firmware.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->firmware.size>256u||value->firmware.size>n-*offset)return 0;uwvm_bounded_copy(value->firmware.data,p+*offset,value->firmware.size);*offset+=value->firmware.size;
  if(*offset>n||n-*offset<4)return 0;value->software.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->software.size>256u||value->software.size>n-*offset)return 0;uwvm_bounded_copy(value->software.data,p+*offset,value->software.size);*offset+=value->software.size;
  return roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_encode(roswasmc_bounded_1_turtlebot3_msgs_VersionInfo const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_decode(void const* input,size_t length,roswasmc_bounded_1_turtlebot3_msgs_VersionInfo* value) {
  roswasmc_bounded_1_turtlebot3_msgs_VersionInfo next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>780u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_turtlebot3_msgs_VersionInfo_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
