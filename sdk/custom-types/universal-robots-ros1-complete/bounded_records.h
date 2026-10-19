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
/** Complete ROS1 geometry_msgs/PoseStamped value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_PoseStamped {
  /** Declared field header; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_std_msgs_Header header;
  /** Declared field pose; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Pose pose;
} roswasmc_bounded_1_geometry_msgs_PoseStamped;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSESTAMPED_MAX_WIRE_BYTES 200u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSESTAMPED_MIN_WIRE_BYTES 72u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_POSESTAMPED_CODEC_IDENTITY "UWVMROS1_bb59c4e82f561266c0f10500bfbbd1a809e9c1122ecbf559a812326af3eb2e95"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_PoseStamped_valid(roswasmc_bounded_1_geometry_msgs_PoseStamped const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_std_msgs_Header_valid(&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Pose_valid(&value->pose))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_PoseStamped_wire_size(roswasmc_bounded_1_geometry_msgs_PoseStamped const* value) {size_t n=0;
  n+=roswasmc_bounded_1_std_msgs_Header_wire_size(&value->header);
  n+=roswasmc_bounded_1_geometry_msgs_Pose_wire_size(&value->pose);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_PoseStamped_write_fields(roswasmc_bounded_1_geometry_msgs_PoseStamped const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_std_msgs_Header_write_fields(&value->header,p,offset);
  roswasmc_bounded_1_geometry_msgs_Pose_write_fields(&value->pose,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_PoseStamped_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_PoseStamped* value) {
  if(!roswasmc_bounded_1_std_msgs_Header_read_fields(p,n,offset,&value->header))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Pose_read_fields(p,n,offset,&value->pose))return 0;
  return roswasmc_bounded_1_geometry_msgs_PoseStamped_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_PoseStamped_encode(roswasmc_bounded_1_geometry_msgs_PoseStamped const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_PoseStamped_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_PoseStamped_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_PoseStamped_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_PoseStamped_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_PoseStamped* value) {
  roswasmc_bounded_1_geometry_msgs_PoseStamped next={0};size_t offset=0;if(!value||(length&&!input)||length<72u||length>200u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_PoseStamped_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
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
/** Complete ROS1 geometry_msgs/Wrench value; no field projects away native data. */
typedef struct roswasmc_bounded_1_geometry_msgs_Wrench {
  /** Declared field force; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 force;
  /** Declared field torque; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 torque;
} roswasmc_bounded_1_geometry_msgs_Wrench;
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_WRENCH_MAX_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_WRENCH_MIN_WIRE_BYTES 48u
#define ROSWASMC_BOUNDED_1_GEOMETRY_MSGS_WRENCH_CODEC_IDENTITY "UWVMROS1_b0938c4e084df68cf7c46445070ac3e9305d06f678275845aec6055db77617c3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_geometry_msgs_Wrench_valid(roswasmc_bounded_1_geometry_msgs_Wrench const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->force))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->torque))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_geometry_msgs_Wrench_wire_size(roswasmc_bounded_1_geometry_msgs_Wrench const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->force);
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->torque);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_geometry_msgs_Wrench_write_fields(roswasmc_bounded_1_geometry_msgs_Wrench const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->force,p,offset);
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->torque,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_geometry_msgs_Wrench_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_geometry_msgs_Wrench* value) {
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->force))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->torque))return 0;
  return roswasmc_bounded_1_geometry_msgs_Wrench_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Wrench_encode(roswasmc_bounded_1_geometry_msgs_Wrench const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_geometry_msgs_Wrench_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_geometry_msgs_Wrench_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_geometry_msgs_Wrench_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_geometry_msgs_Wrench_decode(void const* input,size_t length,roswasmc_bounded_1_geometry_msgs_Wrench* value) {
  roswasmc_bounded_1_geometry_msgs_Wrench next={0};size_t offset=0;if(!value||(length&&!input)||length<48u||length>48u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_geometry_msgs_Wrench_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/AddToLogRequest.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_message_text;
/** Complete ROS1 ur_dashboard_msgs/AddToLogRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest {
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_message_text message;
} roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ADDTOLOGREQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ADDTOLOGREQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ADDTOLOGREQUEST_CODEC_IDENTITY "UWVMROS1_faf6c1ab9c749a05ae3f4e6196cc1c3574560daae1463ebe8684bf13ea938ac4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest const* value) {
  if(!value)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest const* value) {size_t n=0;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_AddToLogRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/AddToLogResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/AddToLogResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ADDTOLOGRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ADDTOLOGRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ADDTOLOGRESPONSE_CODEC_IDENTITY "UWVMROS1_d495f7609d04abde44cb35ec5114618a2875e0a30986d2f7cc62c8ce99354e7c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_AddToLogResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/GetLoadedProgramRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETLOADEDPROGRAMREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETLOADEDPROGRAMREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETLOADEDPROGRAMREQUEST_CODEC_IDENTITY "UWVMROS1_54eecd0b652707f9c16dcbacbef05902112db993500404e1adaf26618e1d3a1a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest* value) {
  return roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/GetLoadedProgramResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/GetLoadedProgramResponse.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_program_name_text;
/** Complete ROS1 ur_dashboard_msgs/GetLoadedProgramResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_answer_text answer;
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_program_name_text program_name;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETLOADEDPROGRAMRESPONSE_MAX_WIRE_BYTES 521u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETLOADEDPROGRAMRESPONSE_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETLOADEDPROGRAMRESPONSE_CODEC_IDENTITY "UWVMROS1_1680747345d40412a4731c48f08c402ac97fb12dd871fb320c2a9d002453f87a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=4+value->program_name.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>521u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetLoadedProgramResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/GetProgramStateRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETPROGRAMSTATEREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETPROGRAMSTATEREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETPROGRAMSTATEREQUEST_CODEC_IDENTITY "UWVMROS1_cf14c6f86ef1904f80e88117dc2f2d29a003b52e21bc6569795771f04cf072a2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest* value) {
  return roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/ProgramState.state; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_state_text;
/** Complete ROS1 ur_dashboard_msgs/ProgramState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_ProgramState {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_state_text state;
} roswasmc_bounded_1_ur_dashboard_msgs_ProgramState;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_PROGRAMSTATE_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_PROGRAMSTATE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_PROGRAMSTATE_CODEC_IDENTITY "UWVMROS1_97a57db64f20abcb9929b5296c75d66d994f2e9e26449810197c657694d3850e"
/** Original scalar constant STOPPED=STOPPED; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_PROGRAMSTATE_STOPPED "\123\124\117\120\120\105\104"
/** Original scalar constant PLAYING=PLAYING; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_PROGRAMSTATE_PLAYING "\120\114\101\131\111\116\107"
/** Original scalar constant PAUSED=PAUSED; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_PROGRAMSTATE_PAUSED "\120\101\125\123\105\104"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_valid(roswasmc_bounded_1_ur_dashboard_msgs_ProgramState const* value) {
  if(!value)return 0;
  if(value->state.size>256u||!uwvm_bounded_utf8(value->state.data,value->state.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_ProgramState const* value) {size_t n=0;
  n+=4+value->state.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_ProgramState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->state.data,value->state.size);*offset+=value->state.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_ProgramState* value) {
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>256u||value->state.size>n-*offset)return 0;uwvm_bounded_copy(value->state.data,p+*offset,value->state.size);*offset+=value->state.size;
  return roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_encode(roswasmc_bounded_1_ur_dashboard_msgs_ProgramState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_ProgramState* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_ProgramState next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/GetProgramStateResponse.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_program_name_text;
/** UTF-8 byte storage for ur_dashboard_msgs/GetProgramStateResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/GetProgramStateResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse {
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_ProgramState state;
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_program_name_text program_name;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETPROGRAMSTATERESPONSE_MAX_WIRE_BYTES 781u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETPROGRAMSTATERESPONSE_MIN_WIRE_BYTES 13u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETPROGRAMSTATERESPONSE_CODEC_IDENTITY "UWVMROS1_366caf89303bfff48d3c0f89aa2a640c3d29af5a0c19d76ceccf26d50c3ab2ed"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_valid(&value->state))return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse const* value) {size_t n=0;
  n+=roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_wire_size(&value->state);
  n+=4+value->program_name.size;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_write_fields(&value->state,p,offset);
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse* value) {
  if(!roswasmc_bounded_1_ur_dashboard_msgs_ProgramState_read_fields(p,n,offset,&value->state))return 0;
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<13u||length>781u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetProgramStateResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/GetRobotModeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETROBOTMODEREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETROBOTMODEREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETROBOTMODEREQUEST_CODEC_IDENTITY "UWVMROS1_b9ffe283d7dbcb85879dbf6964d393f5d44661227bb26e1acc34308edb288609"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest* value) {
  return roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/RobotMode value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_RobotMode {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t mode;
} roswasmc_bounded_1_ur_dashboard_msgs_RobotMode;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_CODEC_IDENTITY "UWVMROS1_b2772853eff2ec9b849b6de4a90ce98487a8eff49d1371283dce503908702066"
/** Original scalar constant NO_CONTROLLER=-1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_NO_CONTROLLER -INT32_C(1)
/** Original scalar constant DISCONNECTED=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_DISCONNECTED INT32_C(0)
/** Original scalar constant CONFIRM_SAFETY=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_CONFIRM_SAFETY INT32_C(1)
/** Original scalar constant BOOTING=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_BOOTING INT32_C(2)
/** Original scalar constant POWER_OFF=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_POWER_OFF INT32_C(3)
/** Original scalar constant POWER_ON=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_POWER_ON INT32_C(4)
/** Original scalar constant IDLE=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_IDLE INT32_C(5)
/** Original scalar constant BACKDRIVE=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_BACKDRIVE INT32_C(6)
/** Original scalar constant RUNNING=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_RUNNING INT32_C(7)
/** Original scalar constant UPDATING_FIRMWARE=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ROBOTMODE_UPDATING_FIRMWARE INT32_C(8)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_valid(roswasmc_bounded_1_ur_dashboard_msgs_RobotMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_RobotMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_RobotMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_RobotMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_encode(roswasmc_bounded_1_ur_dashboard_msgs_RobotMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_RobotMode* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_RobotMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/GetRobotModeResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/GetRobotModeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse {
  /** Declared field robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_RobotMode robot_mode;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETROBOTMODERESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETROBOTMODERESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETROBOTMODERESPONSE_CODEC_IDENTITY "UWVMROS1_f6c0d0d03a956a34b00fd63540226b6abb8b106e2a35cb912b4886a5c4cecc3f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_valid(&value->robot_mode))return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse const* value) {size_t n=0;
  n+=roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_wire_size(&value->robot_mode);
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_write_fields(&value->robot_mode,p,offset);
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse* value) {
  if(!roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_read_fields(p,n,offset,&value->robot_mode))return 0;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetRobotModeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/GetSafetyModeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETSAFETYMODEREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETSAFETYMODEREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETSAFETYMODEREQUEST_CODEC_IDENTITY "UWVMROS1_f697e2ff2bcd56523563060a46bca2e1b7af487128f05562fcb18bfa73cbca8b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest* value) {
  return roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/SafetyMode value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
} roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_CODEC_IDENTITY "UWVMROS1_5a0c2737166cf2e2bd4c17b46c597f674b8d806b802d4e86ee0d76fc55f29cf7"
/** Original scalar constant NORMAL=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_NORMAL UINT32_C(1)
/** Original scalar constant REDUCED=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_REDUCED UINT32_C(2)
/** Original scalar constant PROTECTIVE_STOP=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_PROTECTIVE_STOP UINT32_C(3)
/** Original scalar constant RECOVERY=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_RECOVERY UINT32_C(4)
/** Original scalar constant SAFEGUARD_STOP=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_SAFEGUARD_STOP UINT32_C(5)
/** Original scalar constant SYSTEM_EMERGENCY_STOP=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_SYSTEM_EMERGENCY_STOP UINT32_C(6)
/** Original scalar constant ROBOT_EMERGENCY_STOP=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_ROBOT_EMERGENCY_STOP UINT32_C(7)
/** Original scalar constant VIOLATION=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_VIOLATION UINT32_C(8)
/** Original scalar constant FAULT=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_FAULT UINT32_C(9)
/** Original scalar constant VALIDATE_JOINT_ID=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_VALIDATE_JOINT_ID UINT32_C(10)
/** Original scalar constant UNDEFINED_SAFETY_MODE=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_UNDEFINED_SAFETY_MODE UINT32_C(11)
/** Original scalar constant AUTOMATIC_MODE_SAFEGUARD_STOP=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_AUTOMATIC_MODE_SAFEGUARD_STOP UINT32_C(12)
/** Original scalar constant SYSTEM_THREE_POSITION_ENABLING_STOP=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SAFETYMODE_SYSTEM_THREE_POSITION_ENABLING_STOP UINT32_C(13)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_valid(roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_encode(roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/GetSafetyModeResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/GetSafetyModeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse {
  /** Declared field safety_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode safety_mode;
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETSAFETYMODERESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETSAFETYMODERESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_GETSAFETYMODERESPONSE_CODEC_IDENTITY "UWVMROS1_70121af922588eaab9a34855e3da0cde837b1449d2a2d6c28addc70916df1a15"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_valid(&value->safety_mode))return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse const* value) {size_t n=0;
  n+=roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_wire_size(&value->safety_mode);
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_write_fields(&value->safety_mode,p,offset);
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse* value) {
  if(!roswasmc_bounded_1_ur_dashboard_msgs_SafetyMode_read_fields(p,n,offset,&value->safety_mode))return 0;
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_GetSafetyModeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/IsInRemoteControlRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISINREMOTECONTROLREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISINREMOTECONTROLREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISINREMOTECONTROLREQUEST_CODEC_IDENTITY "UWVMROS1_4b3c39f36f953904eb582d8c5f91161f2864782d4a4ab209a7dab32a117175da"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest* value) {
  return roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/IsInRemoteControlResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/IsInRemoteControlResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_answer_text answer;
  /** Declared field in_remote_control; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t in_remote_control;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISINREMOTECONTROLRESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISINREMOTECONTROLRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISINREMOTECONTROLRESPONSE_CODEC_IDENTITY "UWVMROS1_6c4af27571b48a5b8e8f1f857b2c02a333e7b4b360f8f3645a39630594c1663a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->in_remote_control>1)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->in_remote_control,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->in_remote_control,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_IsInRemoteControlResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/IsProgramRunningRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMRUNNINGREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMRUNNINGREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMRUNNINGREQUEST_CODEC_IDENTITY "UWVMROS1_0be481297028886e15f07ace4178ca0aec595724cd83ab01d0155e7c6aa6666f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest* value) {
  return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/IsProgramRunningResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/IsProgramRunningResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_answer_text answer;
  /** Declared field program_running; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t program_running;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMRUNNINGRESPONSE_MAX_WIRE_BYTES 262u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMRUNNINGRESPONSE_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMRUNNINGRESPONSE_CODEC_IDENTITY "UWVMROS1_4d9de4f7e7071d142c09e3184d4a6f29d4044d003ad11ba3b72e1129ecf144ac"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->program_running>1)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->program_running,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->program_running,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>262u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramRunningResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/IsProgramSavedRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMSAVEDREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMSAVEDREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMSAVEDREQUEST_CODEC_IDENTITY "UWVMROS1_45ce1466c35adc812096bfb8b2cd84dfa87940109831c9f797b5786afa14f5a8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest* value) {
  return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/IsProgramSavedResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_answer_text;
/** UTF-8 byte storage for ur_dashboard_msgs/IsProgramSavedResponse.program_name; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_program_name_text;
/** Complete ROS1 ur_dashboard_msgs/IsProgramSavedResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_answer_text answer;
  /** Declared field program_name; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_program_name_text program_name;
  /** Declared field program_saved; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t program_saved;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMSAVEDRESPONSE_MAX_WIRE_BYTES 522u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMSAVEDRESPONSE_MIN_WIRE_BYTES 10u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_ISPROGRAMSAVEDRESPONSE_CODEC_IDENTITY "UWVMROS1_eb53e077fe7b216e27fc57f838666a54a84e479e02e508d41df5cff9f0637a52"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->program_name.size>256u||!uwvm_bounded_utf8(value->program_name.data,value->program_name.size))return 0;
  if(value->program_saved>1)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=4+value->program_name.size;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,value->program_name.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->program_name.data,value->program_name.size);*offset+=value->program_name.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->program_saved,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<4)return 0;value->program_name.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->program_name.size>256u||value->program_name.size>n-*offset)return 0;uwvm_bounded_copy(value->program_name.data,p+*offset,value->program_name.size);*offset+=value->program_name.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->program_saved,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<10u||length>522u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_IsProgramSavedResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/LoadRequest.filename; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_filename_text;
/** Complete ROS1 ur_dashboard_msgs/LoadRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest {
  /** Declared field filename; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_filename_text filename;
} roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_LOADREQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_LOADREQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_LOADREQUEST_CODEC_IDENTITY "UWVMROS1_dfbde7b2376c7c9ba3926e4803f16b22f7ade3b2f768a25886a1490656edd2bd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest const* value) {
  if(!value)return 0;
  if(value->filename.size>256u||!uwvm_bounded_utf8(value->filename.data,value->filename.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest const* value) {size_t n=0;
  n+=4+value->filename.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->filename.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->filename.data,value->filename.size);*offset+=value->filename.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->filename.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->filename.size>256u||value->filename.size>n-*offset)return 0;uwvm_bounded_copy(value->filename.data,p+*offset,value->filename.size);*offset+=value->filename.size;
  return roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_LoadRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/LoadResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/LoadResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_LOADRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_LOADRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_LOADRESPONSE_CODEC_IDENTITY "UWVMROS1_0ed3b3f2a0066248734e64c0706e52ebf07113569a98baf249a99adfff59be02"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_LoadResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/PopupRequest.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_message_text;
/** Complete ROS1 ur_dashboard_msgs/PopupRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest {
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_message_text message;
} roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_POPUPREQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_POPUPREQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_POPUPREQUEST_CODEC_IDENTITY "UWVMROS1_11556bfb811a28141b6c5e9c11720920a389d735b8c09ed1a8f47d05f293e90d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest const* value) {
  if(!value)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest const* value) {size_t n=0;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_PopupRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/PopupResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/PopupResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_answer_text answer;
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_POPUPRESPONSE_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_POPUPRESPONSE_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_POPUPRESPONSE_CODEC_IDENTITY "UWVMROS1_33ac5b2128d6e7c2e562af7e2b8a8e907c16c07034b377920cba4fbd04ca9e14"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_PopupResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/RawRequestRequest.query; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_query_text;
/** Complete ROS1 ur_dashboard_msgs/RawRequestRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest {
  /** Declared field query; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_query_text query;
} roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_RAWREQUESTREQUEST_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_RAWREQUESTREQUEST_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_RAWREQUESTREQUEST_CODEC_IDENTITY "UWVMROS1_ca8e3fdb45888bf4feb3c0f4ed05b7b9499a74493aaa0a8727f9f513922adfb4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_valid(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest const* value) {
  if(!value)return 0;
  if(value->query.size>256u||!uwvm_bounded_utf8(value->query.data,value->query.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest const* value) {size_t n=0;
  n+=4+value->query.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->query.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->query.data,value->query.size);*offset+=value->query.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest* value) {
  if(*offset>n||n-*offset<4)return 0;value->query.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->query.size>256u||value->query.size>n-*offset)return 0;uwvm_bounded_copy(value->query.data,p+*offset,value->query.size);*offset+=value->query.size;
  return roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_encode(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_RawRequestRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/RawRequestResponse.answer; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_answer_text;
/** Complete ROS1 ur_dashboard_msgs/RawRequestResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse {
  /** Declared field answer; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_answer_text answer;
} roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_RAWREQUESTRESPONSE_MAX_WIRE_BYTES 260u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_RAWREQUESTRESPONSE_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_RAWREQUESTRESPONSE_CODEC_IDENTITY "UWVMROS1_76dbb0c8d0143bd4d39bbf66c6866b0771013fe67fbad76887c6490c91f39e80"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_valid(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse const* value) {
  if(!value)return 0;
  if(value->answer.size>256u||!uwvm_bounded_utf8(value->answer.data,value->answer.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse const* value) {size_t n=0;
  n+=4+value->answer.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->answer.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->answer.data,value->answer.size);*offset+=value->answer.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse* value) {
  if(*offset>n||n-*offset<4)return 0;value->answer.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->answer.size>256u||value->answer.size>n-*offset)return 0;uwvm_bounded_copy(value->answer.data,p+*offset,value->answer.size);*offset+=value->answer.size;
  return roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_encode(roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_RawRequestResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/SetModeGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal {
  /** Declared field target_robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_RobotMode target_robot_mode;
  /** Declared field stop_program; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t stop_program;
  /** Declared field play_program; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t play_program;
} roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODEGOAL_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODEGOAL_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODEGOAL_CODEC_IDENTITY "UWVMROS1_27ba58b57af47e8efff15a70fb0562d5904d019c17e39a499bf003730434a528"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_valid(roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_valid(&value->target_robot_mode))return 0;
  if(value->stop_program>1)return 0;
  if(value->play_program>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal const* value) {size_t n=0;
  n+=roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_wire_size(&value->target_robot_mode);
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_write_fields(&value->target_robot_mode,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->stop_program,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->play_program,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal* value) {
  if(!roswasmc_bounded_1_ur_dashboard_msgs_RobotMode_read_fields(p,n,offset,&value->target_robot_mode))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->stop_program,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->play_program,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_encode(roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_SetModeGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for ur_dashboard_msgs/SetModeResult.message; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_message_text;
/** Complete ROS1 ur_dashboard_msgs/SetModeResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
  /** Declared field message; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_message_text message;
} roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODERESULT_MAX_WIRE_BYTES 261u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODERESULT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODERESULT_CODEC_IDENTITY "UWVMROS1_e558d01061183cabca7022bd66aeb1958e14dfc62226a701c86020e73ffa1114"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_valid(roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  if(value->message.size>256u||!uwvm_bounded_utf8(value->message.data,value->message.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult const* value) {size_t n=0;
  n+=1;
  n+=4+value->message.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
  uwvm_bounded_put(p+*offset,value->message.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->message.data,value->message.size);*offset+=value->message.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;value->message.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->message.size>256u||value->message.size>n-*offset)return 0;uwvm_bounded_copy(value->message.data,p+*offset,value->message.size);*offset+=value->message.size;
  return roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_encode(roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>261u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_SetModeResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_dashboard_msgs/SetModeFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback {
  /** Declared field current_robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t current_robot_mode;
  /** Declared field current_safety_mode; canonical capacity is part of CODEC_IDENTITY. */
  int8_t current_safety_mode;
} roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback;
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODEFEEDBACK_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODEFEEDBACK_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_1_UR_DASHBOARD_MSGS_SETMODEFEEDBACK_CODEC_IDENTITY "UWVMROS1_d29b507dcc5d72ee086c8a5a9a7a762d677ed408ec02dbe661abc17e2de5c7e2"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_valid(roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_wire_size(roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_write_fields(roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->current_robot_mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->current_safety_mode,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->current_robot_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->current_safety_mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_encode(roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback* value) {
  roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_dashboard_msgs_SetModeFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/Analog value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_Analog {
  /** Declared field pin; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t pin;
  /** Declared field domain; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t domain;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  float state;
} roswasmc_bounded_1_ur_msgs_Analog;
#define ROSWASMC_BOUNDED_1_UR_MSGS_ANALOG_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_MSGS_ANALOG_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_MSGS_ANALOG_CODEC_IDENTITY "UWVMROS1_638dc3b30def8193a501b4c1c778792a60c8c683a96880e9fa6db6c01774f6a7"
/** Original scalar constant CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_ANALOG_CURRENT UINT32_C(0)
/** Original scalar constant VOLTAGE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_ANALOG_VOLTAGE UINT32_C(1)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_Analog_valid(roswasmc_bounded_1_ur_msgs_Analog const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_Analog_wire_size(roswasmc_bounded_1_ur_msgs_Analog const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_Analog_write_fields(roswasmc_bounded_1_ur_msgs_Analog const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->domain,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_Analog_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_Analog* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->domain,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->state,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ur_msgs_Analog_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_Analog_encode(roswasmc_bounded_1_ur_msgs_Analog const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_Analog_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_Analog_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_Analog_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_Analog_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_Analog* value) {
  roswasmc_bounded_1_ur_msgs_Analog next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_Analog_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/Digital value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_Digital {
  /** Declared field pin; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t pin;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t state;
} roswasmc_bounded_1_ur_msgs_Digital;
#define ROSWASMC_BOUNDED_1_UR_MSGS_DIGITAL_MAX_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_1_UR_MSGS_DIGITAL_MIN_WIRE_BYTES 2u
#define ROSWASMC_BOUNDED_1_UR_MSGS_DIGITAL_CODEC_IDENTITY "UWVMROS1_c5840cfc5e26fe2f18817e46a0322a92e02ca21983920c71ebeb585769f5622c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_Digital_valid(roswasmc_bounded_1_ur_msgs_Digital const* value) {
  if(!value)return 0;
  if(value->state>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_Digital_wire_size(roswasmc_bounded_1_ur_msgs_Digital const* value) {size_t n=0;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_Digital_write_fields(roswasmc_bounded_1_ur_msgs_Digital const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->state,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_Digital_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_Digital* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->state,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_Digital_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_Digital_encode(roswasmc_bounded_1_ur_msgs_Digital const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_Digital_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_Digital_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_Digital_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_Digital_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_Digital* value) {
  roswasmc_bounded_1_ur_msgs_Digital next={0};size_t offset=0;if(!value||(length&&!input)||length<2u||length>2u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_Digital_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/GetRobotSoftwareVersionRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest;
#define ROSWASMC_BOUNDED_1_UR_MSGS_GETROBOTSOFTWAREVERSIONREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_MSGS_GETROBOTSOFTWAREVERSIONREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UR_MSGS_GETROBOTSOFTWAREVERSIONREQUEST_CODEC_IDENTITY "UWVMROS1_bbcdd98ee8d951a8a4fa873ad1c755f16c5cce14ceb7a614343dd44ccf84b9c8"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_valid(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_wire_size(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_write_fields(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest* value) {
  return roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_encode(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest* value) {
  roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/GetRobotSoftwareVersionResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse {
  /** Declared field major; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t major;
  /** Declared field minor; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t minor;
  /** Declared field bugfix; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t bugfix;
  /** Declared field build; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t build;
} roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse;
#define ROSWASMC_BOUNDED_1_UR_MSGS_GETROBOTSOFTWAREVERSIONRESPONSE_MAX_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_UR_MSGS_GETROBOTSOFTWAREVERSIONRESPONSE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_1_UR_MSGS_GETROBOTSOFTWAREVERSIONRESPONSE_CODEC_IDENTITY "UWVMROS1_36ab51ce55e3630f8ca71f364f4c5cc1f4b010f678777e4fd7b03dd480f92e64"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_valid(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_wire_size(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_write_fields(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->major,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->minor,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->bugfix,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->build,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->major,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->minor,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bugfix,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->build,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_encode(roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse* value) {
  roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>16u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_GetRobotSoftwareVersionResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_ur_msgs_Digital data[32];} roswasmc_bounded_1_ur_msgs_IOStates_digital_in_states_sequence;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_ur_msgs_Digital data[32];} roswasmc_bounded_1_ur_msgs_IOStates_digital_out_states_sequence;
/** At most32 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_ur_msgs_Digital data[32];} roswasmc_bounded_1_ur_msgs_IOStates_flag_states_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_ur_msgs_Analog data[16];} roswasmc_bounded_1_ur_msgs_IOStates_analog_in_states_sequence;
/** At most16 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_ur_msgs_Analog data[16];} roswasmc_bounded_1_ur_msgs_IOStates_analog_out_states_sequence;
/** Complete ROS1 ur_msgs/IOStates value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_IOStates {
  /** Declared field digital_in_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_IOStates_digital_in_states_sequence digital_in_states;
  /** Declared field digital_out_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_IOStates_digital_out_states_sequence digital_out_states;
  /** Declared field flag_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_IOStates_flag_states_sequence flag_states;
  /** Declared field analog_in_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_IOStates_analog_in_states_sequence analog_in_states;
  /** Declared field analog_out_states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_IOStates_analog_out_states_sequence analog_out_states;
} roswasmc_bounded_1_ur_msgs_IOStates;
#define ROSWASMC_BOUNDED_1_UR_MSGS_IOSTATES_MAX_WIRE_BYTES 404u
#define ROSWASMC_BOUNDED_1_UR_MSGS_IOSTATES_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_UR_MSGS_IOSTATES_CODEC_IDENTITY "UWVMROS1_4b10f6efdd2f129267c407bc1f8caed03776fef6d49f0a1b9dfbf2451ad8f033"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_IOStates_valid(roswasmc_bounded_1_ur_msgs_IOStates const* value) {
  if(!value)return 0;
  if(value->digital_in_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Digital_valid(&value->digital_in_states.data[i]))return 0;
  }}
  if(value->digital_out_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Digital_valid(&value->digital_out_states.data[i]))return 0;
  }}
  if(value->flag_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Digital_valid(&value->flag_states.data[i]))return 0;
  }}
  if(value->analog_in_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Analog_valid(&value->analog_in_states.data[i]))return 0;
  }}
  if(value->analog_out_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Analog_valid(&value->analog_out_states.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_IOStates_wire_size(roswasmc_bounded_1_ur_msgs_IOStates const* value) {size_t n=0;
  n+=4;
  if(value->digital_in_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  n+=roswasmc_bounded_1_ur_msgs_Digital_wire_size(&value->digital_in_states.data[i]);
  }}
  n+=4;
  if(value->digital_out_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  n+=roswasmc_bounded_1_ur_msgs_Digital_wire_size(&value->digital_out_states.data[i]);
  }}
  n+=4;
  if(value->flag_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  n+=roswasmc_bounded_1_ur_msgs_Digital_wire_size(&value->flag_states.data[i]);
  }}
  n+=4;
  if(value->analog_in_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  n+=roswasmc_bounded_1_ur_msgs_Analog_wire_size(&value->analog_in_states.data[i]);
  }}
  n+=4;
  if(value->analog_out_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  n+=roswasmc_bounded_1_ur_msgs_Analog_wire_size(&value->analog_out_states.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_IOStates_write_fields(roswasmc_bounded_1_ur_msgs_IOStates const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->digital_in_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  roswasmc_bounded_1_ur_msgs_Digital_write_fields(&value->digital_in_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->digital_out_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  roswasmc_bounded_1_ur_msgs_Digital_write_fields(&value->digital_out_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->flag_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  roswasmc_bounded_1_ur_msgs_Digital_write_fields(&value->flag_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->analog_in_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  roswasmc_bounded_1_ur_msgs_Analog_write_fields(&value->analog_in_states.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->analog_out_states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  roswasmc_bounded_1_ur_msgs_Analog_write_fields(&value->analog_out_states.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_IOStates_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_IOStates* value) {
  if(*offset>n||n-*offset<4)return 0;value->digital_in_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->digital_in_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_in_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Digital_read_fields(p,n,offset,&value->digital_in_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->digital_out_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->digital_out_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->digital_out_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Digital_read_fields(p,n,offset,&value->digital_out_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->flag_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->flag_states.size>32u)return 0;
  {size_t i;for(i=0;i<value->flag_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Digital_read_fields(p,n,offset,&value->flag_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->analog_in_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->analog_in_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_in_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Analog_read_fields(p,n,offset,&value->analog_in_states.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->analog_out_states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->analog_out_states.size>16u)return 0;
  {size_t i;for(i=0;i<value->analog_out_states.size;++i){
  if(!roswasmc_bounded_1_ur_msgs_Analog_read_fields(p,n,offset,&value->analog_out_states.data[i]))return 0;
  }}
  return roswasmc_bounded_1_ur_msgs_IOStates_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_IOStates_encode(roswasmc_bounded_1_ur_msgs_IOStates const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_IOStates_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_IOStates_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_IOStates_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_IOStates_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_IOStates* value) {
  roswasmc_bounded_1_ur_msgs_IOStates next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>404u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_IOStates_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/MasterboardDataMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_MasterboardDataMsg {
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
  /** Declared field robot_voltage_48V; canonical capacity is part of CODEC_IDENTITY. */
  float robot_voltage_48V;
  /** Declared field robot_current; canonical capacity is part of CODEC_IDENTITY. */
  float robot_current;
  /** Declared field master_io_current; canonical capacity is part of CODEC_IDENTITY. */
  float master_io_current;
  /** Declared field master_safety_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t master_safety_state;
  /** Declared field master_onoff_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t master_onoff_state;
} roswasmc_bounded_1_ur_msgs_MasterboardDataMsg;
#define ROSWASMC_BOUNDED_1_UR_MSGS_MASTERBOARDDATAMSG_MAX_WIRE_BYTES 62u
#define ROSWASMC_BOUNDED_1_UR_MSGS_MASTERBOARDDATAMSG_MIN_WIRE_BYTES 62u
#define ROSWASMC_BOUNDED_1_UR_MSGS_MASTERBOARDDATAMSG_CODEC_IDENTITY "UWVMROS1_f80f92b90cb33dd5b118df81447dfcc33c1ab230a91bb915a1be684401da265f"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_valid(roswasmc_bounded_1_ur_msgs_MasterboardDataMsg const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input0,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input1,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_output0,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_output1,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->masterboard_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_voltage_48V,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->master_io_current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_wire_size(roswasmc_bounded_1_ur_msgs_MasterboardDataMsg const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_write_fields(roswasmc_bounded_1_ur_msgs_MasterboardDataMsg const* value,unsigned char* p,size_t* offset) {
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
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_voltage_48V,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->robot_current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->master_io_current,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->master_safety_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->master_onoff_state,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_MasterboardDataMsg* value) {
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
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->robot_voltage_48V,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->robot_current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->master_io_current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->master_safety_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->master_onoff_state,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_encode(roswasmc_bounded_1_ur_msgs_MasterboardDataMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_MasterboardDataMsg* value) {
  roswasmc_bounded_1_ur_msgs_MasterboardDataMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<62u||length>62u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_MasterboardDataMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/RobotModeDataMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_RobotModeDataMsg {
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
} roswasmc_bounded_1_ur_msgs_RobotModeDataMsg;
#define ROSWASMC_BOUNDED_1_UR_MSGS_ROBOTMODEDATAMSG_MAX_WIRE_BYTES 15u
#define ROSWASMC_BOUNDED_1_UR_MSGS_ROBOTMODEDATAMSG_MIN_WIRE_BYTES 15u
#define ROSWASMC_BOUNDED_1_UR_MSGS_ROBOTMODEDATAMSG_CODEC_IDENTITY "UWVMROS1_64c4247d865e0731b13ca13ce2e355df664cabf7f3d646051d9d3ef48b05a231"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_valid(roswasmc_bounded_1_ur_msgs_RobotModeDataMsg const* value) {
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
static inline size_t roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_wire_size(roswasmc_bounded_1_ur_msgs_RobotModeDataMsg const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_write_fields(roswasmc_bounded_1_ur_msgs_RobotModeDataMsg const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_RobotModeDataMsg* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->timestamp,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_robot_connected,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_real_robot_enabled,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_power_on_robot,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_emergency_stopped,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_protective_stopped,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_program_running,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->is_program_paused,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_encode(roswasmc_bounded_1_ur_msgs_RobotModeDataMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_RobotModeDataMsg* value) {
  roswasmc_bounded_1_ur_msgs_RobotModeDataMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<15u||length>15u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_RobotModeDataMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_q_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_qd_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_qdd_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_i_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_m_target_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_q_actual_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_qd_actual_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_i_actual_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tool_acc_values_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tcp_force_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tool_vector_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tcp_speed_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_motor_temperatures_sequence;
/** At most6 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;double data[6];} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_joint_modes_sequence;
/** Complete ROS1 ur_msgs/RobotStateRTMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_RobotStateRTMsg {
  /** Declared field time; canonical capacity is part of CODEC_IDENTITY. */
  double time;
  /** Declared field q_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_q_target_sequence q_target;
  /** Declared field qd_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_qd_target_sequence qd_target;
  /** Declared field qdd_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_qdd_target_sequence qdd_target;
  /** Declared field i_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_i_target_sequence i_target;
  /** Declared field m_target; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_m_target_sequence m_target;
  /** Declared field q_actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_q_actual_sequence q_actual;
  /** Declared field qd_actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_qd_actual_sequence qd_actual;
  /** Declared field i_actual; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_i_actual_sequence i_actual;
  /** Declared field tool_acc_values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tool_acc_values_sequence tool_acc_values;
  /** Declared field tcp_force; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tcp_force_sequence tcp_force;
  /** Declared field tool_vector; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tool_vector_sequence tool_vector;
  /** Declared field tcp_speed; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_tcp_speed_sequence tcp_speed;
  /** Declared field digital_input_bits; canonical capacity is part of CODEC_IDENTITY. */
  double digital_input_bits;
  /** Declared field motor_temperatures; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_motor_temperatures_sequence motor_temperatures;
  /** Declared field controller_timer; canonical capacity is part of CODEC_IDENTITY. */
  double controller_timer;
  /** Declared field test_value; canonical capacity is part of CODEC_IDENTITY. */
  double test_value;
  /** Declared field robot_mode; canonical capacity is part of CODEC_IDENTITY. */
  double robot_mode;
  /** Declared field joint_modes; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_joint_modes_sequence joint_modes;
} roswasmc_bounded_1_ur_msgs_RobotStateRTMsg;
#define ROSWASMC_BOUNDED_1_UR_MSGS_ROBOTSTATERTMSG_MAX_WIRE_BYTES 768u
#define ROSWASMC_BOUNDED_1_UR_MSGS_ROBOTSTATERTMSG_MIN_WIRE_BYTES 96u
#define ROSWASMC_BOUNDED_1_UR_MSGS_ROBOTSTATERTMSG_CODEC_IDENTITY "UWVMROS1_e3d93b6141f749b93c229d8eabebbeb2782a16621fea4d7d39096794e7a67ce7"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_valid(roswasmc_bounded_1_ur_msgs_RobotStateRTMsg const* value) {
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
static inline size_t roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_wire_size(roswasmc_bounded_1_ur_msgs_RobotStateRTMsg const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_write_fields(roswasmc_bounded_1_ur_msgs_RobotStateRTMsg const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_RobotStateRTMsg* value) {
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
  return roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_encode(roswasmc_bounded_1_ur_msgs_RobotStateRTMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_RobotStateRTMsg* value) {
  roswasmc_bounded_1_ur_msgs_RobotStateRTMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<96u||length>768u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_RobotStateRTMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetAnalogOutputRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest {
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_ur_msgs_Analog data;
} roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETANALOGOUTPUTREQUEST_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETANALOGOUTPUTREQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETANALOGOUTPUTREQUEST_CODEC_IDENTITY "UWVMROS1_1830a63c8853ebe8842f0e8e6870fbdee136dfe78726ba581fd1c54f910c4789"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_valid(roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_ur_msgs_Analog_valid(&value->data))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_wire_size(roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_ur_msgs_Analog_wire_size(&value->data);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_write_fields(roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_ur_msgs_Analog_write_fields(&value->data,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest* value) {
  if(!roswasmc_bounded_1_ur_msgs_Analog_read_fields(p,n,offset,&value->data))return 0;
  return roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_encode(roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest* value) {
  roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetAnalogOutputRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetAnalogOutputResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETANALOGOUTPUTRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETANALOGOUTPUTRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETANALOGOUTPUTRESPONSE_CODEC_IDENTITY "UWVMROS1_bb84c969f904082699f67d385b093b0c6de97bfacc33eab86932f4564a823580"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_valid(roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_wire_size(roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_write_fields(roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_encode(roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse* value) {
  roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetAnalogOutputResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetForceModeRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetForceModeRequest {
  /** Declared field task_frame; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_PoseStamped task_frame;
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
  roswasmc_bounded_1_geometry_msgs_Wrench wrench;
  /** Declared field type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t type;
  /** Declared field speed_limits; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Twist speed_limits;
  /** Declared field deviation_limits; canonical capacity is part of CODEC_IDENTITY. */
  float deviation_limits[6];
  /** Declared field damping_factor; canonical capacity is part of CODEC_IDENTITY. */
  float damping_factor;
  /** Declared field gain_scaling; canonical capacity is part of CODEC_IDENTITY. */
  float gain_scaling;
} roswasmc_bounded_1_ur_msgs_SetForceModeRequest;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODEREQUEST_MAX_WIRE_BYTES 335u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODEREQUEST_MIN_WIRE_BYTES 207u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODEREQUEST_CODEC_IDENTITY "UWVMROS1_c148d6eb37258e10f5d16e76e41c334fc57a3d751c0c35c47be4cabf4c015a6b"
/** Original scalar constant TCP_TO_ORIGIN=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODEREQUEST_TCP_TO_ORIGIN UINT32_C(1)
/** Original scalar constant NO_TRANSFORM=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODEREQUEST_NO_TRANSFORM UINT32_C(2)
/** Original scalar constant TCP_VELOCITY_TO_X_Y=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODEREQUEST_TCP_VELOCITY_TO_X_Y UINT32_C(3)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetForceModeRequest_valid(roswasmc_bounded_1_ur_msgs_SetForceModeRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_PoseStamped_valid(&value->task_frame))return 0;
  if(value->selection_vector_x>1)return 0;
  if(value->selection_vector_y>1)return 0;
  if(value->selection_vector_z>1)return 0;
  if(value->selection_vector_rx>1)return 0;
  if(value->selection_vector_ry>1)return 0;
  if(value->selection_vector_rz>1)return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Wrench_valid(&value->wrench))return 0;
  if(!roswasmc_bounded_1_geometry_msgs_Twist_valid(&value->speed_limits))return 0;
  {size_t i;for(i=0;i<6u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->deviation_limits[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->damping_factor,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gain_scaling,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetForceModeRequest_wire_size(roswasmc_bounded_1_ur_msgs_SetForceModeRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_geometry_msgs_PoseStamped_wire_size(&value->task_frame);
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=roswasmc_bounded_1_geometry_msgs_Wrench_wire_size(&value->wrench);
  n+=1;
  n+=roswasmc_bounded_1_geometry_msgs_Twist_wire_size(&value->speed_limits);
  {size_t i;for(i=0;i<6u;++i){
  n+=4;
  }}
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetForceModeRequest_write_fields(roswasmc_bounded_1_ur_msgs_SetForceModeRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_geometry_msgs_PoseStamped_write_fields(&value->task_frame,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_x,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_y,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_z,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_rx,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_ry,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->selection_vector_rz,1);*offset+=1;
  roswasmc_bounded_1_geometry_msgs_Wrench_write_fields(&value->wrench,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->type,1);*offset+=1;
  roswasmc_bounded_1_geometry_msgs_Twist_write_fields(&value->speed_limits,p,offset);
  {size_t i;for(i=0;i<6u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->deviation_limits[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->damping_factor,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gain_scaling,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetForceModeRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetForceModeRequest* value) {
  if(!roswasmc_bounded_1_geometry_msgs_PoseStamped_read_fields(p,n,offset,&value->task_frame))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_x,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_y,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_z,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_rx,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_ry,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->selection_vector_rz,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_1_geometry_msgs_Wrench_read_fields(p,n,offset,&value->wrench))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->type,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_1_geometry_msgs_Twist_read_fields(p,n,offset,&value->speed_limits))return 0;
  {size_t i;for(i=0;i<6u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->deviation_limits[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->damping_factor,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gain_scaling,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ur_msgs_SetForceModeRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetForceModeRequest_encode(roswasmc_bounded_1_ur_msgs_SetForceModeRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetForceModeRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetForceModeRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetForceModeRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetForceModeRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetForceModeRequest* value) {
  roswasmc_bounded_1_ur_msgs_SetForceModeRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<207u||length>335u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetForceModeRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetForceModeResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetForceModeResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_msgs_SetForceModeResponse;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODERESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODERESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETFORCEMODERESPONSE_CODEC_IDENTITY "UWVMROS1_bb1eeb0b38d81d642066bed1510245423f62d25983a2de89e985d1fd1932308c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetForceModeResponse_valid(roswasmc_bounded_1_ur_msgs_SetForceModeResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetForceModeResponse_wire_size(roswasmc_bounded_1_ur_msgs_SetForceModeResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetForceModeResponse_write_fields(roswasmc_bounded_1_ur_msgs_SetForceModeResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetForceModeResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetForceModeResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_SetForceModeResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetForceModeResponse_encode(roswasmc_bounded_1_ur_msgs_SetForceModeResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetForceModeResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetForceModeResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetForceModeResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetForceModeResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetForceModeResponse* value) {
  roswasmc_bounded_1_ur_msgs_SetForceModeResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetForceModeResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetIORequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetIORequest {
  /** Declared field fun; canonical capacity is part of CODEC_IDENTITY. */
  int8_t fun;
  /** Declared field pin; canonical capacity is part of CODEC_IDENTITY. */
  int8_t pin;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  float state;
} roswasmc_bounded_1_ur_msgs_SetIORequest;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_MAX_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_MIN_WIRE_BYTES 6u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_CODEC_IDENTITY "UWVMROS1_3dbd2f05686ddaac16b37dc8ec98303acb672d176be34fc906cf32f2d4cd1872"
/** Original scalar constant PIN_AOUT0=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_AOUT0 INT32_C(0)
/** Original scalar constant PIN_AOUT1=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_AOUT1 INT32_C(1)
/** Original scalar constant PIN_DOUT0=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT0 INT32_C(0)
/** Original scalar constant PIN_DOUT1=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT1 INT32_C(1)
/** Original scalar constant PIN_DOUT2=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT2 INT32_C(2)
/** Original scalar constant PIN_DOUT3=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT3 INT32_C(3)
/** Original scalar constant PIN_DOUT4=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT4 INT32_C(4)
/** Original scalar constant PIN_DOUT5=5; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT5 INT32_C(5)
/** Original scalar constant PIN_DOUT6=6; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT6 INT32_C(6)
/** Original scalar constant PIN_DOUT7=7; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_DOUT7 INT32_C(7)
/** Original scalar constant PIN_CONF_OUT0=8; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT0 INT32_C(8)
/** Original scalar constant PIN_CONF_OUT1=9; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT1 INT32_C(9)
/** Original scalar constant PIN_CONF_OUT2=10; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT2 INT32_C(10)
/** Original scalar constant PIN_CONF_OUT3=11; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT3 INT32_C(11)
/** Original scalar constant PIN_CONF_OUT4=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT4 INT32_C(12)
/** Original scalar constant PIN_CONF_OUT5=13; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT5 INT32_C(13)
/** Original scalar constant PIN_CONF_OUT6=14; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT6 INT32_C(14)
/** Original scalar constant PIN_CONF_OUT7=15; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_CONF_OUT7 INT32_C(15)
/** Original scalar constant PIN_TOOL_DOUT0=16; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_TOOL_DOUT0 INT32_C(16)
/** Original scalar constant PIN_TOOL_DOUT1=17; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_PIN_TOOL_DOUT1 INT32_C(17)
/** Original scalar constant FUN_SET_DIGITAL_OUT=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_FUN_SET_DIGITAL_OUT INT32_C(1)
/** Original scalar constant FUN_SET_FLAG=2; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_FUN_SET_FLAG INT32_C(2)
/** Original scalar constant FUN_SET_ANALOG_OUT=3; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_FUN_SET_ANALOG_OUT INT32_C(3)
/** Original scalar constant FUN_SET_TOOL_VOLTAGE=4; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_FUN_SET_TOOL_VOLTAGE INT32_C(4)
/** Original scalar constant STATE_OFF=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_STATE_OFF INT32_C(0)
/** Original scalar constant STATE_ON=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_STATE_ON INT32_C(1)
/** Original scalar constant STATE_TOOL_VOLTAGE_0V=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_STATE_TOOL_VOLTAGE_0V INT32_C(0)
/** Original scalar constant STATE_TOOL_VOLTAGE_12V=12; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_STATE_TOOL_VOLTAGE_12V INT32_C(12)
/** Original scalar constant STATE_TOOL_VOLTAGE_24V=24; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIOREQUEST_STATE_TOOL_VOLTAGE_24V INT32_C(24)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetIORequest_valid(roswasmc_bounded_1_ur_msgs_SetIORequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetIORequest_wire_size(roswasmc_bounded_1_ur_msgs_SetIORequest const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetIORequest_write_fields(roswasmc_bounded_1_ur_msgs_SetIORequest const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->fun,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->pin,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->state,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetIORequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetIORequest* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->fun,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pin,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->state,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_ur_msgs_SetIORequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetIORequest_encode(roswasmc_bounded_1_ur_msgs_SetIORequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetIORequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetIORequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetIORequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetIORequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetIORequest* value) {
  roswasmc_bounded_1_ur_msgs_SetIORequest next={0};size_t offset=0;if(!value||(length&&!input)||length<6u||length>6u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetIORequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetIOResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetIOResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_msgs_SetIOResponse;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIORESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIORESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETIORESPONSE_CODEC_IDENTITY "UWVMROS1_7fc137aa3db3178a450b8b2396b7821c911c79656e52bf48814745c261aecf61"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetIOResponse_valid(roswasmc_bounded_1_ur_msgs_SetIOResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetIOResponse_wire_size(roswasmc_bounded_1_ur_msgs_SetIOResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetIOResponse_write_fields(roswasmc_bounded_1_ur_msgs_SetIOResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetIOResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetIOResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_SetIOResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetIOResponse_encode(roswasmc_bounded_1_ur_msgs_SetIOResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetIOResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetIOResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetIOResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetIOResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetIOResponse* value) {
  roswasmc_bounded_1_ur_msgs_SetIOResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetIOResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetPayloadRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetPayloadRequest {
  /** Declared field mass; canonical capacity is part of CODEC_IDENTITY. */
  float mass;
  /** Declared field center_of_gravity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_geometry_msgs_Vector3 center_of_gravity;
} roswasmc_bounded_1_ur_msgs_SetPayloadRequest;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETPAYLOADREQUEST_MAX_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETPAYLOADREQUEST_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETPAYLOADREQUEST_CODEC_IDENTITY "UWVMROS1_63ae68b44ce6aaad37cbda779fb51dee78923df27c582b4aab4104b16a307ee3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetPayloadRequest_valid(roswasmc_bounded_1_ur_msgs_SetPayloadRequest const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_valid(&value->center_of_gravity))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetPayloadRequest_wire_size(roswasmc_bounded_1_ur_msgs_SetPayloadRequest const* value) {size_t n=0;
  n+=4;
  n+=roswasmc_bounded_1_geometry_msgs_Vector3_wire_size(&value->center_of_gravity);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetPayloadRequest_write_fields(roswasmc_bounded_1_ur_msgs_SetPayloadRequest const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->mass,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_1_geometry_msgs_Vector3_write_fields(&value->center_of_gravity,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetPayloadRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetPayloadRequest* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->mass,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_1_geometry_msgs_Vector3_read_fields(p,n,offset,&value->center_of_gravity))return 0;
  return roswasmc_bounded_1_ur_msgs_SetPayloadRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetPayloadRequest_encode(roswasmc_bounded_1_ur_msgs_SetPayloadRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetPayloadRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetPayloadRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetPayloadRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetPayloadRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetPayloadRequest* value) {
  roswasmc_bounded_1_ur_msgs_SetPayloadRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>28u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetPayloadRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetPayloadResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetPayloadResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_msgs_SetPayloadResponse;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETPAYLOADRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETPAYLOADRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETPAYLOADRESPONSE_CODEC_IDENTITY "UWVMROS1_a3e991f1357f2d3017ccbb7636a6248e8774434d723f5f6c77ff251dd61fce73"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetPayloadResponse_valid(roswasmc_bounded_1_ur_msgs_SetPayloadResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetPayloadResponse_wire_size(roswasmc_bounded_1_ur_msgs_SetPayloadResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetPayloadResponse_write_fields(roswasmc_bounded_1_ur_msgs_SetPayloadResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetPayloadResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetPayloadResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_SetPayloadResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetPayloadResponse_encode(roswasmc_bounded_1_ur_msgs_SetPayloadResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetPayloadResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetPayloadResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetPayloadResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetPayloadResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetPayloadResponse* value) {
  roswasmc_bounded_1_ur_msgs_SetPayloadResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetPayloadResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetSpeedSliderFractionRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest {
  /** Declared field speed_slider_fraction; canonical capacity is part of CODEC_IDENTITY. */
  double speed_slider_fraction;
} roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETSPEEDSLIDERFRACTIONREQUEST_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETSPEEDSLIDERFRACTIONREQUEST_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETSPEEDSLIDERFRACTIONREQUEST_CODEC_IDENTITY "UWVMROS1_f192d6d655b2ed7725bf5afe6977a352870abfd4d07e0c3380a09d280a0a0a32"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_valid(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed_slider_fraction,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_wire_size(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest const* value) {size_t n=0;
  n+=8;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_write_fields(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->speed_slider_fraction,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->speed_slider_fraction,&bits,8);}*offset+=8;
  return roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_encode(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest* value) {
  roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/SetSpeedSliderFractionResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse {
  /** Declared field success; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t success;
} roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse;
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETSPEEDSLIDERFRACTIONRESPONSE_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETSPEEDSLIDERFRACTIONRESPONSE_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_1_UR_MSGS_SETSPEEDSLIDERFRACTIONRESPONSE_CODEC_IDENTITY "UWVMROS1_cb08c6758ce2eeeac93bc579d7895b19b84e74b7e2d19d87831023df579288b0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_valid(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse const* value) {
  if(!value)return 0;
  if(value->success>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_wire_size(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_write_fields(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->success,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->success,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_encode(roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse* value) {
  roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_SetSpeedSliderFractionResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 ur_msgs/ToolDataMsg value; no field projects away native data. */
typedef struct roswasmc_bounded_1_ur_msgs_ToolDataMsg {
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
} roswasmc_bounded_1_ur_msgs_ToolDataMsg;
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_MAX_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_MIN_WIRE_BYTES 32u
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_CODEC_IDENTITY "UWVMROS1_1fad1aba069bc474306dc421a62950ffeb3a0837668f08a9adf41ac1bc2bb390"
/** Original scalar constant ANALOG_INPUT_RANGE_CURRENT=0; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_ANALOG_INPUT_RANGE_CURRENT INT32_C(0)
/** Original scalar constant ANALOG_INPUT_RANGE_VOLTAGE=1; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_ANALOG_INPUT_RANGE_VOLTAGE INT32_C(1)
/** Original scalar constant TOOL_BOOTLOADER_MODE=249; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_TOOL_BOOTLOADER_MODE UINT32_C(249)
/** Original scalar constant TOOL_RUNNING_MODE=253; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_TOOL_RUNNING_MODE UINT32_C(253)
/** Original scalar constant TOOL_IDLE_MODE=255; no instance/wire field. */
#define ROSWASMC_BOUNDED_1_UR_MSGS_TOOLDATAMSG_TOOL_IDLE_MODE UINT32_C(255)
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_ur_msgs_ToolDataMsg_valid(roswasmc_bounded_1_ur_msgs_ToolDataMsg const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input2,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->analog_input3,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_voltage_48v,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_current,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tool_temperature,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_ur_msgs_ToolDataMsg_wire_size(roswasmc_bounded_1_ur_msgs_ToolDataMsg const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_ur_msgs_ToolDataMsg_write_fields(roswasmc_bounded_1_ur_msgs_ToolDataMsg const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_ur_msgs_ToolDataMsg_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_ur_msgs_ToolDataMsg* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_input_range2,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->analog_input_range3,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_input2,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->analog_input3,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tool_voltage_48v,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tool_output_voltage,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tool_current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tool_temperature,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->tool_mode,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_ur_msgs_ToolDataMsg_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_ToolDataMsg_encode(roswasmc_bounded_1_ur_msgs_ToolDataMsg const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_ur_msgs_ToolDataMsg_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_ur_msgs_ToolDataMsg_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_ur_msgs_ToolDataMsg_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_ur_msgs_ToolDataMsg_decode(void const* input,size_t length,roswasmc_bounded_1_ur_msgs_ToolDataMsg* value) {
  roswasmc_bounded_1_ur_msgs_ToolDataMsg next={0};size_t offset=0;if(!value||(length&&!input)||length<32u||length>32u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_ur_msgs_ToolDataMsg_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
