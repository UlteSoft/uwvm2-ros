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
/** At most2048 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[2048];} roswasmc_bounded_2_unitree_go_msg_AudioData_data_sequence;
/** Complete ROS2 unitree_go/msg/AudioData value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_AudioData {
  /** Declared field time_frame; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t time_frame;
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_AudioData_data_sequence data;
} roswasmc_bounded_2_unitree_go_msg_AudioData;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_AUDIODATA_MAX_WIRE_BYTES 2060u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_AUDIODATA_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_AUDIODATA_CODEC_IDENTITY "UWVMROS1_aa14877a076c7fc6b662efa965cc8a75f42b3f84990a078c7c3c5def01d2c9fe"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_AudioData_valid(roswasmc_bounded_2_unitree_go_msg_AudioData const* value) {
  if(!value)return 0;
  if(value->data.size>2048u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_AudioData_wire_size(roswasmc_bounded_2_unitree_go_msg_AudioData const* value) {size_t n=0;
  n+=8;
  n+=4;
  if(value->data.size>2048u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_AudioData_write_fields(roswasmc_bounded_2_unitree_go_msg_AudioData const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->time_frame,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->data.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->data.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->data.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_AudioData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_AudioData* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->time_frame,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->data.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->data.size>2048u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->data.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_unitree_go_msg_AudioData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_AudioData_encode(roswasmc_bounded_2_unitree_go_msg_AudioData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_AudioData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_AudioData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_AudioData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_AudioData_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_AudioData* value) {
  roswasmc_bounded_2_unitree_go_msg_AudioData next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>2060u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_AudioData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/BmsCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_BmsCmd {
  /** Declared field off; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t off;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t reserve[3];
} roswasmc_bounded_2_unitree_go_msg_BmsCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_BMSCMD_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_BMSCMD_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_BMSCMD_CODEC_IDENTITY "UWVMROS1_d212c1ab0c92d64801a3b10c3a4d79517ea24a830db3d85ca2bb18e1e0ea5339"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_BmsCmd_valid(roswasmc_bounded_2_unitree_go_msg_BmsCmd const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<3u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_BmsCmd_wire_size(roswasmc_bounded_2_unitree_go_msg_BmsCmd const* value) {size_t n=0;
  n+=1;
  {size_t i;for(i=0;i<3u;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_BmsCmd_write_fields(roswasmc_bounded_2_unitree_go_msg_BmsCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->off,1);*offset+=1;
  {size_t i;for(i=0;i<3u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_BmsCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_BmsCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->off,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->reserve[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_unitree_go_msg_BmsCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_BmsCmd_encode(roswasmc_bounded_2_unitree_go_msg_BmsCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_BmsCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_BmsCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_BmsCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_BmsCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_BmsCmd* value) {
  roswasmc_bounded_2_unitree_go_msg_BmsCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_BmsCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/BmsState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_BmsState {
  /** Declared field version_high; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t version_high;
  /** Declared field version_low; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t version_low;
  /** Declared field status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t status;
  /** Declared field soc; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t soc;
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  int32_t current;
  /** Declared field cycle; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t cycle;
  /** Declared field bq_ntc; canonical capacity is part of CODEC_IDENTITY. */
  int8_t bq_ntc[2];
  /** Declared field mcu_ntc; canonical capacity is part of CODEC_IDENTITY. */
  int8_t mcu_ntc[2];
  /** Declared field cell_vol; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t cell_vol[15];
} roswasmc_bounded_2_unitree_go_msg_BmsState;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_BMSSTATE_MAX_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_BMSSTATE_MIN_WIRE_BYTES 44u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_BMSSTATE_CODEC_IDENTITY "UWVMROS1_7e4d5e3c0a2eddddf39c68b73bff5f7721e1623b0a9322dce23d3e551a07f5b0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_BmsState_valid(roswasmc_bounded_2_unitree_go_msg_BmsState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<15u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_BmsState_wire_size(roswasmc_bounded_2_unitree_go_msg_BmsState const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=1;
  n+=4;
  n+=2;
  {size_t i;for(i=0;i<2u;++i){
  n+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=1;
  }}
  {size_t i;for(i=0;i<15u;++i){
  n+=2;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_BmsState_write_fields(roswasmc_bounded_2_unitree_go_msg_BmsState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->version_high,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->version_low,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->soc,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->current,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cycle,2);*offset+=2;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->bq_ntc[i],1);*offset+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->mcu_ntc[i],1);*offset+=1;
  }}
  {size_t i;for(i=0;i<15u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->cell_vol[i],2);*offset+=2;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_BmsState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_BmsState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->version_high,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->version_low,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->soc,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->cycle,&bits,2);}*offset+=2;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->bq_ntc[i],&bits,1);}*offset+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mcu_ntc[i],&bits,1);}*offset+=1;
  }}
  {size_t i;for(i=0;i<15u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->cell_vol[i],&bits,2);}*offset+=2;
  }}
  return roswasmc_bounded_2_unitree_go_msg_BmsState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_BmsState_encode(roswasmc_bounded_2_unitree_go_msg_BmsState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_BmsState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_BmsState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_BmsState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_BmsState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_BmsState* value) {
  roswasmc_bounded_2_unitree_go_msg_BmsState next={0};size_t offset=0;if(!value||(length&&!input)||length<44u||length>44u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_BmsState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/Error value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_Error {
  /** Declared field source; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t source;
  /** Declared field state; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t state;
} roswasmc_bounded_2_unitree_go_msg_Error;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_ERROR_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_ERROR_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_ERROR_CODEC_IDENTITY "UWVMROS1_c3e572e08288b6ed0b32575e645fe546f92b5091d62e57db01ec00d27f5e4afd"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_Error_valid(roswasmc_bounded_2_unitree_go_msg_Error const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_Error_wire_size(roswasmc_bounded_2_unitree_go_msg_Error const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_Error_write_fields(roswasmc_bounded_2_unitree_go_msg_Error const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->source,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->state,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_Error_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_Error* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->source,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->state,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_go_msg_Error_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Error_encode(roswasmc_bounded_2_unitree_go_msg_Error const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_Error_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_Error_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_Error_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Error_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_Error* value) {
  roswasmc_bounded_2_unitree_go_msg_Error next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_Error_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[1024];} roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_video720p_sequence;
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[1024];} roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_video360p_sequence;
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[1024];} roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_video180p_sequence;
/** Complete ROS2 unitree_go/msg/Go2FrontVideoData value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData {
  /** Declared field time_frame; canonical capacity is part of CODEC_IDENTITY. */
  uint64_t time_frame;
  /** Declared field video720p; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_video720p_sequence video720p;
  /** Declared field video360p; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_video360p_sequence video360p;
  /** Declared field video180p; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_video180p_sequence video180p;
} roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_GO2FRONTVIDEODATA_MAX_WIRE_BYTES 3092u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_GO2FRONTVIDEODATA_MIN_WIRE_BYTES 20u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_GO2FRONTVIDEODATA_CODEC_IDENTITY "UWVMROS1_322b6bd4e563436838fe6a05174d173c0d95eb48108b889dca4f61ce6cc71c60"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_valid(roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData const* value) {
  if(!value)return 0;
  if(value->video720p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video720p.size;++i){
  }}
  if(value->video360p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video360p.size;++i){
  }}
  if(value->video180p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video180p.size;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_wire_size(roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData const* value) {size_t n=0;
  n+=8;
  n+=4;
  if(value->video720p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video720p.size;++i){
  n+=1;
  }}
  n+=4;
  if(value->video360p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video360p.size;++i){
  n+=1;
  }}
  n+=4;
  if(value->video180p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video180p.size;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_write_fields(roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->time_frame,8);*offset+=8;
  uwvm_bounded_put(p+*offset,value->video720p.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->video720p.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->video720p.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,value->video360p.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->video360p.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->video360p.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,value->video180p.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->video180p.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->video180p.data[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->time_frame,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->video720p.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->video720p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video720p.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->video720p.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;value->video360p.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->video360p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video360p.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->video360p.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;value->video180p.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->video180p.size>1024u)return 0;
  {size_t i;for(i=0;i<value->video180p.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->video180p.data[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_encode(roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData* value) {
  roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData next={0};size_t offset=0;if(!value||(length&&!input)||length<20u||length>3092u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_Go2FrontVideoData_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for unitree_go/msg/HeightMap.frame_id; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_unitree_go_msg_HeightMap_frame_id_text;
/** At most2048 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;float data[2048];} roswasmc_bounded_2_unitree_go_msg_HeightMap_data_sequence;
/** Complete ROS2 unitree_go/msg/HeightMap value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_HeightMap {
  /** Declared field stamp; canonical capacity is part of CODEC_IDENTITY. */
  double stamp;
  /** Declared field frame_id; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_HeightMap_frame_id_text frame_id;
  /** Declared field resolution; canonical capacity is part of CODEC_IDENTITY. */
  float resolution;
  /** Declared field width; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t width;
  /** Declared field height; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t height;
  /** Declared field origin; canonical capacity is part of CODEC_IDENTITY. */
  float origin[2];
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_HeightMap_data_sequence data;
} roswasmc_bounded_2_unitree_go_msg_HeightMap;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_HEIGHTMAP_MAX_WIRE_BYTES 8356u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_HEIGHTMAP_MIN_WIRE_BYTES 36u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_HEIGHTMAP_CODEC_IDENTITY "UWVMROS1_f4aed8c4e1ba382c418ca83a10ae617355f1169757128be8b7cca46611bb0012"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_HeightMap_valid(roswasmc_bounded_2_unitree_go_msg_HeightMap const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->stamp,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->frame_id.size>128u||!uwvm_bounded_utf8(value->frame_id.data,value->frame_id.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->resolution,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->origin[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  if(value->data.size>2048u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->data.data[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_HeightMap_wire_size(roswasmc_bounded_2_unitree_go_msg_HeightMap const* value) {size_t n=0;
  n+=8;
  n+=4+value->frame_id.size;
  n+=4;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=4;
  if(value->data.size>2048u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_HeightMap_write_fields(roswasmc_bounded_2_unitree_go_msg_HeightMap const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->stamp,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,value->frame_id.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->frame_id.data,value->frame_id.size);*offset+=value->frame_id.size;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->resolution,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->width,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->height,4);*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->origin[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,value->data.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->data.size;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->data.data[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_HeightMap_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_HeightMap* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->stamp,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->frame_id.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->frame_id.size>128u||value->frame_id.size>n-*offset)return 0;uwvm_bounded_copy(value->frame_id.data,p+*offset,value->frame_id.size);*offset+=value->frame_id.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->resolution,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->width,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->height,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->origin[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;value->data.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->data.size>2048u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->data.data[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_go_msg_HeightMap_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_HeightMap_encode(roswasmc_bounded_2_unitree_go_msg_HeightMap const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_HeightMap_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_HeightMap_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_HeightMap_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_HeightMap_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_HeightMap* value) {
  roswasmc_bounded_2_unitree_go_msg_HeightMap next={0};size_t offset=0;if(!value||(length&&!input)||length<36u||length>8356u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_HeightMap_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/IMUState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_IMUState {
  /** Declared field quaternion; canonical capacity is part of CODEC_IDENTITY. */
  float quaternion[4];
  /** Declared field gyroscope; canonical capacity is part of CODEC_IDENTITY. */
  float gyroscope[3];
  /** Declared field accelerometer; canonical capacity is part of CODEC_IDENTITY. */
  float accelerometer[3];
  /** Declared field rpy; canonical capacity is part of CODEC_IDENTITY. */
  float rpy[3];
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int8_t temperature;
} roswasmc_bounded_2_unitree_go_msg_IMUState;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_IMUSTATE_MAX_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_IMUSTATE_MIN_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_IMUSTATE_CODEC_IDENTITY "UWVMROS1_e5326e97c3bfaa06962ad1989d1aa13fa0e60af15d39d9ef6da6ac0a37261bbc"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_IMUState_valid(roswasmc_bounded_2_unitree_go_msg_IMUState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<4u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->quaternion[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gyroscope[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->accelerometer[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rpy[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_IMUState_wire_size(roswasmc_bounded_2_unitree_go_msg_IMUState const* value) {size_t n=0;
  {size_t i;for(i=0;i<4u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_IMUState_write_fields(roswasmc_bounded_2_unitree_go_msg_IMUState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<4u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->quaternion[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->gyroscope[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->accelerometer[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rpy[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_IMUState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_IMUState* value) {
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->quaternion[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->gyroscope[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->accelerometer[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rpy[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->temperature,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_unitree_go_msg_IMUState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_IMUState_encode(roswasmc_bounded_2_unitree_go_msg_IMUState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_IMUState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_IMUState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_IMUState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_IMUState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_IMUState* value) {
  roswasmc_bounded_2_unitree_go_msg_IMUState next={0};size_t offset=0;if(!value||(length&&!input)||length<53u||length>53u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_IMUState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/InterfaceConfig value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_InterfaceConfig {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field value; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t value;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t reserve[2];
} roswasmc_bounded_2_unitree_go_msg_InterfaceConfig;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_INTERFACECONFIG_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_INTERFACECONFIG_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_INTERFACECONFIG_CODEC_IDENTITY "UWVMROS1_9b96ed6672f6a3e039edb28dec8384cd195116dda929d5ba7be85ef7e7668110"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_valid(roswasmc_bounded_2_unitree_go_msg_InterfaceConfig const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_wire_size(roswasmc_bounded_2_unitree_go_msg_InterfaceConfig const* value) {size_t n=0;
  n+=1;
  n+=1;
  {size_t i;for(i=0;i<2u;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_write_fields(roswasmc_bounded_2_unitree_go_msg_InterfaceConfig const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->value,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_InterfaceConfig* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->value,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->reserve[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_encode(roswasmc_bounded_2_unitree_go_msg_InterfaceConfig const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_InterfaceConfig* value) {
  roswasmc_bounded_2_unitree_go_msg_InterfaceConfig next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_InterfaceConfig_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for unitree_go/msg/LidarState.firmware_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_unitree_go_msg_LidarState_firmware_version_text;
/** UTF-8 byte storage for unitree_go/msg/LidarState.software_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_unitree_go_msg_LidarState_software_version_text;
/** UTF-8 byte storage for unitree_go/msg/LidarState.sdk_version; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_unitree_go_msg_LidarState_sdk_version_text;
/** Complete ROS2 unitree_go/msg/LidarState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_LidarState {
  /** Declared field stamp; canonical capacity is part of CODEC_IDENTITY. */
  double stamp;
  /** Declared field firmware_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_LidarState_firmware_version_text firmware_version;
  /** Declared field software_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_LidarState_software_version_text software_version;
  /** Declared field sdk_version; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_LidarState_sdk_version_text sdk_version;
  /** Declared field sys_rotation_speed; canonical capacity is part of CODEC_IDENTITY. */
  float sys_rotation_speed;
  /** Declared field com_rotation_speed; canonical capacity is part of CODEC_IDENTITY. */
  float com_rotation_speed;
  /** Declared field error_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t error_state;
  /** Declared field cloud_frequency; canonical capacity is part of CODEC_IDENTITY. */
  float cloud_frequency;
  /** Declared field cloud_packet_loss_rate; canonical capacity is part of CODEC_IDENTITY. */
  float cloud_packet_loss_rate;
  /** Declared field cloud_size; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t cloud_size;
  /** Declared field cloud_scan_num; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t cloud_scan_num;
  /** Declared field imu_frequency; canonical capacity is part of CODEC_IDENTITY. */
  float imu_frequency;
  /** Declared field imu_packet_loss_rate; canonical capacity is part of CODEC_IDENTITY. */
  float imu_packet_loss_rate;
  /** Declared field imu_rpy; canonical capacity is part of CODEC_IDENTITY. */
  float imu_rpy[3];
  /** Declared field serial_recv_stamp; canonical capacity is part of CODEC_IDENTITY. */
  double serial_recv_stamp;
  /** Declared field serial_buffer_size; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t serial_buffer_size;
  /** Declared field serial_buffer_read; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t serial_buffer_read;
} roswasmc_bounded_2_unitree_go_msg_LidarState;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LIDARSTATE_MAX_WIRE_BYTES 465u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LIDARSTATE_MIN_WIRE_BYTES 81u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LIDARSTATE_CODEC_IDENTITY "UWVMROS1_edc9253eb66e466ae5a893b830b0bc5c9102ef18d2bbed7f7d9ead67bcb1a8e6"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_LidarState_valid(roswasmc_bounded_2_unitree_go_msg_LidarState const* value) {
  if(!value)return 0;
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->stamp,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  if(value->firmware_version.size>128u||!uwvm_bounded_utf8(value->firmware_version.data,value->firmware_version.size))return 0;
  if(value->software_version.size>128u||!uwvm_bounded_utf8(value->software_version.data,value->software_version.size))return 0;
  if(value->sdk_version.size>128u||!uwvm_bounded_utf8(value->sdk_version.data,value->sdk_version.size))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->sys_rotation_speed,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->com_rotation_speed,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cloud_frequency,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cloud_packet_loss_rate,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->imu_frequency,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->imu_packet_loss_rate,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->imu_rpy[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->serial_recv_stamp,8);if((bits&UINT64_C(0x7ff0000000000000))==UINT64_C(0x7ff0000000000000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_LidarState_wire_size(roswasmc_bounded_2_unitree_go_msg_LidarState const* value) {size_t n=0;
  n+=8;
  n+=4+value->firmware_version.size;
  n+=4+value->software_version.size;
  n+=4+value->sdk_version.size;
  n+=4;
  n+=4;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  n+=8;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_LidarState_write_fields(roswasmc_bounded_2_unitree_go_msg_LidarState const* value,unsigned char* p,size_t* offset) {
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->stamp,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,value->firmware_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->firmware_version.data,value->firmware_version.size);*offset+=value->firmware_version.size;
  uwvm_bounded_put(p+*offset,value->software_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->software_version.data,value->software_version.size);*offset+=value->software_version.size;
  uwvm_bounded_put(p+*offset,value->sdk_version.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->sdk_version.data,value->sdk_version.size);*offset+=value->sdk_version.size;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->sys_rotation_speed,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->com_rotation_speed,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_state,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cloud_frequency,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->cloud_packet_loss_rate,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->cloud_size,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cloud_scan_num,4);*offset+=4;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->imu_frequency,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->imu_packet_loss_rate,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->imu_rpy[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint64_t bits;uwvm_bounded_copy(&bits,&value->serial_recv_stamp,8);uwvm_bounded_put(p+*offset,bits,8);*offset+=8;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->serial_buffer_size,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->serial_buffer_read,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_LidarState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_LidarState* value) {
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->stamp,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;value->firmware_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->firmware_version.size>128u||value->firmware_version.size>n-*offset)return 0;uwvm_bounded_copy(value->firmware_version.data,p+*offset,value->firmware_version.size);*offset+=value->firmware_version.size;
  if(*offset>n||n-*offset<4)return 0;value->software_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->software_version.size>128u||value->software_version.size>n-*offset)return 0;uwvm_bounded_copy(value->software_version.data,p+*offset,value->software_version.size);*offset+=value->software_version.size;
  if(*offset>n||n-*offset<4)return 0;value->sdk_version.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->sdk_version.size>128u||value->sdk_version.size>n-*offset)return 0;uwvm_bounded_copy(value->sdk_version.data,p+*offset,value->sdk_version.size);*offset+=value->sdk_version.size;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sys_rotation_speed,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->com_rotation_speed,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->error_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cloud_frequency,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cloud_packet_loss_rate,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cloud_size,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->cloud_scan_num,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->imu_frequency,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->imu_packet_loss_rate,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->imu_rpy[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<8)return 0;{uint64_t bits=(uint64_t)uwvm_bounded_get(p+*offset,8);uwvm_bounded_copy(&value->serial_recv_stamp,&bits,8);}*offset+=8;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->serial_buffer_size,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->serial_buffer_read,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_go_msg_LidarState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_LidarState_encode(roswasmc_bounded_2_unitree_go_msg_LidarState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_LidarState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_LidarState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_LidarState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_LidarState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_LidarState* value) {
  roswasmc_bounded_2_unitree_go_msg_LidarState next={0};size_t offset=0;if(!value||(length&&!input)||length<81u||length>465u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_LidarState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/MotorCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_MotorCmd {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  float q;
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  float dq;
  /** Declared field tau; canonical capacity is part of CODEC_IDENTITY. */
  float tau;
  /** Declared field kp; canonical capacity is part of CODEC_IDENTITY. */
  float kp;
  /** Declared field kd; canonical capacity is part of CODEC_IDENTITY. */
  float kd;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[3];
} roswasmc_bounded_2_unitree_go_msg_MotorCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORCMD_MAX_WIRE_BYTES 33u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORCMD_MIN_WIRE_BYTES 33u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORCMD_CODEC_IDENTITY "UWVMROS1_e5b88c02a6d339ae4ae2706678a3d82f89904f024b6068f4fd174e1e045b0989"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorCmd_valid(roswasmc_bounded_2_unitree_go_msg_MotorCmd const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_MotorCmd_wire_size(roswasmc_bounded_2_unitree_go_msg_MotorCmd const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_MotorCmd_write_fields(roswasmc_bounded_2_unitree_go_msg_MotorCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kp,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->kd,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<3u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_MotorCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kp,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->kd,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_go_msg_MotorCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorCmd_encode(roswasmc_bounded_2_unitree_go_msg_MotorCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_MotorCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_MotorCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_MotorCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_MotorCmd* value) {
  roswasmc_bounded_2_unitree_go_msg_MotorCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<33u||length>33u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_MotorCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/LowCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_LowCmd {
  /** Declared field head; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t head[2];
  /** Declared field level_flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t level_flag;
  /** Declared field frame_reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t frame_reserve;
  /** Declared field sn; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t sn[2];
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t version[2];
  /** Declared field bandwidth; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t bandwidth;
  /** Declared field motor_cmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_MotorCmd motor_cmd[20];
  /** Declared field bms_cmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_BmsCmd bms_cmd;
  /** Declared field wireless_remote; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t wireless_remote[40];
  /** Declared field led; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t led[12];
  /** Declared field fan; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t fan[2];
  /** Declared field gpio; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t gpio;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve;
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
} roswasmc_bounded_2_unitree_go_msg_LowCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWCMD_MAX_WIRE_BYTES 749u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWCMD_MIN_WIRE_BYTES 749u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWCMD_CODEC_IDENTITY "UWVMROS1_efd061d06d38ecc686edb95d1a891c26860b16da1e35747fc97eeaf4a5d93d63"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_LowCmd_valid(roswasmc_bounded_2_unitree_go_msg_LowCmd const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorCmd_valid(&value->motor_cmd[i]))return 0;
  }}
  if(!roswasmc_bounded_2_unitree_go_msg_BmsCmd_valid(&value->bms_cmd))return 0;
  {size_t i;for(i=0;i<40u;++i){
  }}
  {size_t i;for(i=0;i<12u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_LowCmd_wire_size(roswasmc_bounded_2_unitree_go_msg_LowCmd const* value) {size_t n=0;
  {size_t i;for(i=0;i<2u;++i){
  n+=1;
  }}
  n+=1;
  n+=1;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=2;
  {size_t i;for(i=0;i<20u;++i){
  n+=roswasmc_bounded_2_unitree_go_msg_MotorCmd_wire_size(&value->motor_cmd[i]);
  }}
  n+=roswasmc_bounded_2_unitree_go_msg_BmsCmd_wire_size(&value->bms_cmd);
  {size_t i;for(i=0;i<40u;++i){
  n+=1;
  }}
  {size_t i;for(i=0;i<12u;++i){
  n+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=1;
  }}
  n+=1;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_LowCmd_write_fields(roswasmc_bounded_2_unitree_go_msg_LowCmd const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->head[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->level_flag,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->frame_reserve,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->sn[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->bandwidth,2);*offset+=2;
  {size_t i;for(i=0;i<20u;++i){
  roswasmc_bounded_2_unitree_go_msg_MotorCmd_write_fields(&value->motor_cmd[i],p,offset);
  }}
  roswasmc_bounded_2_unitree_go_msg_BmsCmd_write_fields(&value->bms_cmd,p,offset);
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->wireless_remote[i],1);*offset+=1;
  }}
  {size_t i;for(i=0;i<12u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->led[i],1);*offset+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->fan[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->gpio,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_LowCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_LowCmd* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->head[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->level_flag,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->frame_reserve,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sn[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->version[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->bandwidth,&bits,2);}*offset+=2;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorCmd_read_fields(p,n,offset,&value->motor_cmd[i]))return 0;
  }}
  if(!roswasmc_bounded_2_unitree_go_msg_BmsCmd_read_fields(p,n,offset,&value->bms_cmd))return 0;
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->wireless_remote[i],&bits,1);}*offset+=1;
  }}
  {size_t i;for(i=0;i<12u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->led[i],&bits,1);}*offset+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->fan[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->gpio,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_go_msg_LowCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_LowCmd_encode(roswasmc_bounded_2_unitree_go_msg_LowCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_LowCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_LowCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_LowCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_LowCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_LowCmd* value) {
  roswasmc_bounded_2_unitree_go_msg_LowCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<749u||length>749u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_LowCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/MotorState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_MotorState {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  float q;
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  float dq;
  /** Declared field ddq; canonical capacity is part of CODEC_IDENTITY. */
  float ddq;
  /** Declared field tau_est; canonical capacity is part of CODEC_IDENTITY. */
  float tau_est;
  /** Declared field q_raw; canonical capacity is part of CODEC_IDENTITY. */
  float q_raw;
  /** Declared field dq_raw; canonical capacity is part of CODEC_IDENTITY. */
  float dq_raw;
  /** Declared field ddq_raw; canonical capacity is part of CODEC_IDENTITY. */
  float ddq_raw;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int8_t temperature;
  /** Declared field lost; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t lost;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[2];
} roswasmc_bounded_2_unitree_go_msg_MotorState;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORSTATE_MAX_WIRE_BYTES 42u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORSTATE_MIN_WIRE_BYTES 42u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORSTATE_CODEC_IDENTITY "UWVMROS1_b0b5e638b9fd9eec6d0019422d417f80c60e61d47f4750fe9adac8c5084a2a62"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorState_valid(roswasmc_bounded_2_unitree_go_msg_MotorState const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau_est,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q_raw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq_raw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq_raw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_MotorState_wire_size(roswasmc_bounded_2_unitree_go_msg_MotorState const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_MotorState_write_fields(roswasmc_bounded_2_unitree_go_msg_MotorState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau_est,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q_raw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq_raw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq_raw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->lost,4);*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_MotorState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ddq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau_est,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q_raw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq_raw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ddq_raw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->temperature,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->lost,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_go_msg_MotorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorState_encode(roswasmc_bounded_2_unitree_go_msg_MotorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_MotorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_MotorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_MotorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_MotorState* value) {
  roswasmc_bounded_2_unitree_go_msg_MotorState next={0};size_t offset=0;if(!value||(length&&!input)||length<42u||length>42u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_MotorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/LowState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_LowState {
  /** Declared field head; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t head[2];
  /** Declared field level_flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t level_flag;
  /** Declared field frame_reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t frame_reserve;
  /** Declared field sn; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t sn[2];
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t version[2];
  /** Declared field bandwidth; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t bandwidth;
  /** Declared field imu_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_IMUState imu_state;
  /** Declared field motor_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_MotorState motor_state[20];
  /** Declared field bms_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_BmsState bms_state;
  /** Declared field foot_force; canonical capacity is part of CODEC_IDENTITY. */
  int16_t foot_force[4];
  /** Declared field foot_force_est; canonical capacity is part of CODEC_IDENTITY. */
  int16_t foot_force_est[4];
  /** Declared field tick; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t tick;
  /** Declared field wireless_remote; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t wireless_remote[40];
  /** Declared field bit_flag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t bit_flag;
  /** Declared field adc_reel; canonical capacity is part of CODEC_IDENTITY. */
  float adc_reel;
  /** Declared field temperature_ntc1; canonical capacity is part of CODEC_IDENTITY. */
  int8_t temperature_ntc1;
  /** Declared field temperature_ntc2; canonical capacity is part of CODEC_IDENTITY. */
  int8_t temperature_ntc2;
  /** Declared field power_v; canonical capacity is part of CODEC_IDENTITY. */
  float power_v;
  /** Declared field power_a; canonical capacity is part of CODEC_IDENTITY. */
  float power_a;
  /** Declared field fan_frequency; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t fan_frequency[4];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve;
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
} roswasmc_bounded_2_unitree_go_msg_LowState;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWSTATE_MAX_WIRE_BYTES 1050u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWSTATE_MIN_WIRE_BYTES 1050u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_LOWSTATE_CODEC_IDENTITY "UWVMROS1_56c01d52b26d7b2ef1fa7475fc8a353777ed6952a6f1ceee01da81f1edad3cab"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_LowState_valid(roswasmc_bounded_2_unitree_go_msg_LowState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  if(!roswasmc_bounded_2_unitree_go_msg_IMUState_valid(&value->imu_state))return 0;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorState_valid(&value->motor_state[i]))return 0;
  }}
  if(!roswasmc_bounded_2_unitree_go_msg_BmsState_valid(&value->bms_state))return 0;
  {size_t i;for(i=0;i<4u;++i){
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  {size_t i;for(i=0;i<40u;++i){
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->adc_reel,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_v,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_a,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<4u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_LowState_wire_size(roswasmc_bounded_2_unitree_go_msg_LowState const* value) {size_t n=0;
  {size_t i;for(i=0;i<2u;++i){
  n+=1;
  }}
  n+=1;
  n+=1;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=2;
  n+=roswasmc_bounded_2_unitree_go_msg_IMUState_wire_size(&value->imu_state);
  {size_t i;for(i=0;i<20u;++i){
  n+=roswasmc_bounded_2_unitree_go_msg_MotorState_wire_size(&value->motor_state[i]);
  }}
  n+=roswasmc_bounded_2_unitree_go_msg_BmsState_wire_size(&value->bms_state);
  {size_t i;for(i=0;i<4u;++i){
  n+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=2;
  }}
  n+=4;
  {size_t i;for(i=0;i<40u;++i){
  n+=1;
  }}
  n+=1;
  n+=4;
  n+=1;
  n+=1;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<4u;++i){
  n+=2;
  }}
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_LowState_write_fields(roswasmc_bounded_2_unitree_go_msg_LowState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->head[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->level_flag,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->frame_reserve,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->sn[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->bandwidth,2);*offset+=2;
  roswasmc_bounded_2_unitree_go_msg_IMUState_write_fields(&value->imu_state,p,offset);
  {size_t i;for(i=0;i<20u;++i){
  roswasmc_bounded_2_unitree_go_msg_MotorState_write_fields(&value->motor_state[i],p,offset);
  }}
  roswasmc_bounded_2_unitree_go_msg_BmsState_write_fields(&value->bms_state,p,offset);
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->foot_force[i],2);*offset+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->foot_force_est[i],2);*offset+=2;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->tick,4);*offset+=4;
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->wireless_remote[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->bit_flag,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->adc_reel,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature_ntc1,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature_ntc2,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_v,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->power_a,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->fan_frequency[i],2);*offset+=2;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_LowState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_LowState* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->head[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->level_flag,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->frame_reserve,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sn[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->version[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->bandwidth,&bits,2);}*offset+=2;
  if(!roswasmc_bounded_2_unitree_go_msg_IMUState_read_fields(p,n,offset,&value->imu_state))return 0;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorState_read_fields(p,n,offset,&value->motor_state[i]))return 0;
  }}
  if(!roswasmc_bounded_2_unitree_go_msg_BmsState_read_fields(p,n,offset,&value->bms_state))return 0;
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->foot_force[i],&bits,2);}*offset+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->foot_force_est[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tick,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->wireless_remote[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->bit_flag,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->adc_reel,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->temperature_ntc1,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->temperature_ntc2,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->power_v,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->power_a,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->fan_frequency[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_go_msg_LowState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_LowState_encode(roswasmc_bounded_2_unitree_go_msg_LowState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_LowState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_LowState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_LowState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_LowState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_LowState* value) {
  roswasmc_bounded_2_unitree_go_msg_LowState next={0};size_t offset=0;if(!value||(length&&!input)||length<1050u||length>1050u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_LowState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most20 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_unitree_go_msg_MotorCmd data[20];} roswasmc_bounded_2_unitree_go_msg_MotorCmds_cmds_sequence;
/** Complete ROS2 unitree_go/msg/MotorCmds value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_MotorCmds {
  /** Declared field cmds; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_MotorCmds_cmds_sequence cmds;
} roswasmc_bounded_2_unitree_go_msg_MotorCmds;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORCMDS_MAX_WIRE_BYTES 664u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORCMDS_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORCMDS_CODEC_IDENTITY "UWVMROS1_59f6b09147eb868cad93205c27cdd339e55f5928c8eef32da11ec5675c61a671"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorCmds_valid(roswasmc_bounded_2_unitree_go_msg_MotorCmds const* value) {
  if(!value)return 0;
  if(value->cmds.size>20u)return 0;
  {size_t i;for(i=0;i<value->cmds.size;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorCmd_valid(&value->cmds.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_MotorCmds_wire_size(roswasmc_bounded_2_unitree_go_msg_MotorCmds const* value) {size_t n=0;
  n+=4;
  if(value->cmds.size>20u)return 0;
  {size_t i;for(i=0;i<value->cmds.size;++i){
  n+=roswasmc_bounded_2_unitree_go_msg_MotorCmd_wire_size(&value->cmds.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_MotorCmds_write_fields(roswasmc_bounded_2_unitree_go_msg_MotorCmds const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->cmds.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->cmds.size;++i){
  roswasmc_bounded_2_unitree_go_msg_MotorCmd_write_fields(&value->cmds.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorCmds_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_MotorCmds* value) {
  if(*offset>n||n-*offset<4)return 0;value->cmds.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->cmds.size>20u)return 0;
  {size_t i;for(i=0;i<value->cmds.size;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorCmd_read_fields(p,n,offset,&value->cmds.data[i]))return 0;
  }}
  return roswasmc_bounded_2_unitree_go_msg_MotorCmds_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorCmds_encode(roswasmc_bounded_2_unitree_go_msg_MotorCmds const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_MotorCmds_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_MotorCmds_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_MotorCmds_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorCmds_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_MotorCmds* value) {
  roswasmc_bounded_2_unitree_go_msg_MotorCmds next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>664u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_MotorCmds_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** At most20 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;roswasmc_bounded_2_unitree_go_msg_MotorState data[20];} roswasmc_bounded_2_unitree_go_msg_MotorStates_states_sequence;
/** Complete ROS2 unitree_go/msg/MotorStates value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_MotorStates {
  /** Declared field states; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_MotorStates_states_sequence states;
} roswasmc_bounded_2_unitree_go_msg_MotorStates;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORSTATES_MAX_WIRE_BYTES 844u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORSTATES_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_MOTORSTATES_CODEC_IDENTITY "UWVMROS1_0843528158199493384c36a65b0fb1e15244d21160c33aac82f9a87718c08178"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorStates_valid(roswasmc_bounded_2_unitree_go_msg_MotorStates const* value) {
  if(!value)return 0;
  if(value->states.size>20u)return 0;
  {size_t i;for(i=0;i<value->states.size;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorState_valid(&value->states.data[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_MotorStates_wire_size(roswasmc_bounded_2_unitree_go_msg_MotorStates const* value) {size_t n=0;
  n+=4;
  if(value->states.size>20u)return 0;
  {size_t i;for(i=0;i<value->states.size;++i){
  n+=roswasmc_bounded_2_unitree_go_msg_MotorState_wire_size(&value->states.data[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_MotorStates_write_fields(roswasmc_bounded_2_unitree_go_msg_MotorStates const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->states.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->states.size;++i){
  roswasmc_bounded_2_unitree_go_msg_MotorState_write_fields(&value->states.data[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_MotorStates_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_MotorStates* value) {
  if(*offset>n||n-*offset<4)return 0;value->states.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->states.size>20u)return 0;
  {size_t i;for(i=0;i<value->states.size;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_MotorState_read_fields(p,n,offset,&value->states.data[i]))return 0;
  }}
  return roswasmc_bounded_2_unitree_go_msg_MotorStates_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorStates_encode(roswasmc_bounded_2_unitree_go_msg_MotorStates const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_MotorStates_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_MotorStates_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_MotorStates_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_MotorStates_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_MotorStates* value) {
  roswasmc_bounded_2_unitree_go_msg_MotorStates next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>844u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_MotorStates_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/PathPoint value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_PathPoint {
  /** Declared field t_from_start; canonical capacity is part of CODEC_IDENTITY. */
  float t_from_start;
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field yaw; canonical capacity is part of CODEC_IDENTITY. */
  float yaw;
  /** Declared field vx; canonical capacity is part of CODEC_IDENTITY. */
  float vx;
  /** Declared field vy; canonical capacity is part of CODEC_IDENTITY. */
  float vy;
  /** Declared field vyaw; canonical capacity is part of CODEC_IDENTITY. */
  float vyaw;
} roswasmc_bounded_2_unitree_go_msg_PathPoint;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_PATHPOINT_MAX_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_PATHPOINT_MIN_WIRE_BYTES 28u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_PATHPOINT_CODEC_IDENTITY "UWVMROS1_35eb1cc629dcecbace17b342029c287310c30d4e6d988740bf2c212a1251cee4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_PathPoint_valid(roswasmc_bounded_2_unitree_go_msg_PathPoint const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->t_from_start,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vy,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vyaw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_PathPoint_wire_size(roswasmc_bounded_2_unitree_go_msg_PathPoint const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_PathPoint_write_fields(roswasmc_bounded_2_unitree_go_msg_PathPoint const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->t_from_start,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vy,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->vyaw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_PathPoint_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_PathPoint* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->t_from_start,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->yaw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->vx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->vy,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->vyaw,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_go_msg_PathPoint_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_PathPoint_encode(roswasmc_bounded_2_unitree_go_msg_PathPoint const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_PathPoint_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_PathPoint_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_PathPoint_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_PathPoint_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_PathPoint* value) {
  roswasmc_bounded_2_unitree_go_msg_PathPoint next={0};size_t offset=0;if(!value||(length&&!input)||length<28u||length>28u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_PathPoint_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for unitree_go/msg/Req.uuid; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_unitree_go_msg_Req_uuid_text;
/** UTF-8 byte storage for unitree_go/msg/Req.body; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_unitree_go_msg_Req_body_text;
/** Complete ROS2 unitree_go/msg/Req value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_Req {
  /** Declared field uuid; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Req_uuid_text uuid;
  /** Declared field body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Req_body_text body;
} roswasmc_bounded_2_unitree_go_msg_Req;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_REQ_MAX_WIRE_BYTES 4232u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_REQ_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_REQ_CODEC_IDENTITY "UWVMROS1_43e0126993336d81a3f6a6706979c19f3a193f6a1ef504cadcd864be8bfaf84e"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_Req_valid(roswasmc_bounded_2_unitree_go_msg_Req const* value) {
  if(!value)return 0;
  if(value->uuid.size>128u||!uwvm_bounded_utf8(value->uuid.data,value->uuid.size))return 0;
  if(value->body.size>4096u||!uwvm_bounded_utf8(value->body.data,value->body.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_Req_wire_size(roswasmc_bounded_2_unitree_go_msg_Req const* value) {size_t n=0;
  n+=4+value->uuid.size;
  n+=4+value->body.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_Req_write_fields(roswasmc_bounded_2_unitree_go_msg_Req const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->uuid.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->uuid.data,value->uuid.size);*offset+=value->uuid.size;
  uwvm_bounded_put(p+*offset,value->body.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->body.data,value->body.size);*offset+=value->body.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_Req_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_Req* value) {
  if(*offset>n||n-*offset<4)return 0;value->uuid.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->uuid.size>128u||value->uuid.size>n-*offset)return 0;uwvm_bounded_copy(value->uuid.data,p+*offset,value->uuid.size);*offset+=value->uuid.size;
  if(*offset>n||n-*offset<4)return 0;value->body.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->body.size>4096u||value->body.size>n-*offset)return 0;uwvm_bounded_copy(value->body.data,p+*offset,value->body.size);*offset+=value->body.size;
  return roswasmc_bounded_2_unitree_go_msg_Req_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Req_encode(roswasmc_bounded_2_unitree_go_msg_Req const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_Req_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_Req_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_Req_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Req_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_Req* value) {
  roswasmc_bounded_2_unitree_go_msg_Req next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>4232u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_Req_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** UTF-8 byte storage for unitree_go/msg/Res.uuid; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[128];} roswasmc_bounded_2_unitree_go_msg_Res_uuid_text;
/** At most1024 initialized logical elements; unused capacity is never serialized. */
typedef struct {uint32_t size;uint8_t data[1024];} roswasmc_bounded_2_unitree_go_msg_Res_data_sequence;
/** UTF-8 byte storage for unitree_go/msg/Res.body; size excludes padding/terminators. */
typedef struct {uint32_t size;unsigned char data[4096];} roswasmc_bounded_2_unitree_go_msg_Res_body_text;
/** Complete ROS2 unitree_go/msg/Res value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_Res {
  /** Declared field uuid; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Res_uuid_text uuid;
  /** Declared field data; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Res_data_sequence data;
  /** Declared field body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_Res_body_text body;
} roswasmc_bounded_2_unitree_go_msg_Res;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_RES_MAX_WIRE_BYTES 5260u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_RES_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_RES_CODEC_IDENTITY "UWVMROS1_53c1d85c0cb17cfdd7d3799d6e92602a70700e8c55da4045f501a6a12ff55bfb"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_Res_valid(roswasmc_bounded_2_unitree_go_msg_Res const* value) {
  if(!value)return 0;
  if(value->uuid.size>128u||!uwvm_bounded_utf8(value->uuid.data,value->uuid.size))return 0;
  if(value->data.size>1024u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  }}
  if(value->body.size>4096u||!uwvm_bounded_utf8(value->body.data,value->body.size))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_Res_wire_size(roswasmc_bounded_2_unitree_go_msg_Res const* value) {size_t n=0;
  n+=4+value->uuid.size;
  n+=4;
  if(value->data.size>1024u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  n+=1;
  }}
  n+=4+value->body.size;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_Res_write_fields(roswasmc_bounded_2_unitree_go_msg_Res const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,value->uuid.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->uuid.data,value->uuid.size);*offset+=value->uuid.size;
  uwvm_bounded_put(p+*offset,value->data.size,4);*offset+=4;
  {size_t i;for(i=0;i<value->data.size;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->data.data[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,value->body.size,4);*offset+=4;uwvm_bounded_copy(p+*offset,value->body.data,value->body.size);*offset+=value->body.size;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_Res_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_Res* value) {
  if(*offset>n||n-*offset<4)return 0;value->uuid.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->uuid.size>128u||value->uuid.size>n-*offset)return 0;uwvm_bounded_copy(value->uuid.data,p+*offset,value->uuid.size);*offset+=value->uuid.size;
  if(*offset>n||n-*offset<4)return 0;value->data.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->data.size>1024u)return 0;
  {size_t i;for(i=0;i<value->data.size;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->data.data[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;value->body.size=(uint32_t)uwvm_bounded_get(p+*offset,4);*offset+=4;
  if(value->body.size>4096u||value->body.size>n-*offset)return 0;uwvm_bounded_copy(value->body.data,p+*offset,value->body.size);*offset+=value->body.size;
  return roswasmc_bounded_2_unitree_go_msg_Res_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Res_encode(roswasmc_bounded_2_unitree_go_msg_Res const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_Res_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_Res_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_Res_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_Res_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_Res* value) {
  roswasmc_bounded_2_unitree_go_msg_Res next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>5260u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_Res_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/SportModeCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_SportModeCmd {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field gait_type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t gait_type;
  /** Declared field speed_level; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t speed_level;
  /** Declared field foot_raise_height; canonical capacity is part of CODEC_IDENTITY. */
  float foot_raise_height;
  /** Declared field body_height; canonical capacity is part of CODEC_IDENTITY. */
  float body_height;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  float position[2];
  /** Declared field euler; canonical capacity is part of CODEC_IDENTITY. */
  float euler[3];
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  float velocity[2];
  /** Declared field yaw_speed; canonical capacity is part of CODEC_IDENTITY. */
  float yaw_speed;
  /** Declared field bms_cmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_BmsCmd bms_cmd;
  /** Declared field path_point; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_PathPoint path_point[30];
} roswasmc_bounded_2_unitree_go_msg_SportModeCmd;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_SPORTMODECMD_MAX_WIRE_BYTES 887u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_SPORTMODECMD_MIN_WIRE_BYTES 887u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_SPORTMODECMD_CODEC_IDENTITY "UWVMROS1_cf90a065007b9b8cedde3f0b58aa79da3b39a1456dc1b5297f4fe799d92ff22c"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_SportModeCmd_valid(roswasmc_bounded_2_unitree_go_msg_SportModeCmd const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_raise_height,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->body_height,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->euler[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw_speed,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_2_unitree_go_msg_BmsCmd_valid(&value->bms_cmd))return 0;
  {size_t i;for(i=0;i<30u;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_PathPoint_valid(&value->path_point[i]))return 0;
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_SportModeCmd_wire_size(roswasmc_bounded_2_unitree_go_msg_SportModeCmd const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=4;
  n+=roswasmc_bounded_2_unitree_go_msg_BmsCmd_wire_size(&value->bms_cmd);
  {size_t i;for(i=0;i<30u;++i){
  n+=roswasmc_bounded_2_unitree_go_msg_PathPoint_wire_size(&value->path_point[i]);
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_SportModeCmd_write_fields(roswasmc_bounded_2_unitree_go_msg_SportModeCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->gait_type,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->speed_level,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_raise_height,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->body_height,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->euler[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw_speed,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_2_unitree_go_msg_BmsCmd_write_fields(&value->bms_cmd,p,offset);
  {size_t i;for(i=0;i<30u;++i){
  roswasmc_bounded_2_unitree_go_msg_PathPoint_write_fields(&value->path_point[i],p,offset);
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_SportModeCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_SportModeCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->gait_type,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speed_level,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->foot_raise_height,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->body_height,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->position[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->euler[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->velocity[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->yaw_speed,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_unitree_go_msg_BmsCmd_read_fields(p,n,offset,&value->bms_cmd))return 0;
  {size_t i;for(i=0;i<30u;++i){
  if(!roswasmc_bounded_2_unitree_go_msg_PathPoint_read_fields(p,n,offset,&value->path_point[i]))return 0;
  }}
  return roswasmc_bounded_2_unitree_go_msg_SportModeCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_SportModeCmd_encode(roswasmc_bounded_2_unitree_go_msg_SportModeCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_SportModeCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_SportModeCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_SportModeCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_SportModeCmd_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_SportModeCmd* value) {
  roswasmc_bounded_2_unitree_go_msg_SportModeCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<887u||length>887u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_SportModeCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/TimeSpec value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_TimeSpec {
  /** Declared field sec; canonical capacity is part of CODEC_IDENTITY. */
  int32_t sec;
  /** Declared field nanosec; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t nanosec;
} roswasmc_bounded_2_unitree_go_msg_TimeSpec;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_TIMESPEC_MAX_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_TIMESPEC_MIN_WIRE_BYTES 8u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_TIMESPEC_CODEC_IDENTITY "UWVMROS1_9ad3c6ed5a9ff0bba22593b50e1d777ae2f63480a01a744d777ee6b7dad0ecc4"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_TimeSpec_valid(roswasmc_bounded_2_unitree_go_msg_TimeSpec const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_TimeSpec_wire_size(roswasmc_bounded_2_unitree_go_msg_TimeSpec const* value) {size_t n=0;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_TimeSpec_write_fields(roswasmc_bounded_2_unitree_go_msg_TimeSpec const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->sec,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->nanosec,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_TimeSpec_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_TimeSpec* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->sec,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->nanosec,&bits,4);}*offset+=4;
  return roswasmc_bounded_2_unitree_go_msg_TimeSpec_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_TimeSpec_encode(roswasmc_bounded_2_unitree_go_msg_TimeSpec const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_TimeSpec_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_TimeSpec_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_TimeSpec_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_TimeSpec_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_TimeSpec* value) {
  roswasmc_bounded_2_unitree_go_msg_TimeSpec next={0};size_t offset=0;if(!value||(length&&!input)||length<8u||length>8u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_TimeSpec_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/SportModeState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_SportModeState {
  /** Declared field stamp; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_TimeSpec stamp;
  /** Declared field error_code; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t error_code;
  /** Declared field imu_state; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_2_unitree_go_msg_IMUState imu_state;
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field progress; canonical capacity is part of CODEC_IDENTITY. */
  float progress;
  /** Declared field gait_type; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t gait_type;
  /** Declared field foot_raise_height; canonical capacity is part of CODEC_IDENTITY. */
  float foot_raise_height;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  float position[3];
  /** Declared field body_height; canonical capacity is part of CODEC_IDENTITY. */
  float body_height;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  float velocity[3];
  /** Declared field yaw_speed; canonical capacity is part of CODEC_IDENTITY. */
  float yaw_speed;
  /** Declared field range_obstacle; canonical capacity is part of CODEC_IDENTITY. */
  float range_obstacle[4];
  /** Declared field foot_force; canonical capacity is part of CODEC_IDENTITY. */
  int16_t foot_force[4];
  /** Declared field foot_position_body; canonical capacity is part of CODEC_IDENTITY. */
  float foot_position_body[12];
  /** Declared field foot_speed_body; canonical capacity is part of CODEC_IDENTITY. */
  float foot_speed_body[12];
} roswasmc_bounded_2_unitree_go_msg_SportModeState;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_SPORTMODESTATE_MAX_WIRE_BYTES 227u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_SPORTMODESTATE_MIN_WIRE_BYTES 227u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_SPORTMODESTATE_CODEC_IDENTITY "UWVMROS1_2935b9d838087267840b379582862a15c2e1a72fe8ad2bcbf8f19af2f8054760"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_SportModeState_valid(roswasmc_bounded_2_unitree_go_msg_SportModeState const* value) {
  if(!value)return 0;
  if(!roswasmc_bounded_2_unitree_go_msg_TimeSpec_valid(&value->stamp))return 0;
  if(!roswasmc_bounded_2_unitree_go_msg_IMUState_valid(&value->imu_state))return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->progress,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_raise_height,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->body_height,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw_speed,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<4u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->range_obstacle[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_position_body[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_speed_body[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_SportModeState_wire_size(roswasmc_bounded_2_unitree_go_msg_SportModeState const* value) {size_t n=0;
  n+=roswasmc_bounded_2_unitree_go_msg_TimeSpec_wire_size(&value->stamp);
  n+=4;
  n+=roswasmc_bounded_2_unitree_go_msg_IMUState_wire_size(&value->imu_state);
  n+=1;
  n+=4;
  n+=1;
  n+=4;
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  n+=4;
  {size_t i;for(i=0;i<3u;++i){
  n+=4;
  }}
  n+=4;
  {size_t i;for(i=0;i<4u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=2;
  }}
  {size_t i;for(i=0;i<12u;++i){
  n+=4;
  }}
  {size_t i;for(i=0;i<12u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_SportModeState_write_fields(roswasmc_bounded_2_unitree_go_msg_SportModeState const* value,unsigned char* p,size_t* offset) {
  roswasmc_bounded_2_unitree_go_msg_TimeSpec_write_fields(&value->stamp,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_code,4);*offset+=4;
  roswasmc_bounded_2_unitree_go_msg_IMUState_write_fields(&value->imu_state,p,offset);
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->progress,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->gait_type,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_raise_height,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->body_height,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw_speed,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<4u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->range_obstacle[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->foot_force[i],2);*offset+=2;
  }}
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_position_body[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<12u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->foot_speed_body[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_SportModeState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_SportModeState* value) {
  if(!roswasmc_bounded_2_unitree_go_msg_TimeSpec_read_fields(p,n,offset,&value->stamp))return 0;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->error_code,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_2_unitree_go_msg_IMUState_read_fields(p,n,offset,&value->imu_state))return 0;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->progress,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->gait_type,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->foot_raise_height,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->position[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->body_height,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->velocity[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->yaw_speed,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->range_obstacle[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->foot_force[i],&bits,2);}*offset+=2;
  }}
  {size_t i;for(i=0;i<12u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->foot_position_body[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<12u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->foot_speed_body[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_2_unitree_go_msg_SportModeState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_SportModeState_encode(roswasmc_bounded_2_unitree_go_msg_SportModeState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_SportModeState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_SportModeState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_SportModeState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_SportModeState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_SportModeState* value) {
  roswasmc_bounded_2_unitree_go_msg_SportModeState next={0};size_t offset=0;if(!value||(length&&!input)||length<227u||length>227u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_SportModeState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/UwbState value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_UwbState {
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t version[2];
  /** Declared field channel; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t channel;
  /** Declared field joy_mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t joy_mode;
  /** Declared field orientation_est; canonical capacity is part of CODEC_IDENTITY. */
  float orientation_est;
  /** Declared field pitch_est; canonical capacity is part of CODEC_IDENTITY. */
  float pitch_est;
  /** Declared field distance_est; canonical capacity is part of CODEC_IDENTITY. */
  float distance_est;
  /** Declared field yaw_est; canonical capacity is part of CODEC_IDENTITY. */
  float yaw_est;
  /** Declared field tag_roll; canonical capacity is part of CODEC_IDENTITY. */
  float tag_roll;
  /** Declared field tag_pitch; canonical capacity is part of CODEC_IDENTITY. */
  float tag_pitch;
  /** Declared field tag_yaw; canonical capacity is part of CODEC_IDENTITY. */
  float tag_yaw;
  /** Declared field base_roll; canonical capacity is part of CODEC_IDENTITY. */
  float base_roll;
  /** Declared field base_pitch; canonical capacity is part of CODEC_IDENTITY. */
  float base_pitch;
  /** Declared field base_yaw; canonical capacity is part of CODEC_IDENTITY. */
  float base_yaw;
  /** Declared field joystick; canonical capacity is part of CODEC_IDENTITY. */
  float joystick[2];
  /** Declared field error_state; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t error_state;
  /** Declared field buttons; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t buttons;
  /** Declared field enabled_from_app; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t enabled_from_app;
} roswasmc_bounded_2_unitree_go_msg_UwbState;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_UWBSTATE_MAX_WIRE_BYTES 55u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_UWBSTATE_MIN_WIRE_BYTES 55u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_UWBSTATE_CODEC_IDENTITY "UWVMROS1_017fbafd61a006aeaf6cb80456ac89f429d30a562da2e0d394c6cfb406e61a34"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_UwbState_valid(roswasmc_bounded_2_unitree_go_msg_UwbState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->orientation_est,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pitch_est,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->distance_est,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw_est,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tag_roll,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tag_pitch,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tag_yaw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->base_roll,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->base_pitch,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->base_yaw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joystick[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_UwbState_wire_size(roswasmc_bounded_2_unitree_go_msg_UwbState const* value) {size_t n=0;
  {size_t i;for(i=0;i<2u;++i){
  n+=1;
  }}
  n+=1;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_UwbState_write_fields(roswasmc_bounded_2_unitree_go_msg_UwbState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->channel,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->joy_mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->orientation_est,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->pitch_est,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->distance_est,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yaw_est,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tag_roll,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tag_pitch,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tag_yaw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->base_roll,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->base_pitch,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->base_yaw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->joystick[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->error_state,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->buttons,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->enabled_from_app,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_UwbState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_UwbState* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->version[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->channel,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->joy_mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->orientation_est,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->pitch_est,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->distance_est,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->yaw_est,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tag_roll,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tag_pitch,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tag_yaw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->base_roll,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->base_pitch,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->base_yaw,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->joystick[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->error_state,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->buttons,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->enabled_from_app,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_unitree_go_msg_UwbState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_UwbState_encode(roswasmc_bounded_2_unitree_go_msg_UwbState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_UwbState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_UwbState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_UwbState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_UwbState_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_UwbState* value) {
  roswasmc_bounded_2_unitree_go_msg_UwbState next={0};size_t offset=0;if(!value||(length&&!input)||length<55u||length>55u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_UwbState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/UwbSwitch value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_UwbSwitch {
  /** Declared field enabled; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t enabled;
} roswasmc_bounded_2_unitree_go_msg_UwbSwitch;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_UWBSWITCH_MAX_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_UWBSWITCH_MIN_WIRE_BYTES 1u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_UWBSWITCH_CODEC_IDENTITY "UWVMROS1_4546618797829b3911d1533d8d6bacc9da969d40d7752424de34bbd3de4bede5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_UwbSwitch_valid(roswasmc_bounded_2_unitree_go_msg_UwbSwitch const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_UwbSwitch_wire_size(roswasmc_bounded_2_unitree_go_msg_UwbSwitch const* value) {size_t n=0;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_UwbSwitch_write_fields(roswasmc_bounded_2_unitree_go_msg_UwbSwitch const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->enabled,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_UwbSwitch_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_UwbSwitch* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->enabled,&bits,1);}*offset+=1;
  return roswasmc_bounded_2_unitree_go_msg_UwbSwitch_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_UwbSwitch_encode(roswasmc_bounded_2_unitree_go_msg_UwbSwitch const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_UwbSwitch_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_UwbSwitch_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_UwbSwitch_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_UwbSwitch_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_UwbSwitch* value) {
  roswasmc_bounded_2_unitree_go_msg_UwbSwitch next={0};size_t offset=0;if(!value||(length&&!input)||length<1u||length>1u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_UwbSwitch_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS2 unitree_go/msg/WirelessController value; no field projects away native data. */
typedef struct roswasmc_bounded_2_unitree_go_msg_WirelessController {
  /** Declared field lx; canonical capacity is part of CODEC_IDENTITY. */
  float lx;
  /** Declared field ly; canonical capacity is part of CODEC_IDENTITY. */
  float ly;
  /** Declared field rx; canonical capacity is part of CODEC_IDENTITY. */
  float rx;
  /** Declared field ry; canonical capacity is part of CODEC_IDENTITY. */
  float ry;
  /** Declared field keys; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t keys;
} roswasmc_bounded_2_unitree_go_msg_WirelessController;
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_WIRELESSCONTROLLER_MAX_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_WIRELESSCONTROLLER_MIN_WIRE_BYTES 18u
#define ROSWASMC_BOUNDED_2_UNITREE_GO_MSG_WIRELESSCONTROLLER_CODEC_IDENTITY "UWVMROS1_536e69ce5459227cb68b3c6d484a96bfa40e1400aab820f8ad0d0d62ff6ba438"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_2_unitree_go_msg_WirelessController_valid(roswasmc_bounded_2_unitree_go_msg_WirelessController const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ly,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_2_unitree_go_msg_WirelessController_wire_size(roswasmc_bounded_2_unitree_go_msg_WirelessController const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=2;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_2_unitree_go_msg_WirelessController_write_fields(roswasmc_bounded_2_unitree_go_msg_WirelessController const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->lx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ly,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rx,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ry,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->keys,2);*offset+=2;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_2_unitree_go_msg_WirelessController_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_2_unitree_go_msg_WirelessController* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->lx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ly,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rx,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ry,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->keys,&bits,2);}*offset+=2;
  return roswasmc_bounded_2_unitree_go_msg_WirelessController_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_WirelessController_encode(roswasmc_bounded_2_unitree_go_msg_WirelessController const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_2_unitree_go_msg_WirelessController_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_2_unitree_go_msg_WirelessController_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_2_unitree_go_msg_WirelessController_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_2_unitree_go_msg_WirelessController_decode(void const* input,size_t length,roswasmc_bounded_2_unitree_go_msg_WirelessController* value) {
  roswasmc_bounded_2_unitree_go_msg_WirelessController next={0};size_t offset=0;if(!value||(length&&!input)||length<18u||length>18u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_2_unitree_go_msg_WirelessController_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
