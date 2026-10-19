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
/** UTF-8 byte storage for uwvm_groot_bridge/msg/RGBFrame.key; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_key_text;
/** At most768 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[768];} roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_pixels_sequence;
/** Complete ROS2 uwvm_groot_bridge/msg/RGBFrame value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame {
  /** Declared field key; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_key_text key;
  /** Declared field shape; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t shape[5];
  /** Declared field pixels; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_pixels_sequence pixels;
} roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame;
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_RGBFRAME_MAX_WIRE_BYTES 860u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_RGBFRAME_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_RGBFRAME_CODEC_IDENTITY "UWVMROS1_db7172fe7a7f0672c907f31ca0b1cb6286b804246d62e0414ac4313ae1096364"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_valid(roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame const* value) {
  if(!value)return 0;
  if(value->key.size>64u||!uwvm_bounded_utf8(value->key.data,value->key.size))return 0;
  {size_t i;for(i=0;i<5u;++i){
  }}
  if(value->pixels.size>768u)return 0;
  {size_t i;for(i=0;i<value->pixels.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_wire_size(roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame const* value) {size_t n=0;
  n+=4+value->key.size;
  {size_t i;for(i=0;i<5u;++i){
  n+=4;
  }}
  n+=4;
  if(value->pixels.size>768u)return 0;
  {size_t i;for(i=0;i<value->pixels.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_write_fields(roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->key.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->key.data,value->key.size);*offset+=value->key.size;
  {size_t i;for(i=0;i<5u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->shape[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,value->pixels.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->pixels.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->pixels.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame* value) {
  if(*offset>n||n-*offset<4)return 0;value->key.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->key.size>64u||value->key.size>n-*offset)return 0;uwvm_bounded_copy(value->key.data,p+*offset,value->key.size);*offset+=value->key.size;
  {size_t i;for(i=0;i<5u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->shape[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->pixels.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->pixels.size>768u)return 0;
  {size_t i;for(i=0;i<value->pixels.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->pixels.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_encode(roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame* value) {
  roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>860u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_groot_bridge/msg/FloatTensor.key; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_key_text;
/** At most320 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[320];} roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_values_sequence;
/** Complete ROS2 uwvm_groot_bridge/msg/FloatTensor value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor {
  /** Declared field key; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_key_text key;
  /** Declared field shape; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t shape[3];
  /** Declared field values; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_values_sequence values;
} roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor;
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_FLOATTENSOR_MAX_WIRE_BYTES 1364u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_FLOATTENSOR_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_FLOATTENSOR_CODEC_IDENTITY "UWVMROS1_a091d322ab737d2a9415f889fb78a18df7aec6ce74cfab0dcd5bb2a61ae3abae"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_valid(roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor const* value) {
  if(!value)return 0;
  if(value->key.size>64u||!uwvm_bounded_utf8(value->key.data,value->key.size))return 0;
  {size_t i;for(i=0;i<3u;++i){
  }}
  if(value->values.size>320u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->values.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_wire_size(roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor const* value) {size_t n=0;
  n+=4+value->key.size;
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  n+=4;
  if(value->values.size>320u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_write_fields(roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->key.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->key.data,value->key.size);*offset+=value->key.size;
  {size_t i;for(i=0;i<3u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->shape[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,value->values.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->values.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->values.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor* value) {
  if(*offset>n||n-*offset<4)return 0;value->key.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->key.size>64u||value->key.size>n-*offset)return 0;uwvm_bounded_copy(value->key.data,p+*offset,value->key.size);*offset+=value->key.size;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->shape[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->values.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->values.size>320u)return 0;
  {size_t i;for(i=0;i<value->values.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->values.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_encode(roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor* value) {
  roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>1364u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for uwvm_groot_bridge/msg/TextSequence.key; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[64];} roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_key_text;
/** UTF-8 byte storage for uwvm_groot_bridge/msg/TextSequence.texts; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[256];} roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_texts_text;
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_texts_text data[4];} roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_texts_sequence;
/** Complete ROS2 uwvm_groot_bridge/msg/TextSequence value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence {
  /** Declared field key; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_key_text key;
  /** Declared field shape; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t shape[2];
  /** Declared field texts; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_texts_sequence texts;
} roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence;
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_TEXTSEQUENCE_MAX_WIRE_BYTES 1120u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_TEXTSEQUENCE_MIN_WIRE_BYTES 16u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_MSG_TEXTSEQUENCE_CODEC_IDENTITY "UWVMROS1_895be974f2560f0afff7614f5400ff7205e013cda269825a8443628621c8e80d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_valid(roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence const* value) {
  if(!value)return 0;
  if(value->key.size>64u||!uwvm_bounded_utf8(value->key.data,value->key.size))return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  if(value->texts.size>4u)return 0;
  {size_t i;for(i=0;i<value->texts.size;++i){
  if(value->texts.data[i].size>256u||!uwvm_bounded_utf8(value->texts.data[i].data,value->texts.data[i].size))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_wire_size(roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence const* value) {size_t n=0;
  n+=4+value->key.size;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=4;
  if(value->texts.size>4u)return 0;
  {size_t i;for(i=0;i<value->texts.size;++i){
  n+=4+value->texts.data[i].size;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_write_fields(roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->key.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->key.data,value->key.size);*offset+=value->key.size;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->shape[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,value->texts.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->texts.size;++i){
  uwvm_bounded_put(p+*offset,value->texts.data[i].size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->texts.data[i].data,value->texts.data[i].size);*offset+=value->texts.data[i].size;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence* value) {
  if(*offset>n||n-*offset<4)return 0;value->key.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->key.size>64u||value->key.size>n-*offset)return 0;uwvm_bounded_copy(value->key.data,p+*offset,value->key.size);*offset+=value->key.size;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->shape[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->texts.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->texts.size>4u)return 0;
  {size_t i;for(i=0;i<value->texts.size;++i){
  if(*offset>n||n-*offset<4)return 0;value->texts.data[i].size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->texts.data[i].size>256u||value->texts.data[i].size>n-*offset)return 0;uwvm_bounded_copy(value->texts.data[i].data,p+*offset,value->texts.data[i].size);*offset+=value->texts.data[i].size;
  }}
  return roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_encode(roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence* value) {
  roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence next={0};size_t offset=0;if(!value||(length&&!input)||length<16u||length>1120u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most3 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame data[3];} roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_video_sequence;
/** At most4 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor data[4];} roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_state_sequence;
/** At most2 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence data[2];} roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_language_sequence;
/** Complete ROS2 uwvm_groot_bridge/srv/InferAction_Request value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request {
  /** Declared field application_nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t application_nonce;
  /** Declared field video; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_video_sequence video;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_state_sequence state;
  /** Declared field language; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_language_sequence language;
} roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request;
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_SRV_INFERACTION_REQUEST_MAX_WIRE_BYTES 10296u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_SRV_INFERACTION_REQUEST_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_SRV_INFERACTION_REQUEST_CODEC_IDENTITY "UWVMROS1_78f85929fb8646e9a92f17cafa3e8296fde42aaf49a56b271531ebad869a0fa6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_valid(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request const* value) {
  if(!value)return 0;
  if(value->video.size>3u)return 0;
  {size_t i;for(i=0;i<value->video.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_valid(&value->video.data[i]))return 0;
  }}
  if(value->state.size>4u)return 0;
  {size_t i;for(i=0;i<value->state.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_valid(&value->state.data[i]))return 0;
  }}
  if(value->language.size>2u)return 0;
  {size_t i;for(i=0;i<value->language.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_valid(&value->language.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_wire_size(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request const* value) {size_t n=0;
  n+=8;
  n+=4;
  if(value->video.size>3u)return 0;
  {size_t i;for(i=0;i<value->video.size;++i){
  n+=roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_wire_size(&value->video.data[i]);
  }}
  n+=4;
  if(value->state.size>4u)return 0;
  {size_t i;for(i=0;i<value->state.size;++i){
  n+=roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_wire_size(&value->state.data[i]);
  }}
  n+=4;
  if(value->language.size>2u)return 0;
  {size_t i;for(i=0;i<value->language.size;++i){
  n+=roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_wire_size(&value->language.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_write_fields(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->application_nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->video.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->video.size;++i){
  roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_write_fields(&value->video.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->state.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->state.size;++i){
  roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_write_fields(&value->state.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,value->language.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->language.size;++i){
  roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_write_fields(&value->language.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->application_nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->video.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->video.size>3u)return 0;
  {size_t i;for(i=0;i<value->video.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_RGBFrame_read_fields(p,n,offset,&value->video.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->state.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->state.size>4u)return 0;
  {size_t i;for(i=0;i<value->state.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_read_fields(p,n,offset,&value->state.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<4)return 0;value->language.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->language.size>2u)return 0;
  {size_t i;for(i=0;i<value->language.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_TextSequence_read_fields(p,n,offset,&value->language.data[i]))return 0;
  }}
  return roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_encode(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request* value) {
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>10296u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Request_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most8 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor data[8];} roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_action_sequence;
/** Complete ROS2 uwvm_groot_bridge/srv/InferAction_Response value; no field projects away native data. */
typedef struct roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response {
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t status;
  /** Declared field application_nonce; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t application_nonce;
  /** Declared field action; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_action_sequence action;
  /** Declared field info_is_empty; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t info_is_empty;
} roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response;
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_SRV_INFERACTION_RESPONSE_MAX_WIRE_BYTES 10929u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_SRV_INFERACTION_RESPONSE_MIN_WIRE_BYTES 17u
#define ROSWASMC_BOUNDED_2_UWVM_GROOT_BRIDGE_SRV_INFERACTION_RESPONSE_CODEC_IDENTITY "UWVMROS1_787281a2adea187ae43ff951a414dd7563b3e08318414e5199e1950b8143a3ce"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_valid(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response const* value) {
  if(!value)return 0;
  if(value->action.size>8u)return 0;
  {size_t i;for(i=0;i<value->action.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_valid(&value->action.data[i]))return 0;
  }}
  if(value->info_is_empty>1)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_wire_size(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response const* value) {size_t n=0;
  n+=4;
  n+=8;
  n+=4;
  if(value->action.size>8u)return 0;
  {size_t i;for(i=0;i<value->action.size;++i){
  n+=roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_wire_size(&value->action.data[i]);
  }}
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_write_fields(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->application_nonce,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->action.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->action.size;++i){
  roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_write_fields(&value->action.data[i],p,offset);
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->info_is_empty,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->status,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->application_nonce,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->action.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->action.size>8u)return 0;
  {size_t i;for(i=0;i<value->action.size;++i){
  if(!roswasmc_bounded_2_uwvm_groot_bridge_msg_FloatTensor_read_fields(p,n,offset,&value->action.data[i]))return 0;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->info_is_empty,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_encode(roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_decode(void const* input,size_t length,roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response* value) {
  roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response next={0};size_t offset=0;if(!value||(length&&!input)||length<17u||length>10929u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_uwvm_groot_bridge_srv_InferAction_Response_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
