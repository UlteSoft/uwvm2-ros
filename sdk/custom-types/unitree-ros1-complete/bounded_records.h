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
/** Complete ROS1 unitree_legged_msgs/BmsCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_BmsCmd {
  /** Declared field off; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t off;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t reserve[3];
} roswasmc_bounded_1_unitree_legged_msgs_BmsCmd;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_BMSCMD_MAX_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_BMSCMD_MIN_WIRE_BYTES 4u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_BMSCMD_CODEC_IDENTITY "UWVMROS1_75732753b156fff8db3ba7e39541444bed2b71e139389a9b5bfbc49a4fa14928"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_valid(roswasmc_bounded_1_unitree_legged_msgs_BmsCmd const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<3u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_wire_size(roswasmc_bounded_1_unitree_legged_msgs_BmsCmd const* value) {size_t n=0;
  n+=1;
  {size_t i;for(i=0;i<3u;++i){
  n+=1;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_write_fields(roswasmc_bounded_1_unitree_legged_msgs_BmsCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->off,1);*offset+=1;
  {size_t i;for(i=0;i<3u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],1);*offset+=1;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_BmsCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->off,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->reserve[i],&bits,1);}*offset+=1;
  }}
  return roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_encode(roswasmc_bounded_1_unitree_legged_msgs_BmsCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_BmsCmd* value) {
  roswasmc_bounded_1_unitree_legged_msgs_BmsCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<4u||length>4u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/BmsState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_BmsState {
  /** Declared field version_h; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t version_h;
  /** Declared field version_l; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t version_l;
  /** Declared field bms_status; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t bms_status;
  /** Declared field SOC; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t SOC;
  /** Declared field current; canonical capacity is part of CODEC_IDENTITY. */
  int32_t current;
  /** Declared field cycle; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t cycle;
  /** Declared field BQ_NTC; canonical capacity is part of CODEC_IDENTITY. */
  int8_t BQ_NTC[2];
  /** Declared field MCU_NTC; canonical capacity is part of CODEC_IDENTITY. */
  int8_t MCU_NTC[2];
  /** Declared field cell_vol; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t cell_vol[10];
} roswasmc_bounded_1_unitree_legged_msgs_BmsState;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_BMSSTATE_MAX_WIRE_BYTES 34u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_BMSSTATE_MIN_WIRE_BYTES 34u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_BMSSTATE_CODEC_IDENTITY "UWVMROS1_ef0f93501f54d0ff760039d86bd1ba1739cb0c888b3a873611679484ccec094a"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_BmsState_valid(roswasmc_bounded_1_unitree_legged_msgs_BmsState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<10u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_BmsState_wire_size(roswasmc_bounded_1_unitree_legged_msgs_BmsState const* value) {size_t n=0;
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
  {size_t i;for(i=0;i<10u;++i){
  n+=2;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_BmsState_write_fields(roswasmc_bounded_1_unitree_legged_msgs_BmsState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->version_h,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->version_l,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->bms_status,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->SOC,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->current,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->cycle,2);*offset+=2;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->BQ_NTC[i],1);*offset+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->MCU_NTC[i],1);*offset+=1;
  }}
  {size_t i;for(i=0;i<10u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->cell_vol[i],2);*offset+=2;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_BmsState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_BmsState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->version_h,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->version_l,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->bms_status,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->SOC,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->current,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->cycle,&bits,2);}*offset+=2;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->BQ_NTC[i],&bits,1);}*offset+=1;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->MCU_NTC[i],&bits,1);}*offset+=1;
  }}
  {size_t i;for(i=0;i<10u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->cell_vol[i],&bits,2);}*offset+=2;
  }}
  return roswasmc_bounded_1_unitree_legged_msgs_BmsState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_BmsState_encode(roswasmc_bounded_1_unitree_legged_msgs_BmsState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_BmsState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_BmsState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_BmsState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_BmsState_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_BmsState* value) {
  roswasmc_bounded_1_unitree_legged_msgs_BmsState next={0};size_t offset=0;if(!value||(length&&!input)||length<34u||length>34u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/Cartesian value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_Cartesian {
  /** Declared field x; canonical capacity is part of CODEC_IDENTITY. */
  float x;
  /** Declared field y; canonical capacity is part of CODEC_IDENTITY. */
  float y;
  /** Declared field z; canonical capacity is part of CODEC_IDENTITY. */
  float z;
} roswasmc_bounded_1_unitree_legged_msgs_Cartesian;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_CARTESIAN_MAX_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_CARTESIAN_MIN_WIRE_BYTES 12u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_CARTESIAN_CODEC_IDENTITY "UWVMROS1_43c7c62ba2a689751a0a15e326c5c9690a9b1f166b6726a0f803b98d2b3fa722"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(roswasmc_bounded_1_unitree_legged_msgs_Cartesian const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(roswasmc_bounded_1_unitree_legged_msgs_Cartesian const* value) {size_t n=0;
  n+=4;
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(roswasmc_bounded_1_unitree_legged_msgs_Cartesian const* value,unsigned char* p,size_t* offset) {
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->x,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->y,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->z,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_Cartesian* value) {
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->x,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->y,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->z,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_Cartesian_encode(roswasmc_bounded_1_unitree_legged_msgs_Cartesian const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_Cartesian_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_Cartesian* value) {
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian next={0};size_t offset=0;if(!value||(length&&!input)||length<12u||length>12u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/LED value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_LED {
  /** Declared field r; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t r;
  /** Declared field g; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t g;
  /** Declared field b; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t b;
} roswasmc_bounded_1_unitree_legged_msgs_LED;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LED_MAX_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LED_MIN_WIRE_BYTES 3u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LED_CODEC_IDENTITY "UWVMROS1_b4efac59eaf4d3f09c069e4833dd82b5422980f6a9f27db84950243a47164ea5"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_LED_valid(roswasmc_bounded_1_unitree_legged_msgs_LED const* value) {
  if(!value)return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_LED_wire_size(roswasmc_bounded_1_unitree_legged_msgs_LED const* value) {size_t n=0;
  n+=1;
  n+=1;
  n+=1;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_LED_write_fields(roswasmc_bounded_1_unitree_legged_msgs_LED const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->r,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->g,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->b,1);*offset+=1;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_LED_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_LED* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->r,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->g,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->b,&bits,1);}*offset+=1;
  return roswasmc_bounded_1_unitree_legged_msgs_LED_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_LED_encode(roswasmc_bounded_1_unitree_legged_msgs_LED const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_LED_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_LED_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_LED_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_LED_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_LED* value) {
  roswasmc_bounded_1_unitree_legged_msgs_LED next={0};size_t offset=0;if(!value||(length&&!input)||length<3u||length>3u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_LED_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/HighCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_HighCmd {
  /** Declared field head; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t head[2];
  /** Declared field levelFlag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t levelFlag;
  /** Declared field frameReserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t frameReserve;
  /** Declared field SN; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t SN[2];
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t version[2];
  /** Declared field bandWidth; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t bandWidth;
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field gaitType; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t gaitType;
  /** Declared field speedLevel; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t speedLevel;
  /** Declared field footRaiseHeight; canonical capacity is part of CODEC_IDENTITY. */
  float footRaiseHeight;
  /** Declared field bodyHeight; canonical capacity is part of CODEC_IDENTITY. */
  float bodyHeight;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  float position[2];
  /** Declared field euler; canonical capacity is part of CODEC_IDENTITY. */
  float euler[3];
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  float velocity[2];
  /** Declared field yawSpeed; canonical capacity is part of CODEC_IDENTITY. */
  float yawSpeed;
  /** Declared field bms; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_BmsCmd bms;
  /** Declared field led; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_LED led[4];
  /** Declared field wirelessRemote; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t wirelessRemote[40];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve;
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
} roswasmc_bounded_1_unitree_legged_msgs_HighCmd;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_HIGHCMD_MAX_WIRE_BYTES 129u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_HIGHCMD_MIN_WIRE_BYTES 129u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_HIGHCMD_CODEC_IDENTITY "UWVMROS1_ec6ec8937b2629a4441adc4a21eed83fca940b18d46b970d96e05c957ac90b38"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_HighCmd_valid(roswasmc_bounded_1_unitree_legged_msgs_HighCmd const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->footRaiseHeight,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bodyHeight,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->euler[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yawSpeed,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_valid(&value->bms))return 0;
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_LED_valid(&value->led[i]))return 0;
  }}
  {size_t i;for(i=0;i<40u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_HighCmd_wire_size(roswasmc_bounded_1_unitree_legged_msgs_HighCmd const* value) {size_t n=0;
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
  n+=roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_wire_size(&value->bms);
  {size_t i;for(i=0;i<4u;++i){
  n+=roswasmc_bounded_1_unitree_legged_msgs_LED_wire_size(&value->led[i]);
  }}
  {size_t i;for(i=0;i<40u;++i){
  n+=1;
  }}
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_HighCmd_write_fields(roswasmc_bounded_1_unitree_legged_msgs_HighCmd const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->head[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->levelFlag,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->frameReserve,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->SN[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->bandWidth,2);*offset+=2;
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->gaitType,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->speedLevel,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->footRaiseHeight,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bodyHeight,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->euler[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<2u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yawSpeed,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_write_fields(&value->bms,p,offset);
  {size_t i;for(i=0;i<4u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_LED_write_fields(&value->led[i],p,offset);
  }}
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->wirelessRemote[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_HighCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_HighCmd* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->head[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->levelFlag,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->frameReserve,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->SN[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->version[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->bandWidth,&bits,2);}*offset+=2;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->gaitType,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->speedLevel,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->footRaiseHeight,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bodyHeight,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->position[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->euler[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->velocity[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->yawSpeed,&bits,4);}*offset+=4;
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_read_fields(p,n,offset,&value->bms))return 0;
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_LED_read_fields(p,n,offset,&value->led[i]))return 0;
  }}
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->wirelessRemote[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_unitree_legged_msgs_HighCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_HighCmd_encode(roswasmc_bounded_1_unitree_legged_msgs_HighCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_HighCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_HighCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_HighCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_HighCmd_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_HighCmd* value) {
  roswasmc_bounded_1_unitree_legged_msgs_HighCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<129u||length>129u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_HighCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/IMU value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_IMU {
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
} roswasmc_bounded_1_unitree_legged_msgs_IMU;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_IMU_MAX_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_IMU_MIN_WIRE_BYTES 53u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_IMU_CODEC_IDENTITY "UWVMROS1_29d9ec99e676fe8747665da588b78c8ab77a3197e281ba892ab389d2d81fa276"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_IMU_valid(roswasmc_bounded_1_unitree_legged_msgs_IMU const* value) {
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
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_IMU_wire_size(roswasmc_bounded_1_unitree_legged_msgs_IMU const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_unitree_legged_msgs_IMU_write_fields(roswasmc_bounded_1_unitree_legged_msgs_IMU const* value,unsigned char* p,size_t* offset) {
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
static inline int roswasmc_bounded_1_unitree_legged_msgs_IMU_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_IMU* value) {
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
  return roswasmc_bounded_1_unitree_legged_msgs_IMU_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_IMU_encode(roswasmc_bounded_1_unitree_legged_msgs_IMU const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_IMU_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_IMU_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_IMU_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_IMU_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_IMU* value) {
  roswasmc_bounded_1_unitree_legged_msgs_IMU next={0};size_t offset=0;if(!value||(length&&!input)||length<53u||length>53u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_IMU_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/MotorState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_MotorState {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  float q;
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  float dq;
  /** Declared field ddq; canonical capacity is part of CODEC_IDENTITY. */
  float ddq;
  /** Declared field tauEst; canonical capacity is part of CODEC_IDENTITY. */
  float tauEst;
  /** Declared field q_raw; canonical capacity is part of CODEC_IDENTITY. */
  float q_raw;
  /** Declared field dq_raw; canonical capacity is part of CODEC_IDENTITY. */
  float dq_raw;
  /** Declared field ddq_raw; canonical capacity is part of CODEC_IDENTITY. */
  float ddq_raw;
  /** Declared field temperature; canonical capacity is part of CODEC_IDENTITY. */
  int8_t temperature;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[2];
} roswasmc_bounded_1_unitree_legged_msgs_MotorState;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_MOTORSTATE_MAX_WIRE_BYTES 38u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_MOTORSTATE_MIN_WIRE_BYTES 38u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_MOTORSTATE_CODEC_IDENTITY "UWVMROS1_be540cb22db5f08b6b9a8082777c76244e80f3148e9cfd63f510e4b2acdc0abf"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_MotorState_valid(roswasmc_bounded_1_unitree_legged_msgs_MotorState const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tauEst,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q_raw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq_raw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq_raw,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<2u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_MotorState_wire_size(roswasmc_bounded_1_unitree_legged_msgs_MotorState const* value) {size_t n=0;
  n+=1;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=4;
  n+=1;
  {size_t i;for(i=0;i<2u;++i){
  n+=4;
  }}
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_MotorState_write_fields(roswasmc_bounded_1_unitree_legged_msgs_MotorState const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tauEst,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q_raw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq_raw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->ddq_raw,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->temperature,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_MotorState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_MotorState* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ddq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tauEst,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q_raw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq_raw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->ddq_raw,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->temperature,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_1_unitree_legged_msgs_MotorState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_MotorState_encode(roswasmc_bounded_1_unitree_legged_msgs_MotorState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_MotorState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_MotorState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_MotorState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_MotorState_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_MotorState* value) {
  roswasmc_bounded_1_unitree_legged_msgs_MotorState next={0};size_t offset=0;if(!value||(length&&!input)||length<38u||length>38u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/HighState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_HighState {
  /** Declared field head; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t head[2];
  /** Declared field levelFlag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t levelFlag;
  /** Declared field frameReserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t frameReserve;
  /** Declared field SN; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t SN[2];
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t version[2];
  /** Declared field bandWidth; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t bandWidth;
  /** Declared field imu; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_IMU imu;
  /** Declared field motorState; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_MotorState motorState[20];
  /** Declared field bms; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_BmsState bms;
  /** Declared field footForce; canonical capacity is part of CODEC_IDENTITY. */
  int16_t footForce[4];
  /** Declared field footForceEst; canonical capacity is part of CODEC_IDENTITY. */
  int16_t footForceEst[4];
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field progress; canonical capacity is part of CODEC_IDENTITY. */
  float progress;
  /** Declared field gaitType; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t gaitType;
  /** Declared field footRaiseHeight; canonical capacity is part of CODEC_IDENTITY. */
  float footRaiseHeight;
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  float position[3];
  /** Declared field bodyHeight; canonical capacity is part of CODEC_IDENTITY. */
  float bodyHeight;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  float velocity[3];
  /** Declared field yawSpeed; canonical capacity is part of CODEC_IDENTITY. */
  float yawSpeed;
  /** Declared field rangeObstacle; canonical capacity is part of CODEC_IDENTITY. */
  float rangeObstacle[4];
  /** Declared field footPosition2Body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian footPosition2Body[4];
  /** Declared field footSpeed2Body; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian footSpeed2Body[4];
  /** Declared field wirelessRemote; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t wirelessRemote[40];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve;
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
} roswasmc_bounded_1_unitree_legged_msgs_HighState;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_HIGHSTATE_MAX_WIRE_BYTES 1087u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_HIGHSTATE_MIN_WIRE_BYTES 1087u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_HIGHSTATE_CODEC_IDENTITY "UWVMROS1_82ee0efaab21b5cb89e99f5c0f5daec583b48dd5feb2dd6a7e09ba9d9110b53d"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_HighState_valid(roswasmc_bounded_1_unitree_legged_msgs_HighState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_IMU_valid(&value->imu))return 0;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorState_valid(&value->motorState[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsState_valid(&value->bms))return 0;
  {size_t i;for(i=0;i<4u;++i){
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->progress,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->footRaiseHeight,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bodyHeight,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yawSpeed,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<4u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rangeObstacle[i],4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value->footPosition2Body[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value->footSpeed2Body[i]))return 0;
  }}
  {size_t i;for(i=0;i<40u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_HighState_wire_size(roswasmc_bounded_1_unitree_legged_msgs_HighState const* value) {size_t n=0;
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
  n+=roswasmc_bounded_1_unitree_legged_msgs_IMU_wire_size(&value->imu);
  {size_t i;for(i=0;i<20u;++i){
  n+=roswasmc_bounded_1_unitree_legged_msgs_MotorState_wire_size(&value->motorState[i]);
  }}
  n+=roswasmc_bounded_1_unitree_legged_msgs_BmsState_wire_size(&value->bms);
  {size_t i;for(i=0;i<4u;++i){
  n+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=2;
  }}
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
  n+=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value->footPosition2Body[i]);
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value->footSpeed2Body[i]);
  }}
  {size_t i;for(i=0;i<40u;++i){
  n+=1;
  }}
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_HighState_write_fields(roswasmc_bounded_1_unitree_legged_msgs_HighState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->head[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->levelFlag,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->frameReserve,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->SN[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->bandWidth,2);*offset+=2;
  roswasmc_bounded_1_unitree_legged_msgs_IMU_write_fields(&value->imu,p,offset);
  {size_t i;for(i=0;i<20u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_MotorState_write_fields(&value->motorState[i],p,offset);
  }}
  roswasmc_bounded_1_unitree_legged_msgs_BmsState_write_fields(&value->bms,p,offset);
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->footForce[i],2);*offset+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->footForceEst[i],2);*offset+=2;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->progress,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  uwvm_bounded_put(p+*offset,(uint64_t)value->gaitType,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->footRaiseHeight,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->position[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->bodyHeight,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<3u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->velocity[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->yawSpeed,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<4u;++i){
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->rangeObstacle[i],4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  }}
  {size_t i;for(i=0;i<4u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(&value->footPosition2Body[i],p,offset);
  }}
  {size_t i;for(i=0;i<4u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(&value->footSpeed2Body[i],p,offset);
  }}
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->wirelessRemote[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_HighState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_HighState* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->head[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->levelFlag,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->frameReserve,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->SN[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->version[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->bandWidth,&bits,2);}*offset+=2;
  if(!roswasmc_bounded_1_unitree_legged_msgs_IMU_read_fields(p,n,offset,&value->imu))return 0;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorState_read_fields(p,n,offset,&value->motorState[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsState_read_fields(p,n,offset,&value->bms))return 0;
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->footForce[i],&bits,2);}*offset+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->footForceEst[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->progress,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->gaitType,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->footRaiseHeight,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->position[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->bodyHeight,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->velocity[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->yawSpeed,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->rangeObstacle[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(p,n,offset,&value->footPosition2Body[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(p,n,offset,&value->footSpeed2Body[i]))return 0;
  }}
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->wirelessRemote[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_unitree_legged_msgs_HighState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_HighState_encode(roswasmc_bounded_1_unitree_legged_msgs_HighState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_HighState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_HighState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_HighState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_HighState_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_HighState* value) {
  roswasmc_bounded_1_unitree_legged_msgs_HighState next={0};size_t offset=0;if(!value||(length&&!input)||length<1087u||length>1087u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_HighState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/MotorCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_MotorCmd {
  /** Declared field mode; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t mode;
  /** Declared field q; canonical capacity is part of CODEC_IDENTITY. */
  float q;
  /** Declared field dq; canonical capacity is part of CODEC_IDENTITY. */
  float dq;
  /** Declared field tau; canonical capacity is part of CODEC_IDENTITY. */
  float tau;
  /** Declared field Kp; canonical capacity is part of CODEC_IDENTITY. */
  float Kp;
  /** Declared field Kd; canonical capacity is part of CODEC_IDENTITY. */
  float Kd;
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve[3];
} roswasmc_bounded_1_unitree_legged_msgs_MotorCmd;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_MOTORCMD_MAX_WIRE_BYTES 33u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_MOTORCMD_MIN_WIRE_BYTES 33u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_MOTORCMD_CODEC_IDENTITY "UWVMROS1_632b25f209e72857182d92752aa72e3ab47a45e895cc352ee6dc05f1c1168083"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_valid(roswasmc_bounded_1_unitree_legged_msgs_MotorCmd const* value) {
  if(!value)return 0;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->Kp,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->Kd,4);if((bits&UINT32_C(0x7f800000))==UINT32_C(0x7f800000))return 0;}
  {size_t i;for(i=0;i<3u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_wire_size(roswasmc_bounded_1_unitree_legged_msgs_MotorCmd const* value) {size_t n=0;
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
static inline void roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_write_fields(roswasmc_bounded_1_unitree_legged_msgs_MotorCmd const* value,unsigned char* p,size_t* offset) {
  uwvm_bounded_put(p+*offset,(uint64_t)value->mode,1);*offset+=1;
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->q,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->dq,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->tau,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->Kp,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {uint32_t bits;uwvm_bounded_copy(&bits,&value->Kd,4);uwvm_bounded_put(p+*offset,bits,4);*offset+=4;}
  {size_t i;for(i=0;i<3u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve[i],4);*offset+=4;
  }}
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_MotorCmd* value) {
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->mode,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->q,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->dq,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tau,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->Kp,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->Kd,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<3u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve[i],&bits,4);}*offset+=4;
  }}
  return roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_encode(roswasmc_bounded_1_unitree_legged_msgs_MotorCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_MotorCmd* value) {
  roswasmc_bounded_1_unitree_legged_msgs_MotorCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<33u||length>33u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/LowCmd value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_LowCmd {
  /** Declared field head; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t head[2];
  /** Declared field levelFlag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t levelFlag;
  /** Declared field frameReserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t frameReserve;
  /** Declared field SN; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t SN[2];
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t version[2];
  /** Declared field bandWidth; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t bandWidth;
  /** Declared field motorCmd; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_MotorCmd motorCmd[20];
  /** Declared field bms; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_BmsCmd bms;
  /** Declared field wirelessRemote; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t wirelessRemote[40];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve;
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
} roswasmc_bounded_1_unitree_legged_msgs_LowCmd;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LOWCMD_MAX_WIRE_BYTES 734u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LOWCMD_MIN_WIRE_BYTES 734u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LOWCMD_CODEC_IDENTITY "UWVMROS1_11d3ac7c497f01fbf6624c39ec71fa613c462cd62acb20c1f263cdde672e9181"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_LowCmd_valid(roswasmc_bounded_1_unitree_legged_msgs_LowCmd const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_valid(&value->motorCmd[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_valid(&value->bms))return 0;
  {size_t i;for(i=0;i<40u;++i){
  }}
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_LowCmd_wire_size(roswasmc_bounded_1_unitree_legged_msgs_LowCmd const* value) {size_t n=0;
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
  n+=roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_wire_size(&value->motorCmd[i]);
  }}
  n+=roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_wire_size(&value->bms);
  {size_t i;for(i=0;i<40u;++i){
  n+=1;
  }}
  n+=4;
  n+=4;
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_LowCmd_write_fields(roswasmc_bounded_1_unitree_legged_msgs_LowCmd const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->head[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->levelFlag,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->frameReserve,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->SN[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->bandWidth,2);*offset+=2;
  {size_t i;for(i=0;i<20u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_write_fields(&value->motorCmd[i],p,offset);
  }}
  roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_write_fields(&value->bms,p,offset);
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->wirelessRemote[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_LowCmd_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_LowCmd* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->head[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->levelFlag,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->frameReserve,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->SN[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->version[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->bandWidth,&bits,2);}*offset+=2;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorCmd_read_fields(p,n,offset,&value->motorCmd[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsCmd_read_fields(p,n,offset,&value->bms))return 0;
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->wirelessRemote[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  return roswasmc_bounded_1_unitree_legged_msgs_LowCmd_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_LowCmd_encode(roswasmc_bounded_1_unitree_legged_msgs_LowCmd const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_LowCmd_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_LowCmd_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_LowCmd_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_LowCmd_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_LowCmd* value) {
  roswasmc_bounded_1_unitree_legged_msgs_LowCmd next={0};size_t offset=0;if(!value||(length&&!input)||length<734u||length>734u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_LowCmd_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
/** Complete ROS1 unitree_legged_msgs/LowState value; no field projects away native data. */
typedef struct roswasmc_bounded_1_unitree_legged_msgs_LowState {
  /** Declared field head; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t head[2];
  /** Declared field levelFlag; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t levelFlag;
  /** Declared field frameReserve; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t frameReserve;
  /** Declared field SN; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t SN[2];
  /** Declared field version; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t version[2];
  /** Declared field bandWidth; canonical capacity is part of CODEC_IDENTITY. */
  uint16_t bandWidth;
  /** Declared field imu; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_IMU imu;
  /** Declared field motorState; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_MotorState motorState[20];
  /** Declared field bms; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_BmsState bms;
  /** Declared field footForce; canonical capacity is part of CODEC_IDENTITY. */
  int16_t footForce[4];
  /** Declared field footForceEst; canonical capacity is part of CODEC_IDENTITY. */
  int16_t footForceEst[4];
  /** Declared field tick; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t tick;
  /** Declared field wirelessRemote; canonical capacity is part of CODEC_IDENTITY. */
  uint8_t wirelessRemote[40];
  /** Declared field reserve; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t reserve;
  /** Declared field crc; canonical capacity is part of CODEC_IDENTITY. */
  uint32_t crc;
  /** Declared field eeForceRaw; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian eeForceRaw[4];
  /** Declared field eeForce; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian eeForce[4];
  /** Declared field position; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian position;
  /** Declared field velocity; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian velocity;
  /** Declared field velocity_w; canonical capacity is part of CODEC_IDENTITY. */
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian velocity_w;
} roswasmc_bounded_1_unitree_legged_msgs_LowState;
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LOWSTATE_MAX_WIRE_BYTES 1069u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LOWSTATE_MIN_WIRE_BYTES 1069u
#define ROSWASMC_BOUNDED_1_UNITREE_LEGGED_MSGS_LOWSTATE_CODEC_IDENTITY "UWVMROS1_37425ed49c0eebd762b6084ee413c7706fc43defc524414d3f50cdd2d25552c0"
/** Validate every initialized field without allocation, writes or ROS calls. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_LowState_valid(roswasmc_bounded_1_unitree_legged_msgs_LowState const* value) {
  if(!value)return 0;
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  {size_t i;for(i=0;i<2u;++i){
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_IMU_valid(&value->imu))return 0;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorState_valid(&value->motorState[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsState_valid(&value->bms))return 0;
  {size_t i;for(i=0;i<4u;++i){
  }}
  {size_t i;for(i=0;i<4u;++i){
  }}
  {size_t i;for(i=0;i<40u;++i){
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value->eeForceRaw[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value->eeForce[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value->position))return 0;
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value->velocity))return 0;
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_valid(&value->velocity_w))return 0;
  return 1;
}
/** Return exact active-prefix wire size; the full value must already be valid. */
static inline size_t roswasmc_bounded_1_unitree_legged_msgs_LowState_wire_size(roswasmc_bounded_1_unitree_legged_msgs_LowState const* value) {size_t n=0;
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
  n+=roswasmc_bounded_1_unitree_legged_msgs_IMU_wire_size(&value->imu);
  {size_t i;for(i=0;i<20u;++i){
  n+=roswasmc_bounded_1_unitree_legged_msgs_MotorState_wire_size(&value->motorState[i]);
  }}
  n+=roswasmc_bounded_1_unitree_legged_msgs_BmsState_wire_size(&value->bms);
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
  n+=4;
  n+=4;
  {size_t i;for(i=0;i<4u;++i){
  n+=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value->eeForceRaw[i]);
  }}
  {size_t i;for(i=0;i<4u;++i){
  n+=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value->eeForce[i]);
  }}
  n+=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value->position);
  n+=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value->velocity);
  n+=roswasmc_bounded_1_unitree_legged_msgs_Cartesian_wire_size(&value->velocity_w);
  return n;
}
/** Internal writer enters only after full validation, capacity and overlap checks. */
static inline void roswasmc_bounded_1_unitree_legged_msgs_LowState_write_fields(roswasmc_bounded_1_unitree_legged_msgs_LowState const* value,unsigned char* p,size_t* offset) {
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->head[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->levelFlag,1);*offset+=1;
  uwvm_bounded_put(p+*offset,(uint64_t)value->frameReserve,1);*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->SN[i],4);*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->version[i],4);*offset+=4;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->bandWidth,2);*offset+=2;
  roswasmc_bounded_1_unitree_legged_msgs_IMU_write_fields(&value->imu,p,offset);
  {size_t i;for(i=0;i<20u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_MotorState_write_fields(&value->motorState[i],p,offset);
  }}
  roswasmc_bounded_1_unitree_legged_msgs_BmsState_write_fields(&value->bms,p,offset);
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->footForce[i],2);*offset+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->footForceEst[i],2);*offset+=2;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->tick,4);*offset+=4;
  {size_t i;for(i=0;i<40u;++i){
  uwvm_bounded_put(p+*offset,(uint64_t)value->wirelessRemote[i],1);*offset+=1;
  }}
  uwvm_bounded_put(p+*offset,(uint64_t)value->reserve,4);*offset+=4;
  uwvm_bounded_put(p+*offset,(uint64_t)value->crc,4);*offset+=4;
  {size_t i;for(i=0;i<4u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(&value->eeForceRaw[i],p,offset);
  }}
  {size_t i;for(i=0;i<4u;++i){
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(&value->eeForce[i],p,offset);
  }}
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(&value->position,p,offset);
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(&value->velocity,p,offset);
  roswasmc_bounded_1_unitree_legged_msgs_Cartesian_write_fields(&value->velocity_w,p,offset);
}
/** Internal reader fills only the public decoder temporary and checks every cursor move. */
static inline int roswasmc_bounded_1_unitree_legged_msgs_LowState_read_fields(unsigned char const* p,size_t n,size_t* offset,roswasmc_bounded_1_unitree_legged_msgs_LowState* value) {
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->head[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->levelFlag,&bits,1);}*offset+=1;
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->frameReserve,&bits,1);}*offset+=1;
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->SN[i],&bits,4);}*offset+=4;
  }}
  {size_t i;for(i=0;i<2u;++i){
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->version[i],&bits,4);}*offset+=4;
  }}
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->bandWidth,&bits,2);}*offset+=2;
  if(!roswasmc_bounded_1_unitree_legged_msgs_IMU_read_fields(p,n,offset,&value->imu))return 0;
  {size_t i;for(i=0;i<20u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_MotorState_read_fields(p,n,offset,&value->motorState[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_BmsState_read_fields(p,n,offset,&value->bms))return 0;
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->footForce[i],&bits,2);}*offset+=2;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(*offset>n||n-*offset<2)return 0;{uint16_t bits=(uint16_t)uwvm_bounded_get(p+*offset,2);uwvm_bounded_copy(&value->footForceEst[i],&bits,2);}*offset+=2;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->tick,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<40u;++i){
  if(*offset>n||n-*offset<1)return 0;{uint8_t bits=(uint8_t)uwvm_bounded_get(p+*offset,1);uwvm_bounded_copy(&value->wirelessRemote[i],&bits,1);}*offset+=1;
  }}
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->reserve,&bits,4);}*offset+=4;
  if(*offset>n||n-*offset<4)return 0;{uint32_t bits=(uint32_t)uwvm_bounded_get(p+*offset,4);uwvm_bounded_copy(&value->crc,&bits,4);}*offset+=4;
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(p,n,offset,&value->eeForceRaw[i]))return 0;
  }}
  {size_t i;for(i=0;i<4u;++i){
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(p,n,offset,&value->eeForce[i]))return 0;
  }}
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(p,n,offset,&value->position))return 0;
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(p,n,offset,&value->velocity))return 0;
  if(!roswasmc_bounded_1_unitree_legged_msgs_Cartesian_read_fields(p,n,offset,&value->velocity_w))return 0;
  return roswasmc_bounded_1_unitree_legged_msgs_LowState_valid(value);
}
/** Encode transactionally: preserve output/length on invalid values or insufficient capacity. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_LowState_encode(roswasmc_bounded_1_unitree_legged_msgs_LowState const* value,void* output,size_t capacity,size_t* length) {
  size_t n,offset=0;if(!length||!roswasmc_bounded_1_unitree_legged_msgs_LowState_valid(value))return UWVM_BOUNDED_INVALID;n=roswasmc_bounded_1_unitree_legged_msgs_LowState_wire_size(value);
  if(capacity<n)return UWVM_BOUNDED_TOO_LARGE;if((n&&!output)||uwvm_bounded_overlap(value,sizeof *value,output,n)||uwvm_bounded_overlap(length,sizeof *length,output,n)||uwvm_bounded_overlap(length,sizeof *length,value,sizeof *value))return UWVM_BOUNDED_INVALID;
  roswasmc_bounded_1_unitree_legged_msgs_LowState_write_fields(value,(unsigned char*)output,&offset);*length=offset;return 0;
}
/** Decode exact bytes into a temporary; refused/truncated/trailing input leaves value unchanged. */
static inline uint32_t roswasmc_bounded_1_unitree_legged_msgs_LowState_decode(void const* input,size_t length,roswasmc_bounded_1_unitree_legged_msgs_LowState* value) {
  roswasmc_bounded_1_unitree_legged_msgs_LowState next={0};size_t offset=0;if(!value||(length&&!input)||length<1069u||length>1069u||uwvm_bounded_overlap(value,sizeof *value,input,length))return UWVM_BOUNDED_INVALID;
  if(!roswasmc_bounded_1_unitree_legged_msgs_LowState_read_fields((unsigned char const*)input,length,&offset,&next)||offset!=length)return UWVM_BOUNDED_INVALID;*value=next;return 0;
}
#undef UWVM_BOUNDED_ASSERT
#endif
