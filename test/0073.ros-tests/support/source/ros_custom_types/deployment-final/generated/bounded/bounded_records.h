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
/** Complete ROS1 uwvm_ros_bounded_v2/Point value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_Point {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  int32_t value;
  /** Declared field enabled; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t enabled;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_Point;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_POINT_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_POINT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_POINT_CODEC_IDENTITY "UWVMROS1_adb4b0b058e3491e31d00d4c0c5914bdeeabe7ba8ce7a4de798ac358c8fecf28"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_Point const* value) {
  if(!value)return 0;
  if(value->enabled>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_Point const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_Point const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->enabled,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_Point* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->enabled,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_Point const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_Point* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Point next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_ros_bounded_v2/Envelope.label; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[32];} roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_label_text;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint16_t data[8];} roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_samples_sequence;
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_1_uwvm_ros_bounded_v2_Point data[4];} roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_points_sequence;
/** UTF-8 byte storage for uwvm_ros_bounded_v2/Envelope.labels; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[16];} roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_labels_text;
/** Complete ROS1 uwvm_ros_bounded_v2/Envelope value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope {
  /** Declared field label; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_label_text label;
  /** Declared field samples; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_samples_sequence samples;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_points_sequence points;
  /** Declared field labels; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_labels_text labels[2];
} roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ENVELOPE_MAX_WIRE_BYTES 120u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ENVELOPE_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ENVELOPE_CODEC_IDENTITY "UWVMROS1_18a098b82136b0c52c84df1c74e3dba78c63a877feabb3b14a750cbb2d7e3da4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope const* value) {
  if(!value)return 0;
  if(value->label.size>32u||!uwvm_bounded_utf8(value->label.data,value->label.size))return 0;
  if(value->samples.size>8u)return 0;
  {size_t i;for(i=0;i<value->samples.size;++i){
  }}
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_valid(&value->points.data[i]))return 0;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(value->labels[i].size>16u||!uwvm_bounded_utf8(value->labels[i].data,value->labels[i].size))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope const* value) {size_t n=0;
  n+=4+value->label.size;
  n+=4;
  if(value->samples.size>8u)return 0;
  {size_t i;for(i=0;i<value->samples.size;++i){
  n+=2;
  }}
  n+=4;
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  n+=roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_wire_size(&value->points.data[i]);
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=4+value->labels[i].size;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->label.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->label.data,value->label.size);*offset+=value->label.size;
  uwvm_bounded_put(p+*offset,value->samples.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->samples.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->samples.data[i],2);*offset+=2;
  }}
  uwvm_bounded_put(p+*offset,value->points.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->points.size;++i){
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_write_fields(&value->points.data[i],p,offset);
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,value->labels[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->labels[i].data,value->labels[i].size);*offset+=value->labels[i].size;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope* value) {
  if(*offset>n||n-*offset<4)return 0;value->label.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->label.size>32u||value->label.size>n-*offset)return 0;uwvm_bounded_copy(value->label.data,p+*offset,value->label.size);*offset+=value->label.size;
  if(*offset>n||n-*offset<4)return 0;value->samples.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->samples.size>8u)return 0;
  {size_t i;for(i=0;i<value->samples.size;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->samples.data[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Point_read_fields(p,n,offset,&value->points.data[i]))return 0;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;value->labels[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->labels[i].size>16u||value->labels[i].size>n-*offset)return 0;uwvm_bounded_copy(value->labels[i].data,p+*offset,value->labels[i].size);*offset+=value->labels[i].size;
  }}
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>120u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 uwvm_ros_bounded_v2/EchoRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest {
  /** Declared field sample; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope sample;
  /** Declared field limit; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t limit;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ECHOREQUEST_MAX_WIRE_BYTES 124u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ECHOREQUEST_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ECHOREQUEST_CODEC_IDENTITY "UWVMROS1_fc0c655e5a6a780160b2953a6fba67e28745d60b46c80f55be5ffb8b82da4420"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(&value->sample))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest const* value) {size_t n=0;
  n+=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(&value->sample);
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_write_fields(&value->sample,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->limit,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest* value) {
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_read_fields(p,n,offset,&value->sample))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->limit,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>124u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 uwvm_ros_bounded_v2/EchoResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse {
  /** Declared field accepted; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t accepted;
  /** Declared field echoed; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope echoed;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ECHORESPONSE_MAX_WIRE_BYTES 121u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ECHORESPONSE_MIN_WIRE_BYTES 21u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_ECHORESPONSE_CODEC_IDENTITY "UWVMROS1_b06920091695407488787551c23510de5a76cf0368bbed5ea82d833d36c8385b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse const* value) {
  if(!value)return 0;
  if(value->accepted>1)return 0;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(&value->echoed))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse const* value) {size_t n=0;
  n+=1;
  n+=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(&value->echoed);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->accepted,1);*offset+=1;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_write_fields(&value->echoed,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->accepted,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_read_fields(p,n,offset,&value->echoed))return 0;
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<21u||length>121u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EchoResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 uwvm_ros_bounded_v2/EmptyRequest value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_EMPTYREQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_EMPTYREQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_EMPTYREQUEST_CODEC_IDENTITY "UWVMROS1_3f00a65933d0d7b1bc1848910b3d2a503b9f2b8a9f09ef58a83fc27e534dcf40"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest* value) {
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyRequest_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 uwvm_ros_bounded_v2/EmptyResponse value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_EMPTYRESPONSE_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_EMPTYRESPONSE_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_EMPTYRESPONSE_CODEC_IDENTITY "UWVMROS1_9ddb085437fd7750f10f08121d4cfd64d83bd293553a6ed9090044ec0625a817"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse* value) {
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_EmptyResponse_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 uwvm_ros_bounded_v2/DigestGoal value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal {
  /** Declared field sample; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope sample;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTGOAL_MAX_WIRE_BYTES 120u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTGOAL_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTGOAL_CODEC_IDENTITY "UWVMROS1_08876b8b3f345f7baa27b2e371458f77eddb4f636cb00f03b6725d91d523accc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_valid(&value->sample))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal const* value) {size_t n=0;
  n+=roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_wire_size(&value->sample);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_write_fields(&value->sample,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal* value) {
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_Envelope_read_fields(p,n,offset,&value->sample))return 0;
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>120u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestGoal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_ros_bounded_v2/DigestResult.note; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[32];} roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_note_text;
/** Complete ROS1 uwvm_ros_bounded_v2/DigestResult value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult {
  /** Declared field accepted; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t accepted;
  /** Declared field sum; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t sum;
  /** Declared field note; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_note_text note;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTRESULT_MAX_WIRE_BYTES 41u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTRESULT_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTRESULT_CODEC_IDENTITY "UWVMROS1_4753d6deda29277eabe2e0b2b53ee10cbbf1a546eaf41ddf6a56174850339bcd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult const* value) {
  if(!value)return 0;
  if(value->accepted>1)return 0;
  if(value->note.size>32u||!uwvm_bounded_utf8(value->note.data,value->note.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4+value->note.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->accepted,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sum,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->note.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->note.data,value->note.size);*offset+=value->note.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->accepted,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sum,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->note.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->note.size>32u||value->note.size>n-*offset)return 0;uwvm_bounded_copy(value->note.data,p+*offset,value->note.size);*offset+=value->note.size;
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>41u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestResult_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_ros_bounded_v2/DigestFeedback.phase; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[16];} roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_phase_text;
/** Complete ROS1 uwvm_ros_bounded_v2/DigestFeedback value; no field projects away native data. */
typedef struct roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback {
  /** Declared field processed; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t processed;
  /** Declared field phase; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_phase_text phase;
} roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback;
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTFEEDBACK_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTFEEDBACK_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_1_UWVM_ROS_BOUNDED_V2_DIGESTFEEDBACK_CODEC_IDENTITY "UWVMROS1_1ff8a8acc544c733fe0719dc89ecbb3c99da08b156d1af7d29c5931006bee619"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_valid(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback const* value) {
  if(!value)return 0;
  if(value->phase.size>16u||!uwvm_bounded_utf8(value->phase.data,value->phase.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_wire_size(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback const* value) {size_t n=0;
  n+=4;
  n+=4+value->phase.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_write_fields(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->processed,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->phase.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->phase.data,value->phase.size);*offset+=value->phase.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->processed,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->phase.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->phase.size>16u||value->phase.size>n-*offset)return 0;uwvm_bounded_copy(value->phase.data,p+*offset,value->phase.size);*offset+=value->phase.size;
  return roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_encode(roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_decode(void const* input,size_t length,roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback* value) {
  roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_uwvm_ros_bounded_v2_DigestFeedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_ros_bounded_v2/msg/Point value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point {
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  int32_t value;
  /** Declared field enabled; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t enabled;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_MSG_POINT_MAX_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_MSG_POINT_MIN_WIRE_BYTES 5u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_MSG_POINT_CODEC_IDENTITY "UWVMROS1_60309848e4857d663e4b90a42790335a74352469d0d30bc3bf33083b529fb333"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point const* value) {
  if(!value)return 0;
  if(value->enabled>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point const* value) {size_t n=0;
  n+=4;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->enabled,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->value,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->enabled,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point next={0};size_t offset=0;if(!value||(length&&!input)||length<5u||length>5u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_ros_bounded_v2/msg/Envelope.label; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[32];} roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_label_text;
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint16_t data[8];} roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_samples_sequence;
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point data[4];} roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_points_sequence;
/** UTF-8 byte storage for uwvm_ros_bounded_v2/msg/Envelope.labels; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[16];} roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_labels_text;
/** Complete ROS2 uwvm_ros_bounded_v2/msg/Envelope value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope {
  /** Declared field label; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_label_text label;
  /** Declared field samples; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_samples_sequence samples;
  /** Declared field points; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_points_sequence points;
  /** Declared field labels; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_labels_text labels[2];
} roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_MSG_ENVELOPE_MAX_WIRE_BYTES 120u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_MSG_ENVELOPE_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_MSG_ENVELOPE_CODEC_IDENTITY "UWVMROS1_0a1a27d41a3aa4946fd3e118be4a76e32b496b4aff9206ba7c45f69f85374c36"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope const* value) {
  if(!value)return 0;
  if(value->label.size>32u||!uwvm_bounded_utf8(value->label.data,value->label.size))return 0;
  if(value->samples.size>8u)return 0;
  {size_t i;for(i=0;i<value->samples.size;++i){
  }}
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_valid(&value->points.data[i]))return 0;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(value->labels[i].size>16u||!uwvm_bounded_utf8(value->labels[i].data,value->labels[i].size))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope const* value) {size_t n=0;
  n+=4+value->label.size;
  n+=4;
  if(value->samples.size>8u)return 0;
  {size_t i;for(i=0;i<value->samples.size;++i){
  n+=2;
  }}
  n+=4;
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  n+=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_wire_size(&value->points.data[i]);
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=4+value->labels[i].size;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->label.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->label.data,value->label.size);*offset+=value->label.size;
  uwvm_bounded_put(p+*offset,value->samples.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->samples.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->samples.data[i],2);*offset+=2;
  }}
  uwvm_bounded_put(p+*offset,value->points.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->points.size;++i){
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_write_fields(&value->points.data[i],p,offset);
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,value->labels[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->labels[i].data,value->labels[i].size);*offset+=value->labels[i].size;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope* value) {
  if(*offset>n||n-*offset<4)return 0;value->label.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->label.size>32u||value->label.size>n-*offset)return 0;uwvm_bounded_copy(value->label.data,p+*offset,value->label.size);*offset+=value->label.size;
  if(*offset>n||n-*offset<4)return 0;value->samples.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->samples.size>8u)return 0;
  {size_t i;for(i=0;i<value->samples.size;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->samples.data[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<4)return 0;value->points.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->points.size>4u)return 0;
  {size_t i;for(i=0;i<value->points.size;++i){
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Point_read_fields(p,n,offset,&value->points.data[i]))return 0;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;value->labels[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->labels[i].size>16u||value->labels[i].size>n-*offset)return 0;uwvm_bounded_copy(value->labels[i].data,p+*offset,value->labels[i].size);*offset+=value->labels[i].size;
  }}
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>120u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Echo_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request {
  /** Declared field sample; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope sample;
  /** Declared field limit; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t limit;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_ECHO_REQUEST_MAX_WIRE_BYTES 124u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_ECHO_REQUEST_MIN_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_ECHO_REQUEST_CODEC_IDENTITY "UWVMROS1_dd30d845fcb865e94b268c77fc18bc7906d226a4ca3274224cc6e202e69b93f3"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(&value->sample))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request const* value) {size_t n=0;
  n+=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(&value->sample);
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_write_fields(&value->sample,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->limit,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request* value) {
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_read_fields(p,n,offset,&value->sample))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->limit,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<24u||length>124u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Echo_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response {
  /** Declared field accepted; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t accepted;
  /** Declared field echoed; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope echoed;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_ECHO_RESPONSE_MAX_WIRE_BYTES 121u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_ECHO_RESPONSE_MIN_WIRE_BYTES 21u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_ECHO_RESPONSE_CODEC_IDENTITY "UWVMROS1_995cb48a672a99327ed71ffb5af6d7518e11aae2f4068e17801b38799a52e5e5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response const* value) {
  if(!value)return 0;
  if(value->accepted>1)return 0;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(&value->echoed))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response const* value) {size_t n=0;
  n+=1;
  n+=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(&value->echoed);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->accepted,1);*offset+=1;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_write_fields(&value->echoed,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->accepted,&bits,1);}*offset+=1;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_read_fields(p,n,offset,&value->echoed))return 0;
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<21u||length>121u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Echo_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Empty_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_EMPTY_REQUEST_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_EMPTY_REQUEST_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_EMPTY_REQUEST_CODEC_IDENTITY "UWVMROS1_60e31c2bafbb5c26e57de70bd2f26cea80c8f1d2299ed064aa6d74f51b25a8f4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request* value) {
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_ros_bounded_v2/srv/Empty_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response {
  /** Local C layout placeholder; empty native component has exactly zero canonical bytes. */
  unsigned char local_empty_placeholder;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_EMPTY_RESPONSE_MAX_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_EMPTY_RESPONSE_MIN_WIRE_BYTES 0u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_SRV_EMPTY_RESPONSE_CODEC_IDENTITY "UWVMROS1_159ef50f81d76904657508db066fc6ffdb7229581aaeb5e2d425cd9b1e31cf2b"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response const* value) {size_t n=0;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response const* value,unsigned char* p,size_t* offset) {
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response* value) {
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<0u||length>0u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_srv_Empty_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Goal value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal {
  /** Declared field sample; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope sample;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_GOAL_MAX_WIRE_BYTES 120u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_GOAL_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_GOAL_CODEC_IDENTITY "UWVMROS1_b853f639f7516b2fd475fdd1e238f182f34423a38f7e3fa0430dc952f76bb58c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_valid(&value->sample))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal const* value) {size_t n=0;
  n+=roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_wire_size(&value->sample);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_write_fields(&value->sample,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal* value) {
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_msg_Envelope_read_fields(p,n,offset,&value->sample))return 0;
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>120u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Goal_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_ros_bounded_v2/action/Digest_Result.note; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[32];} roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_note_text;
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Result value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result {
  /** Declared field accepted; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t accepted;
  /** Declared field sum; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t sum;
  /** Declared field note; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_note_text note;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_RESULT_MAX_WIRE_BYTES 41u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_RESULT_MIN_WIRE_BYTES 9u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_RESULT_CODEC_IDENTITY "UWVMROS1_9a80357b1963e18b15441487bc1ca7dc876faa47c4a97df377dbc4f98cacd3da"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result const* value) {
  if(!value)return 0;
  if(value->accepted>1)return 0;
  if(value->note.size>32u||!uwvm_bounded_utf8(value->note.data,value->note.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4+value->note.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->accepted,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->sum,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->note.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->note.data,value->note.size);*offset+=value->note.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->accepted,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sum,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->note.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->note.size>32u||value->note.size>n-*offset)return 0;uwvm_bounded_copy(value->note.data,p+*offset,value->note.size);*offset+=value->note.size;
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result next={0};size_t offset=0;if(!value||(length&&!input)||length<9u||length>41u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Result_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_ros_bounded_v2/action/Digest_Feedback.phase; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[16];} roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_phase_text;
/** Complete ROS2 uwvm_ros_bounded_v2/action/Digest_Feedback value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback {
  /** Declared field processed; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t processed;
  /** Declared field phase; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_phase_text phase;
} roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback;
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_FEEDBACK_MAX_WIRE_BYTES 24u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_FEEDBACK_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UWVM_ROS_BOUNDED_V2_ACTION_DIGEST_FEEDBACK_CODEC_IDENTITY "UWVMROS1_940051bd38fe0ab7f0dc08355ff3da10782ee3d9705adb60ac218ea8f82180c1"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_valid(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback const* value) {
  if(!value)return 0;
  if(value->phase.size>16u||!uwvm_bounded_utf8(value->phase.data,value->phase.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_wire_size(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback const* value) {size_t n=0;
  n+=4;
  n+=4+value->phase.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_write_fields(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->processed,4);*offset+=4;
  uwvm_bounded_put(p+*offset,value->phase.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->phase.data,value->phase.size);*offset+=value->phase.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->processed,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;value->phase.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->phase.size>16u||value->phase.size>n-*offset)return 0;uwvm_bounded_copy(value->phase.data,p+*offset,value->phase.size);*offset+=value->phase.size;
  return roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_encode(roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback* value) {
  roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>24u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_ros_bounded_v2_action_Digest_Feedback_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
